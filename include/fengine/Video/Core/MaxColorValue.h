/*
 * Maximal color value header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MAX_COLOR_VALUE_H__
#define __FORK_MAX_COLOR_VALUE_H__


namespace Fork
{

namespace Video
{


//! Returns the maximal color value for the data type T. By default 1.
template <typename T> inline T MaxColorValue()
{
    return T(1);
}

//! Specialized version. For unsigned 8-bit integers, the return value is 255.
template <> inline unsigned char MaxColorValue<unsigned char>()
{
    return 255;
}

//! Specialized version. For boolean, the return value is true.
template <> inline bool MaxColorValue<bool>()
{
    return true;
}

/**
Specialized version. For signed 32-bit integers, the return value is 255.
\todo Maybe this must be a different value than 255.
*/
template <> inline int MaxColorValue<int>()
{
    return 255; //???
}

/**
Returns the inverse color.
\tparam C Specifies the color type. This should be ColorRGB or ColorRGBA.
\tparam T Specifies the data type. This should be unsigned char, int, float or double.
\param[in] color Specifies the color which is to be inverted.
\return The inverse color of the input color.
\code
InvColor(Video::ColorRGBf(1, 0.7, 0)) == { 0, 0.3, 1 };
InvColor(Video::ColorRGBAub(255, 230, 50, 255)) == { 0, 25, 205, 0 };
\endcode
\see MaxColorValue
*/
template <template <typename> class C, typename T> C<T> InvColor(const C<T>& color)
{
    C<T> invColor;

    for (size_t i = 0; i < C<T>::num; ++i)
        invColor[i] = MaxColorValue<T>() - color[i];

    return invColor;
}


} // /namespace Video

} // /namespace Fork


#endif



// ========================