/*
 * Rich-text renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RICH_TEXT_RENDERER_H__
#define __FORK_RICH_TEXT_RENDERER_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"

#include <map>
#include <stack>
#include <vector>
#include <functional>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(RichTextRenderer);

/**
The rich-text renderer class can be used for comfortable text drawing with varying fonts.
It uses a PrimitiveRenderer to draw the text. This class is also used for the HTML renderer.
\see PrimitiveRenderer
*/
class FORK_EXPORT RichTextRenderer
{
    
    public:
        
        /**
        Function signature for the string separator filter.
        \param[in] chr Specifies the current UTF-16 character.
        \return True if the specified character is a separator, which allows a string to be croped at that position.
        */
        typedef std::function<bool (wchar_t chr)> StringSeparatorFilter;

        /**
        Main constructor.
        \param[in] renderer Shared pointer to the primitive renderer.
        \param[in] initialFont Shared pointer to the initial textured font.
        If this is null a standard font will be created (see GetStandardFontDesc).
        \throws NullPointerException If 'renderer' is null.
        \remarks The initial color is { 1, 1, 1, 1 }.
        \see GetStandardFontDesc
        \see PushFont
        \see PushColor
        */
        RichTextRenderer(const PrimitiveRendererPtr& renderer, const TextureFontPtr& initialFont = nullptr);
        ~RichTextRenderer();

        /**
        Sets the string separator filter function.
        \param[in] separatorFilter Specifies the new filter function. If this is null, the default filter function is used.
        \remarks The default filter function filters all characters as separators which are not alpha-numeric (see 'std::isalnum').
        \see StringSeparatorFilter
        */
        void SetupSeparatorFilter(const StringSeparatorFilter& separatorFilter);

        /**
        Pushes the font with the specified description onto the stack.
        If the font has not already been created, this will be done 'on the fly'.
        \param[in] desc Specifies the font descriptions.
        \see Font::Description
        */
        void PushFont(const Font::Description& desc);
        /**
        Pushes the specified font object onto the stack.
        \param[in] font Shared-pointer to the font which is to be pushed onto the stack.
        If 'font' is null, this call has no effect.
        */
        void PushFont(const TextureFontPtr& font);
        /**
        Pops the previous font from the stack.
        If the stack only contains one element, this call will be ignored.
        */
        void PopFont();

        /**
        Pushes the current font with new size.
        \param[in] fontSize Specifies the new font size.
        \see PushFont
        */
        void PushFontSize(int fontSize);
        /**
        Pushes the current font with additional flags.
        \param[in] fontFlags Specifies the additional flags with will be added to the current font.
        \see Font::Flags
        */
        void PushFontFlags(const Font::Flags::DataType fontFlags);
        /**
        Pushes the current font with additional flag: 'bold' type.
        \see PushFontFlags
        */
        void PushFontBold();
        /**
        Pushes the current font with additional flag: 'italic' type.
        \see PushFontFlags
        */
        void PushFontItalic();
        /**
        Pushes the current font with additional flag: 'underlined' type.
        \see PushFontFlags
        */
        void PushFontUnderlined();
        /**
        Pushes the current font with additional flag: 'strike out' type.
        \see PushFontFlags
        */
        void PushFontStrikeOut();

        /**
        Pushes the specified color onto the stack.
        \param[in] color Specifies the RGBA color which is to be pushed onto the stack.
        */
        void PushColor(const ColorRGBAf& color);
        /**
        Pops the previous color from the stack.
        If the stack only contains one element, this call will be ignored.
        */
        void PopColor();

        /**
        Draws the specified text with the current font and color.
        \param[in] position Specifies the 2D text position which is used for the 'PrimitiveRenderer::DrawText2D' function.
        \param[in] textUTF16 Specifies the UTF-16 text which is to be drawn.
        \see PrimitiveRenderer::DrawText2D
        \see CurrentFont
        \see CurrentColor
        */
        void Text(const Math::Point2f& position, const std::wstring& textUTF16);
        /**
        Draws the specified text with the current font and color.
        Also the position will be computed automatically.
        \see Text(const Math::Point2f&, const std::wstring&)
        */
        void Text(const std::wstring& textUTF16);

        /**
        Adds a new line. This will modify the 'textPosition' member.
        \see textPosition
        */
        void NewLine();

        /**
        Flushes the current text drawing command buffer.
        This should be called when all drawing operations are done.
        */
        void Flush();

        /**
        Returns a constant raw-pointer to the current font object.
        This will never be null.
        \see PushFont
        */
        const TextureFont* CurrentFont() const;
        /**
        Returns a current color. The initial color is { 1, 1, 1, 1 }.
        \see PushColor
        */
        const ColorRGBAf& CurrentColor() const;
        
        //! Returns the standard font description. This is used when the initial font is null.
        static Font::Description GetStandardFontDesc();

        /**
        Specifies the size of the view where the text will be rendered.
        This will be initialized with the active render context resolution at creation time of this class.
        */
        Math::Size2f viewSize;
        /**
        Specifies the offset of the view where the text will be rendered.
        This will be added to the 'textPosition' when a text is drawn.
        This affects all 'Text' methods. By default { 0, 0 }.
        */
        Math::Point2f viewOffset;
        /**
        Specifies the current text position. This will be incremented when new text is drawn.
        'viewSize' determines when the Y axis of this position will be incremented and the X axis will be reset to 'textBorder.x'.
        By default same as 'textBorder'.
        */
        Math::Point2f textPosition;
        /**
        Specifies the text border. This is used as initial text position.
        This is also used when the text leaves the view area and must be reset to the left side.
        By default { 10, 10 }.
        */
        Math::Size2f textBorder { 10, 10 };
        /**
        Minimal text height. This will be used when an empty new line will be added. By default 10.
        \see NewLine
        */
        int minTextHeight = 10;

    private:
        
        /* === Structures === */

        struct TextField
        {
            const TextureFont*  font;
            Math::Point2f       position;
            std::wstring        text;
            ColorRGBAf          color;
        };

        /* === Functions === */

        void AddTextField(const TextField& textField);
        void AddText(const std::wstring& text, float textWidth);

        std::wstring TrimTextToFit(const std::wstring& str, bool allowCrop) const;

        bool IsNewLineRequired(float width) const;

        /* === Members === */

        PrimitiveRendererPtr                        renderer_;

        std::stack<TextureFontPtr>                  fontStack_;
        std::map<Font::Description, TextureFontPtr> fontMap_;

        std::stack<ColorRGBAf>                      colorStack_;

        int                                         textFieldMaxHeight_ = 0;
        std::vector<TextField>                      textFieldBuffer_;

        StringSeparatorFilter                       separatorFilter_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================