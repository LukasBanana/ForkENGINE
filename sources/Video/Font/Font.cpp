/*
 * Font file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Font/Font.h"
#include "Core/StringModifier.h"
#include "Core/AuxiliaryMacros.h"


namespace Fork
{

namespace Video
{


/*
 * Description structure
 */

Font::Description::Description(
    const std::wstring& fontNameUTF16, int fontHeight, const Flags::DataType fontFlags) :
        name    { fontNameUTF16               },
        size    { Math::Size2i(0, fontHeight) },
        flags   { fontFlags                   }
{
}
Font::Description::Description(
    const std::wstring& fontNameUTF16, const Math::Size2i& fontSize, const Flags::DataType fontFlags) :
        name    { fontNameUTF16 },
        size    { fontSize      },
        flags   { fontFlags     }
{
}
Font::Description::Description(
    const std::string& fontNameUTF8, int fontHeight, const Flags::DataType fontFlags) :
        name    { UTF8toUTF16(fontNameUTF8)   },
        size    { Math::Size2i(0, fontHeight) },
        flags   { fontFlags                   }
{
}
Font::Description::Description(
    const std::string& fontNameUTF8, const Math::Size2i& fontSize, const Flags::DataType fontFlags) :
        name    { UTF8toUTF16(fontNameUTF8) },
        size    { fontSize                  },
        flags   { fontFlags                 }
{
}

std::string Font::Description::Desc() const
{
    std::string desc;

    /* Add font name */
    desc += '\"';
    desc += ToStr(name);
    desc += '\"';

    /* Add font size */
    desc += ", ";
    desc += ToStr(size.height);
    desc += "px";

    /* Add flags */
    auto AddFlag = [&](const Flags::DataType flag, const std::string& ident)
    {
        if ((flags & flag) != 0)
        {
            desc += ", ";
            desc += ident;
        }
    };

    AddFlag( Flags::Bold,       "Bold"       );
    AddFlag( Flags::Italic,     "Italic"     );
    AddFlag( Flags::Underlined, "Underlined" );
    AddFlag( Flags::StrikeOut,  "StrikeOut"  );
    AddFlag( Flags::Symbols,    "Symbols"    );

    return desc;
}

FORK_EXPORT bool operator < (const Font::Description& lhs, const Font::Description& rhs)
{
    RETURN_COMPARE_SWO( lhs.name,        rhs.name        );
    RETURN_COMPARE_SWO( lhs.size.width,  rhs.size.width  );
    RETURN_COMPARE_SWO( lhs.size.height, rhs.size.height );
    RETURN_COMPARE_SWO( lhs.flags,       rhs.flags       );
    RETURN_COMPARE_SWO( lhs.firstGlyph,  rhs.firstGlyph  );
    RETURN_COMPARE_SWO( lhs.lastGlyph,   rhs.lastGlyph   );
    return false;
}


/*
 * Font class
 */

Font::Font(const Description& desc, const FontGlyphSet& glyphSet) :
    desc_       { desc     },
    glyphSet_   { glyphSet }
{
}
Font::~Font()
{
}

int Font::TextWidth(const std::string& textUTF8) const
{
    int size = 0;

    for (auto chr : textUTF8)
        size += glyphSet_[chr].TotalWidth();

    return size;
}

int Font::TextWidth(const std::wstring& textUTF16) const
{
    int size = 0;

    for (auto chr : textUTF16)
        size += glyphSet_[chr].TotalWidth();

    return size;
}

int Font::TextWidth(const std::string& textUTF8, size_t startPos, size_t len) const
{
    int size = 0;

    len += startPos;
    for (size_t num = std::min(len, textUTF8.size()); startPos < num; ++startPos)
    {
        auto chr = textUTF8[startPos];
        size += glyphSet_[chr].TotalWidth();
    }

    return size;
}

int Font::TextWidth(const std::wstring& textUTF16, size_t startPos, size_t len) const
{
    int size = 0;

    len += startPos;
    for (size_t num = std::min(len, textUTF16.size()); startPos < num; ++startPos)
    {
        auto chr = textUTF16[startPos];
        size += glyphSet_[chr].TotalWidth();
    }

    return size;
}

Math::Size2i Font::TextSize(const std::string& textUTF8) const
{
    return { TextWidth(textUTF8), desc_.size.height };
}

Math::Size2i Font::TextSize(const std::wstring& textUTF16) const
{
    return { TextWidth(textUTF16), desc_.size.height };
}


} // /namespace Video

} // /namespace Fork



// ========================