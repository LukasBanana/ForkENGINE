/*
 * Rich-text renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/RichTextRenderer.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidStateException.h"
#include "Video/RenderSystem/RenderContext.h"

#include <cctype>


namespace Fork
{

namespace Video
{


RichTextRenderer::RichTextRenderer(const PrimitiveRendererPtr& renderer, const TextureFontPtr& initialFont) :
    renderer_{ renderer }
{
    ASSERT_POINTER(renderer);

    SetupSeparatorFilter(nullptr);

    /* Setup initial drawing range */
    viewSize        = RenderContext::Active()->GetVideoMode().resolution.Cast<float>();
    textPosition    = { textBorder.width, textBorder.height };

    /* Setup initial state of the stacks */
    PushColor({ 1, 1, 1, 1 });

    if (initialFont)
        PushFont(initialFont);
    else
        PushFont(GetStandardFontDesc());
}
RichTextRenderer::~RichTextRenderer()
{
    /* Release all font objects */
    //for (auto& font : fontMap_)
    //    renderer_->ReleaseTextureFont(font.second);
}

void RichTextRenderer::SetupSeparatorFilter(const StringSeparatorFilter& separatorFilter)
{
    if (separatorFilter)
        separatorFilter_ = separatorFilter;
    else
        separatorFilter_ = [](wchar_t chr)
        {
            return std::isalnum(static_cast<int>(chr)) == 0;
        };
}

void RichTextRenderer::PushFont(const Font::Description& desc)
{
    /* Check if font was already created */
    auto it = fontMap_.find(desc);
    if (it == fontMap_.end())
    {
        /* Create new font */
        auto font = renderer_->CreateTextureFont(desc);
        fontMap_[desc] = font;
        PushFont(font);
    }
    else
        PushFont(it->second);
}

void RichTextRenderer::PushFont(const TextureFontPtr& font)
{
    if (font)
        fontStack_.push(font);
}

void RichTextRenderer::PopFont()
{
    if (fontStack_.size() > 1)
        fontStack_.pop();
}

void RichTextRenderer::PushFontSize(int fontSize)
{
    /* Change the current font size */
    auto fontDesc = CurrentFont()->GetDesc();
    fontDesc.size.height = fontSize;
    PushFont(fontDesc);
}

void RichTextRenderer::PushFontFlags(const Font::Flags::DataType fontFlags)
{
    /* Add specified flags to current font and push the new font description */
    auto fontDesc = CurrentFont()->GetDesc();
    fontDesc.flags |= fontFlags;
    PushFont(fontDesc);
}

void RichTextRenderer::PushFontBold()
{
    PushFontFlags(Font::Flags::Bold);
}

void RichTextRenderer::PushFontItalic()
{
    PushFontFlags(Font::Flags::Italic);
}

void RichTextRenderer::PushFontUnderlined()
{
    PushFontFlags(Font::Flags::Underlined);
}

void RichTextRenderer::PushFontStrikeOut()
{
    PushFontFlags(Font::Flags::StrikeOut);
}

void RichTextRenderer::PushColor(const ColorRGBAf& color)
{
    colorStack_.push(color);
}

void RichTextRenderer::PopColor()
{
    if (colorStack_.size() > 1)
        colorStack_.pop();
}

const TextureFont* RichTextRenderer::CurrentFont() const
{
    return fontStack_.top().get();
}

const ColorRGBAf& RichTextRenderer::CurrentColor() const
{
    return colorStack_.top();
}

void RichTextRenderer::Text(const Math::Point2f& position, const std::wstring& textUTF16)
{
    textPosition = position;
    Text(textUTF16);
}

void RichTextRenderer::Text(const std::wstring& textUTF16)
{
    const auto font = CurrentFont();
    const auto& glyphSet = font->GetGlyphSet();

    std::wstring text = textUTF16;
    RemoveSpecialChars(text);
    
    auto textWidth = static_cast<float>(font->TextWidth(text));

    if (IsNewLineRequired(textWidth))
    {
        bool allowCrop = false;

        while (!text.empty())
        {
            /* Find sub text which fits into the current line */
            auto subText = TrimTextToFit(text, allowCrop);
            auto subTextWidth = static_cast<float>(font->TextWidth(subText));

            /* Allow text crop only after the first text part has been trimed */
            allowCrop = true;
            
            if (!subText.empty())
            {
                /* Trim main text and update text width */
                text = text.substr(subText.size());
                textWidth = static_cast<float>(font->TextWidth(text));
            }

            AddText(subText, subTextWidth);

            /* Check if a new line must be added */
            if (IsNewLineRequired(textWidth))
                NewLine();
        }
    }
    else
    {
        /* Draw text on the whole piece */
        AddText(text, textWidth);
    }
}

