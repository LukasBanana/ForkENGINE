/*
 * Font glyph set file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Font/FontGlyphSet.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Core/StaticConfig.h"


namespace Fork
{

namespace Video
{


FontGlyph& FontGlyphSet::operator [] (char chr)
{
    #ifdef FORK_DEBUG
    if (!Contains(chr))
        throw IndexOutOfBoundsException(__FUNCTION__, chr);
    #endif
    return glyphs[unsigned char(chr) - firstGlyph];
}

const FontGlyph& FontGlyphSet::operator [] (char chr) const
{
    #ifdef FORK_DEBUG
    if (!Contains(chr))
        throw IndexOutOfBoundsException(__FUNCTION__, chr);
    #endif
    return glyphs[unsigned char(chr) - firstGlyph];
}

FontGlyph& FontGlyphSet::operator [] (wchar_t wchr)
{
    #ifdef FORK_DEBUG
    if (!Contains(wchr))
        throw IndexOutOfBoundsException(__FUNCTION__, wchr);
    #endif
    return glyphs[wchr - firstGlyph];
}

const FontGlyph& FontGlyphSet::operator [] (wchar_t wchr) const
{
    #ifdef FORK_DEBUG
    if (!Contains(wchr))
        throw IndexOutOfBoundsException(__FUNCTION__, wchr);
    #endif
    return glyphs[wchr - firstGlyph];
}


} // /namespace Video

} // /namespace Fork



// ========================