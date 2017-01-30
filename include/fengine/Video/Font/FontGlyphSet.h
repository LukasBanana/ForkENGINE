/*
 * Font glyph set header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FONT_GLYPH_SET_H__
#define __FORK_FONT_GLYPH_SET_H__


#include "Video/Font/FontGlyph.h"


namespace Fork
{

namespace Video
{


//! Set of font glyphs.
struct FORK_EXPORT FontGlyphSet
{
    /**
    Returns a reference to the specified glyph.
    \throws IndexOutOfBoundsException If the index is out of bounds.
    */
    FontGlyph& operator [] (char chr);
    /**
    Returns a constant reference to the specified glyph.
    \throws IndexOutOfBoundsException If the index is out of bounds.
    */
    const FontGlyph& operator [] (char chr) const;

    /**
    Returns a reference to the specified glyph.
    \throws IndexOutOfBoundsException If the index is out of bounds.
    */
    FontGlyph& operator [] (wchar_t wchr);
    /**
    Returns a constant reference to the specified glyph.
    \throws IndexOutOfBoundsException If the index is out of bounds.
    */
    const FontGlyph& operator [] (wchar_t wchr) const;

    //! Returns true if this glyph set contains the specified character.
    inline bool Contains(char chr) const
    {
        return unsigned char(chr) >= firstGlyph && unsigned char(chr) <= lastGlyph;
    }
    //! Returns true if this glyph set contains the specified wide character.
    inline bool Contains(wchar_t wchr) const
    {
        return wchr >= firstGlyph && wchr <= lastGlyph;
    }

    wchar_t                 firstGlyph  = 0x00; //!< Index of the first glyph used in the glyph set.
    wchar_t                 lastGlyph   = 0xff; //!< Index of the last glyph used in the glyph set.

    std::vector<FontGlyph>  glyphs;             //!< List of glyphs. If 'firstGlyph' is 32 and 'lastGlyph' is 200, then 'glyphs' has 196 elements.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================