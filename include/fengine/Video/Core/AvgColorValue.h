/*
 * Average color value header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AVG_COLOR_VALUE_H__
#define __FORK_AVG_COLOR_VALUE_H__


namespace Fork
{

namespace Video
{


/**
Returns the average value of the specified color.
\tparam C Specifies the color type. This should be ColorRGB or ColorRGBA.
\tparam T Specifies the data type. This should be unsigned char, int, float or double.
\param[in] color Specifies the color whose average value is to be computed.
\return The average color value of the input color.
\code
AvgColorValue(Video::ColorRGBf(1, 0.8, 0)) == 0.6;
AvgColorValue(Video::ColorRGBAub(255, 230, 50, 255)) == 197;
\endcode
*/
template <template <typename> class C, typename T> T AvgColorValue(const C<T>& color)
{
    T avg = T(0);

    for (size_t i = 0; i < C<T>::num; ++i)
        avg += color[i];

    return avg / static_cast<T>(C<T>::num);
}

//! Specialized version. For unsigned 8-bit integers.
template <template <typename> class C> unsigned char AvgColorValue(const C<unsigned char>& color)
{
    int avg = 0;

    for (size_t i = 0; i < C<unsigned char>::num; ++i)
        avg += static_cast<int>(color[i]);

    return static_cast<unsigned char>(avg / C<unsigned char>::num);
}


} // /namespace Video

} // /namespace Fork


#endif



// ========================