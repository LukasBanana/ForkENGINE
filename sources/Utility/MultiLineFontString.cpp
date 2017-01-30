/*
 * Multi-line font string file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/MultiLineFontString.h"


namespace Fork
{

namespace Utility
{


MultiLineFontString::MultiLineFontString(const Video::Font* font, const std::string& text, int maxWidth) :
    font_       { font     },
    text_       { text     },
    maxWidth_   { maxWidth }
{
    UpdateLines();
}

void MultiLineFontString::ChangeText(const std::string& text)
{
    /* Update lines only if text has changed */
    if (text_ != text)
    {
        text_ = text;
        UpdateLines();
    }
}

void MultiLineFontString::ChangeFont(const Video::Font* font)
{
    /* Always update lines here */
    font_ = font;
    UpdateLines();
}

void MultiLineFontString::ChangeMaxWidth(int maxWidth)
{
    /* Update lines only if maximal width has changed */
    if (maxWidth_ != maxWidth)
    {
        maxWidth_ = maxWidth;
        UpdateLines();
    }
}

void MultiLineFontString::Change(const Video::Font* font, const std::string& text, int maxWidth)
{
    /* Update all configurations at once */
    font_ = font;
    text_ = text;
    maxWidth_ = maxWidth;
    UpdateLines();
}

int MultiLineFontString::FontHeight() const
{
    return font_ != nullptr ? font_->FontHeight() : 0;
}


/*
 * ======= Private: =======
 */

void MultiLineFontString::UpdateLines()
{
    lines_.clear();

    if (font_ && !text_.empty() && maxWidth_ > 0)
    {
        const auto& glyphSet = font_->GetGlyphSet();

        int width;
        
        /* Find all trimed lines */
        for (size_t pos = 0, size = 0, num = text_.size(); ( pos < num ); pos += size)
        {
            for (size = 0, width = 0; ( pos + size < num && width < maxWidth_ ); ++size)
            {
                /* Add width of current character */
                auto chr = text_[pos + size];
                width += glyphSet[chr].TotalWidth();
            }

            /* Add new line to list */
            if (size == 0 && pos < num)
                ++size;

            if (size > 0)
                lines_.push_back(text_.substr(pos, size));
        }
    }
}


} // /namespace Utility

} // /namespace Fork



// ========================