void RichTextRenderer::NewLine()
{
    /* Flush text field buffer (draws the final text) */
    const auto maxFontHeight = textFieldMaxHeight_;

    Flush();

    /* Setup new text position */
    textPosition.y += maxFontHeight;
    textPosition.x = textBorder.width;
}

void RichTextRenderer::Flush()
{
    if (!textFieldBuffer_.empty())
    {
        /*
        Draw the text entries as described in
        the text field buffer, then clear the buffer.
        */
        for (const auto& textField : textFieldBuffer_)
        {
            /*
            Setup final Y-axis for text position
            -> Maximal font height in the current text line affects the other text positions.
            */
            const auto textPosY = textField.position.y + static_cast<float>(textFieldMaxHeight_ - textField.font->GetDesc().size.height);

            /* Draw current text field */
            renderer_->DrawText2D(
                textField.font,
                { viewOffset.x + textField.position.x, viewOffset.y + textPosY },
                textField.text,
                textField.color
            );
        }

        /* Clear text field buffer and reset maximal font height */
        textFieldBuffer_.clear();
        textFieldMaxHeight_ = minTextHeight;
    }
}

Font::Description RichTextRenderer::GetStandardFontDesc()
{
    return { "Times New Roman", 22 };
}


/*
 * ======= Private: =======
 */

void RichTextRenderer::AddTextField(const TextField& textField)
{
    /* Store maximal font height */
    auto fontHeight = textField.font->GetDesc().size.height;

    if (textFieldMaxHeight_ < fontHeight)
        textFieldMaxHeight_ = fontHeight;

    /* Add text field to the buffer */
    textFieldBuffer_.push_back(textField);
}

void RichTextRenderer::AddText(const std::wstring& text, float textWidth)
{
    if (!text.empty())
    {
        AddTextField({ CurrentFont(), textPosition, text, CurrentColor() });
        textPosition.x += textWidth;
    }
}

/*
This function trims the specified string, so that it fits into the current 'viewSize' width.
It trys not to crop concatenated identifiers, by storing the last identifier-separation character (named 'lastSepPos' here).
*/
std::wstring RichTextRenderer::TrimTextToFit(const std::wstring& str, bool allowCrop) const
{
    const auto font = CurrentFont();
    const auto& glyphSet = font->GetGlyphSet();

    /* Get maximal width for the string */
    const auto maxWidth = viewSize.width - textBorder.width - textPosition.x;

    /* Initialize 'last separation position' (lastSepPos) with maximal string size */
    size_t pos = 0;
    size_t lastSepPos = std::string::npos;

    int subTextWidth = 0;
    bool prevCharWasSep = false;

    for (auto num = str.size(); pos < num; ++pos)
    {
        /* Increment text width */
        auto chr = str[pos];
        subTextWidth += glyphSet[chr].TotalWidth();

        /* Check if text is too large with current character */
        if (subTextWidth > maxWidth)
        {
            /* Return trimed string (minimal one character) */
            if (lastSepPos != std::string::npos)
            {
                if (lastSepPos == 0)
                    ++lastSepPos;
                return str.substr(0, lastSepPos);
            }

            /* If text crop is not allowed, return an empty string */
            if (!allowCrop)
                return L"";

            /* Clamp text size to one character */
            return str.substr(0, (pos > 0 ? pos : 1));
        }

        /* Store last white space position */
        if (separatorFilter_(chr))
        {
            lastSepPos = pos;
            prevCharWasSep = true;
        }
        else if (prevCharWasSep)
        {
            lastSepPos = pos;
            prevCharWasSep = false;
        }
    }

    return str;
}

bool RichTextRenderer::IsNewLineRequired(float width) const
{
    return textPosition.x + width > viewSize.width - textBorder.width;
}


} // /namespace Video

} // /namespace Fork



// ========================