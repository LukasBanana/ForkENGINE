/*
 * Bitwise color file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Core/BitwiseColor.h"


namespace Fork
{

namespace Video
{


/* === Internal functions === */

static inline void ConcatAndShiftColor(unsigned int& bits, unsigned char color)
{
    bits |= color;
    bits <<= 8;
}

static inline void ConcatColor(unsigned int& bits, unsigned char color)
{
    bits |= color;
}


/* --- ARGB functions --- */

FORK_EXPORT unsigned int ColorToUInt32ARGB(const ColorRGBAub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, color.a );
    ConcatAndShiftColor( bits, color.r );
    ConcatAndShiftColor( bits, color.g );
    ConcatColor        ( bits, color.b );

    return bits;
}

FORK_EXPORT unsigned int ColorToUInt32XRGB(const ColorRGBub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, 255     );
    ConcatAndShiftColor( bits, color.r );
    ConcatAndShiftColor( bits, color.g );
    ConcatColor        ( bits, color.b );

    return bits;
}

/* --- ABGR functions --- */

FORK_EXPORT unsigned int ColorToUInt32ABGR(const ColorRGBAub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, color.a );
    ConcatAndShiftColor( bits, color.b );
    ConcatAndShiftColor( bits, color.g );
    ConcatColor        ( bits, color.r );

    return bits;
}

FORK_EXPORT unsigned int ColorToUInt32XBGR(const ColorRGBub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, 255     );
    ConcatAndShiftColor( bits, color.b );
    ConcatAndShiftColor( bits, color.g );
    ConcatColor        ( bits, color.r );

    return bits;
}

/* --- RGBA functions --- */

FORK_EXPORT unsigned int ColorToUInt32RGBA(const ColorRGBAub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, color.r );
    ConcatAndShiftColor( bits, color.g );
    ConcatAndShiftColor( bits, color.b );
    ConcatColor        ( bits, color.a );

    return bits;
}

FORK_EXPORT unsigned int ColorToUInt32RGBX(const ColorRGBub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, color.r );
    ConcatAndShiftColor( bits, color.g );
    ConcatAndShiftColor( bits, color.b );
    ConcatColor        ( bits, 255     );

    return bits;
}

/* --- BGRA functions --- */

FORK_EXPORT unsigned int ColorToUInt32BGRA(const ColorRGBAub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, color.b );
    ConcatAndShiftColor( bits, color.g );
    ConcatAndShiftColor( bits, color.r );
    ConcatColor        ( bits, color.a );

    return bits;
}

FORK_EXPORT unsigned int ColorToUInt32BGRX(const ColorRGBub& color)
{
    unsigned int bits = 0;

    ConcatAndShiftColor( bits, color.b );
    ConcatAndShiftColor( bits, color.g );
    ConcatAndShiftColor( bits, color.r );
    ConcatColor        ( bits, 255     );

    return bits;
}


} // /namespace Video

} // /namespace Fork



// ========================