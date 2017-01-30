/*
 * Font glyph header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FONT_GLYPH_H__
#define __FORK_FONT_GLYPH_H__


#include "Math/Core/Rect.h"
#include "Core/Export.h"

#include <vector>


namespace Fork
{

namespace Video
{


//! Font glyph structure.
struct FORK_EXPORT FontGlyph
{
    FontGlyph() = default;
    FontGlyph(const Math::Recti& glyphRect);
    FontGlyph(const Math::Recti& glyphRect, int glyphStartOffset, int glyphDrawnWidth, int glyphWhiteSpace);

    //! Returns the number of font glyphs for UTF8 font. This is 0xff.
    static inline size_t NumGlyphsUTF8()
    {
        return 0xff;
    }
    //! Returns the number of font glyphs for UTF16 font. This is 0xffff.
    static inline size_t NumGlyphsUTF16()
    {
        return 0xffff;
    }

    //! Returns the total width of the font glyph.
    inline int TotalWidth() const
    {
        return startOffset + drawnWidth + whiteSpace;
    }

    Math::Recti rect;
    int startOffset = 0;
    int drawnWidth = 0;
    int whiteSpace = 0;
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================