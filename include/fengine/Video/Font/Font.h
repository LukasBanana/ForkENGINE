/*
 * Font header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FONT_H__
#define __FORK_FONT_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Font/Font.h"
#include "Video/Font/FontGlyphSet.h"
#include "Math/Core/Size2.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(Font);

//! Font base class.
class FORK_EXPORT Font
{
    
    public:

        /* === Enumerations === */
        
        //! Font types
        enum class Types
        {
            Texture, //!< Texture font. \see TextureFont
        };

        //! Font description flags.
        struct Flags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                Bold        = (1 << 0),
                Italic      = (1 << 1),
                Underlined  = (1 << 2),
                StrikeOut   = (1 << 3),
                Symbols     = (1 << 4),
            };
        };

        /* === Structures === */

        //! Font description structure. Contains name, size, flags and glyph set range.
        struct FORK_EXPORT Description
        {
            Description() = default;
            Description(const std::wstring& fontNameUTF16, int fontHeight, const Flags::DataType fontFlags = 0);
            Description(const std::wstring& fontNameUTF16, const Math::Size2i& fontSize, const Flags::DataType fontFlags = 0);
            Description(const std::string& fontNameUTF8, int fontHeight, const Flags::DataType fontFlags = 0);
            Description(const std::string& fontNameUTF8, const Math::Size2i& fontSize, const Flags::DataType fontFlags = 0);

            //! Returns a description string, e.g. "\"Times New Roman\", 32 px, Bold, Italic".
            std::string Desc() const;

            std::wstring    name;               //!< Font name. This is always stored as UTF-16 string.
            Math::Size2i    size;               //!< Font size. It is sufficient to set the height only.
            Flags::DataType flags       = 0;    //!< Font type flags. \see Font::Flags

            /**
            Specifies the first index of the glyph range.
            By default 0x0020 (to ignore special characters like the new-line chatacter '\n' etc.).
            */
            wchar_t         firstGlyph  = 0x0020;
            /**
            Specifies the last index of the glyph range. By default 0x00ff.
            If you want to use the unicode, increase this value.
            \note You should never create a texture font with the entire unicode,
            because the texture will be too large and the generation process can take several minutes!
            */
            wchar_t         lastGlyph   = 0x00ff;
        };

        /* === Functions === */

        Font(const Description& desc, const FontGlyphSet& glyphSet);
        virtual ~Font();

        //! Returns the font type.
        virtual Types Type() const = 0;

        //! Returns the width of the specified UTF8 text.
        int TextWidth(const std::string& textUTF8) const;
        //! Returns the width of the specified UTF16 text.
        int TextWidth(const std::wstring& textUTF16) const;

        //! Returns the width of the specified UTF8 sub text.
        int TextWidth(const std::string& textUTF8, size_t startPos, size_t len = std::string::npos) const;
        //! Returns the width of the specified UTF16 sub text.
        int TextWidth(const std::wstring& textUTF16, size_t startPos, size_t len = std::string::npos) const;

        //! Returns the size of the specified UTF8 text.
        Math::Size2i TextSize(const std::string& textUTF8) const;
        //! Returns the size of the specified UTF16 text.
        Math::Size2i TextSize(const std::wstring& textUTF16) const;

        //! Returns the font description.
        inline const Description& GetDesc() const
        {
            return desc_;
        }

        //! Returns the glyph set.
        inline const FontGlyphSet& GetGlyphSet() const
        {
            return glyphSet_;
        }

        //! Returns the font's height. This is equivalent to "GetDesc().size.height".
        inline int FontHeight() const
        {
            return desc_.size.height;
        }

    private:
        
        Description     desc_;
        FontGlyphSet    glyphSet_;

};


//! Compares the two font descriptions. This is used for sorting methods.
FORK_EXPORT bool operator < (const Font::Description& lhs, const Font::Description& rhs);


} // /namespace Video

} // /namespace Fork


#endif



// ========================