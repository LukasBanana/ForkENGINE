/*
 * Bitwise color header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BITWISE_COLOR_H__
#define __FORK_BITWISE_COLOR_H__


#include "Core/Export.h"
#include "Video/Core/ColorRGB.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{


/**
Converts the specified color into a 32-bit unsigned integer of the form 0xAARRGGBB,
where AA, RR, GG and BB are the alpha, red, green and blue components.
\code
// -----------------------------------------
// |  Alpha  |   Red   |  Green  |  Blue   |
// |---------|---------|---------|---------|
// | 31...24 | 23...16 | 15...08 | 07...00 |
// -----------------------------------------
\endcode
*/
FORK_EXPORT unsigned int ColorToUInt32ARGB(const ColorRGBAub& color);
//! \see ColorToUInt32ARGB
FORK_EXPORT unsigned int ColorToUInt32XRGB(const ColorRGBub& color);

/**
Converts the specified color into a 32-bit unsigned integer of the form 0xAABBGGRR,
where AA, BB, GG and RR are the alpha, blue, green and red components.
\code
// -----------------------------------------
// |  Alpha  |  Blue   |  Green  |   Red   |
// |---------|---------|---------|---------|
// | 31...24 | 23...16 | 15...08 | 07...00 |
// -----------------------------------------
\endcode
*/
FORK_EXPORT unsigned int ColorToUInt32ABGR(const ColorRGBAub& color);
//! \see ColorToUInt32ABGR
FORK_EXPORT unsigned int ColorToUInt32XBGR(const ColorRGBub& color);

/**
Converts the specified color into a 32-bit unsigned integer of the form 0xRRGGBBAA,
where RR, GG, BB and AA are the red, green, blue and alpha components.
\code
// -----------------------------------------
// |   Red   |  Green  |  Blue   |  Alpha  |
// |---------|---------|---------|---------|
// | 31...24 | 23...16 | 15...08 | 07...00 |
// -----------------------------------------
\endcode
*/
FORK_EXPORT unsigned int ColorToUInt32RGBA(const ColorRGBAub& color);
//! \see ColorToUInt32RGBA
FORK_EXPORT unsigned int ColorToUInt32RGBX(const ColorRGBub& color);

/**
Converts the specified color into a 32-bit unsigned integer of the form 0xBBGGRRAA,
where BB, GG, RR and AA are the blue, green, red and alpha components.
\code
// -----------------------------------------
// |   Red   |  Green  |  Blue   |  Alpha  |
// |---------|---------|---------|---------|
// | 31...24 | 23...16 | 15...08 | 07...00 |
// -----------------------------------------
\endcode
*/
FORK_EXPORT unsigned int ColorToUInt32BGRA(const ColorRGBAub& color);
//! \see ColorToUInt32BGRA
FORK_EXPORT unsigned int ColorToUInt32BGRX(const ColorRGBub& color);


/**
Returns the first (index 0) component from the specified 32-bit
color value (e.g. from an RGBA color this is the red component).
*/
inline unsigned int ExtractColor0(unsigned int color)
{
    return color >> 24;
}

/**
Returns the second (index 1) component from the specified 32-bit
color value (e.g. from an RGBA color this is the green component).
*/
inline unsigned int ExtractColor1(unsigned int color)
{
    return (color >> 16) & 0xff;
}

/**
Returns the third (index 1) component from the specified 32-bit
color value (e.g. from an RGBA color this is the blue component).
*/
inline unsigned int ExtractColor2(unsigned int color)
{
    return (color >> 8) & 0xff;
}

/**
Returns the forth (index 3) component from the specified 32-bit
color value (e.g. from an RGBA color this is the alpha component).
*/
inline unsigned int ExtractColor3(unsigned int color)
{
    return color & 0xff;
}


} // /namespace Video

} // /namespace Fork


#endif



// ========================