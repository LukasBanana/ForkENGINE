/*
 * Image converter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_CONVERTER_H__
#define __FORK_IMAGE_CONVERTER_H__


#include "Video/Image/ImageConversionException.h"
#include "Video/Image/ImageAttributes.h"
#include "Video/Core/AvgColorValue.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Core/Exception/InvalidStateException.h"
#include "Math/Core/BaseMath.h"
#include "Math/Core/Size3.h"
#include "Core/StaticConfig.h"

#include <memory>
#include <algorithm>


namespace Fork
{

namespace Video
{

namespace ImageConverter
{


//! Size type for the image converter. This is Math::Size3st (or rather Math::Size3<size_t>).
typedef Math::Size3st SizeType;

/* --- Template declarations --- */

/**
Inverts the colors.
\tparam invVal Specifies the inversion value. For floating-point data types, this should be 1.
For an unsigned 8-bit integer data type, this should be 255.
\tparam It Specifies the iterator type to a color component. This could be float* or vector<float>::iterator.
\param[in,out] begin Iterator to the first color element.
\param[in,out] end Iterator after the last color element.
\see Image::PixelIterator
\note The two pointers work like iterators!
*/
template <int invVal, typename It> void InvertColors(It begin, It end);

/**
Flips the color values from RGB to BGR or vice versa.
\tparam It Specifies the iterator type to an RGB or RGBA color. This type must have the dereferencing operator '->'
which returns a type which has the members 'r' and 'b'. Here is an example list of possible iterator types:
\code
std::vector<ColorRGB<T>>::iterator // With arbitrary type T
std::list<ColorRGBA<T>>::iterator // With arbitrary type T
Video::Image<T>::PixelIterator // With arbitrary type T
ColorRGB<T>* // With arbitrary type T
ColorRGBA<T>* // With arbitrary type T
\endcode
\note The colors of the modified container must have at least three components!
*/
template <typename It> void FlipColorsRGBtoBGR(It begin, It end);

/**
Flips the image data on the x-axis
\throws ImageConversionException if imageBuffer is null or components is less than 1 or greater than 4.
*/
template <typename T> void FlipImageX(T* imageBuffer, const SizeType& size, size_t components);

/**
Flips the image data on the y-axis
\see FlipImageX
*/
template <typename T> void FlipImageY(T* imageBuffer, const SizeType& size, size_t components);

/**
Flips the image data on the z-axis
\see FlipImageX
*/
template <typename T> void FlipImageZ(T* imageBuffer, const SizeType& size, size_t components);

/**
Copies the source image in a scaled form to the destination image
without interpolation (nearest pixel is used).
*/
template <typename T> void ResizeImageNearest(
    const T* srcImageBuffer, const SizeType& srcSize,
    T* destImageBuffer, const SizeType& destSize,
    size_t components
);

/**
Copies the source image in a scaled form to the destination image
with linear interpolation (interpolate between the surrounding pixels).
*/
template <typename T> void ResizeImageLinear(
    const T* srcImageBuffer, const SizeType& srcSize,
    T* destImageBuffer, const SizeType& destSize,
    size_t components
);

/**
Converts the color format of the source image buffer into the color format for the destination image buffer.
\param[in] srcBuffer Raw pointer to the source image buffer. This must not be null!
\param[in] srcFormat Specifies the color format of the source buffer.
\param[in,out] destBuffer Raw pointer to the destination image buffer. This must not be null!
This buffer must already has the correct size (depending on the destination color format).
\param[in] destFormat Specifies the color format of the destination buffer.
\param[in] numPixels Specifies the number of pixels to convert. This should cover the entire image buffer.
\param[in] defaultAlpha Specifies an optional default alpha channel
if the source buffer has no alpha channel. By default MaxColorValue<T>().
\remarks Valid values for 'srcFormat' and 'destFormat' are: Gray, GrayAlpha, RGG, BGR, RGBA, BGRA.
DepthComponent and DepthStencil are no value values for this function!
\remarks Usage example:
\code
static const size_t w = 2;
static const size_t h = 2;
// Allocate buffers
unsigned char srcBuffer[w*h*3] =
{
    255,200,130,    20,255, 64,
      0,  0,  0,   100,  0,160
};
unsigned char destBuffer[w*h*4] = { 0 };
// Convert image format
Video::ConvertImageFormat<unsigned char>(
    srcBuffer, Video::ImageColorFormats::RGB,
    destBuffer, Video::ImageColorFormats::BGRA,
    w*h
);
// Resulting buffer will be:
// destBuffer ==
// {
//     130,200,255,255,    64,255, 20,255,
//       0,  0,  0,255,   160,  0,100,255
// }
\endcode
\throws NullPointerException If 'srcBuffer' or 'destBuffer' is null.
\throws InvalidArgumentException If 'srcFormat' or 'destFormat' is invalid (see remarks).
\see MaxColorValue
*/
template <typename T> void ConvertImageFormat(
    const T* srcBuffer, const ImageColorFormats srcFormat,
    T* destBuffer, const ImageColorFormats destFormat,
    size_t numPixels, const T& defaultAlpha = MaxColorValue<T>()
);

#if 0

//! Scales the image to a new size
template <typename T> void scaleImage(T* &ImageBuffer, int Width, int Height, int NewWidth, int NewHeight, int FormatSize);

//! Scales the image to a half size (smooth)
template <typename T> void halveImage(T* &ImageBuffer, int Width, int Height, int FormatSize);

//! Converts the image data formats (e.g. RGB -> RGBA)
template <typename T, int DefVal> void convertImageFormat(T* &ImageBuffer, int Width, int Height, int OldFormatSize, int NewFormatSize);

//! Blurs the image data (for mipmap levels)
template <typename T> void blurImage(T* &ImageBuffer, int Width, int Height, int FormatSize);

//! Turns the image data in 90°, 180° or 270°
template <typename T> void turnImage(T* ImageBuffer, int Width, int Height, int FormatSize, const EImageTurnDegrees Degree);

/**
\tparam T Specifies the image buffer data type. Usually unsigned byte (or rather 'u8') or 32-bit floating point (or rather 'f32').
\tparam DefVal Specifies the default pixel component value. For unsigned byte pixel format use 255 and for floating point values use 1.
Or use zero whenever you want to convert the pixels (not the alpha channel) to black.
Converts the gray values from the specified image buffer into alpha values.
After this conversion, the RGB components of each pixel have the value specified by 'DefVal'
and only the alpha channel contains information about the image (which were previously the gray values).
\param[in,out] ImageBuffer Pointer to the image buffer which is to be modified.
The image buffer must contain (Width x Height x 4) elements, i.e. it must have an RGBA format.
\param[in] Width Specifies the image buffer width.
\param[in] Height Specifies the image buffer height.
*/
template <typename T, int DefVal> void convertImageGrayToAlpha(T* ImageBuffer, int Width, int Height);

/**
Appends the given image buffer to the bottom of the base image buffer. If the new frame's size does not fit
it will be scaled and the pixel format will be adjusted as well.
\param[in,out] ImageBuffer Pointer to the base image buffer. This buffer will be re-allocated.
\param[in] AdditionalBuffer Constant pointer to the image buffer which is to be appended.
\param[in] Width Width of the base image buffer.
\param[in] Height Height of the base image buffer.
\param[in] FrameWidth Width of the frame image buffer.
\param[in] FrameHeight Height of the frame image buffer.
\param[in] FormatSize Format size of the base image buffer (1, 2, 3 or 4).
\return True on success. Otherwise false when the arguments are invalid.
\todo This function has not been tested yet!
*/
template <typename T> bool appendImageBufferBottom(
    T* &ImageBuffer, const T* AdditionalBuffer, int Width, int Height, int FrameWidth, int FrameHeight, int FormatSize
);

/**
Appends the given image buffer to the bottom of the base image buffer. If the new frame's size does not fit
it will be scaled and the pixel format will be adjusted as well. For more information look at "appendImageBufferBottom".
\see appendImageBufferBottom
\todo Not implemented yet!
*/
template <typename T> bool appendImageBufferRight(
    T* &ImageBuffer, const T* AdditionalBuffer, int Width, int Height, int FrameWidth, int FrameHeight, int FormatSize
);

/**
Sets the color key of the given image buffer. The format size must be 4.
\param[in,out] ImageBuffer Pointer to the image buffer which is to be modified.
\param[in] Width Width of the image buffer.
\param[in] Height Height of the image buffer.
\param[in] Color Specifies the color key. Each pixel with the same RGB color components will get the new alpha channel (Color.Alpha).
\param[in] Tolerance Specifies the tolerance factor. Each pixel will get the new alpha channel when the RGB color components
are in the same range (if PixelColor = ColorKey +/- Tolerance). By default 0.
*/
FORK_EXPORT void setImageColorKey(u8* ImageBuffer, int Width, int Height, const video::color &Color, int Tolerance = 0);

//! Get the correct image size (2^x)
FORK_EXPORT bool checkImageSize(dim::size2di &InputSize);

//! Get the count of mipmap levels
FORK_EXPORT int getMipmapLevelsCount(int Width, int Height);

#endif


/* --- Template defintions --- */

template <int invVal, typename It> void InvertColors(It begin, It end)
{
    if (begin && end)
    {
        while (begin != end)
        {
            *begin = Math::Inv<invVal>(*begin);
            ++begin;
        }
    }
}

template <typename It> void FlipColorsRGBtoBGR(It begin, It end)
{
    if (begin && end)
    {
        while (begin != end)
        {
            std::swap(begin->r, begin->b);
            ++begin;
        }
    }
}

template <typename T> void FlipImageX(T* imageBuffer, const SizeType& size, size_t components)
{
    /* Check parameter validity */
    ASSERT_POINTER(imageBuffer);

    if (components < 1 || components > 4)
        throw ImageConversionException(__FUNCTION__, size, components);

    /* Store stride size */
    const size_t pitchWidth = size.width * components;

    /* Iterate over each scanline and slice */
    for (size_t y = 0; y < size.height*size.depth; ++y)
    {
        /* Iterate over each color */
        for (size_t x = 0; x < size.width/2; ++x)
        {
            auto xPitch = x*components;

            /* Iterate over each color component in the current color */
            for (int i = 0; i < components; ++i)
            {
                /*
                Compute indices where the colors will be swaped:
                - Get index from the right side (indexA).
                - Get index from the left side (indexB).
                - Swap the two color components.
                */
                auto indexA = ( pitchWidth - xPitch - 1 ) - i;
                auto indexB = xPitch + (components - i - 1);

                std::swap(imageBuffer[indexA], imageBuffer[indexB]);
            }
        }

        /* Step to the next scanline */
        imageBuffer += pitchWidth;
    }
}

template <typename T> void FlipImageY(T* imageBuffer, const SizeType& size, size_t components)
{
    /* Check parameter validity */
    ASSERT_POINTER(imageBuffer);

    if (components < 1 || components > 4)
        throw ImageConversionException(__FUNCTION__, size, components);

    /* Store stride sizes */
    const size_t pitchWidth = size.width*components;
    const size_t pitchHeight = pitchWidth*size.height;
    
    /* Iterate over each slice */
    for (size_t z = 0; z < size.depth; ++z)
    {
        /* Iterate over each scanline */
        for (size_t y = 0; y < size.height/2; ++y)
        {
            /* Iterate over each color component in the current scanline */
            for (size_t i = 0; i < pitchWidth; ++i)
            {
                /*
                Compute indices where the colors will be swaped:
                - Get index from the bottom (indexA).
                - Get index from the top (indexB).
                - Swap the two color components.
                */
                auto indexA = (size.height - y - 1)*pitchWidth + i;
                auto indexB = y*pitchWidth + i;

                std::swap(imageBuffer[indexA], imageBuffer[indexB]);
            }
        }

        /* Step to the next slice */
        imageBuffer += pitchHeight;
    }
}

template <typename T> void FlipImageZ(T* imageBuffer, const SizeType& size, size_t components)
{
    //...
}

template <typename T> void ResizeImageNearest(
    const T* srcImageBuffer, const SizeType& srcSize,
    T* destImageBuffer, const SizeType& destSize,
    size_t components)
{
    /* Check parameter validity */
    ASSERT_POINTER(srcImageBuffer);
    ASSERT_POINTER(destImageBuffer);

    if (components < 1 || components > 4)
        throw ImageConversionException(__FUNCTION__, srcSize, components);
    
    /* Temporary indices */
    size_t indexDst = 0;

    /* Iterate over each slice */
    for (size_t z = 0; z < destSize.depth; ++z)
    {
        /* Compute depth index pitch */
        const size_t pitchSrcDepth = ( z * srcSize.depth / destSize.depth ) * srcSize.height * srcSize.width;

        /* Iterate over each scanline */
        for (size_t y = 0; y < destSize.height; ++y)
        {
            /* Compute height index pitch */
            const size_t pitchSrcHeight = ( y * srcSize.height / destSize.height ) * srcSize.width;

            /* Iterate over each color */
            for (size_t x = 0; x < destSize.width; ++x)
            {
                /* Compute width index pitch and final source index */
                const size_t pitchSrcWidth = x * srcSize.width / destSize.width;
                const size_t indexSrc = ( pitchSrcDepth + pitchSrcHeight + pitchSrcWidth ) * components;
                
                /* Iterate over each color component */
                for (size_t c = 0; c < components; ++c)
                {
                    /* Copy pixel data */
                    destImageBuffer[indexDst] = srcImageBuffer[indexSrc + c];
                    ++indexDst;
                }
            }
        }
    }
}

template <typename T> void ResizeImageLinear(
    const T* srcImageBuffer, const SizeType& srcSize,
    T* destImageBuffer, const SizeType& destSize,
    size_t components)
{
    //...
}

template <typename T> void ConvertImageFormat(
    const T* srcBuffer, const ImageColorFormats srcFormat,
    T* destBuffer, const ImageColorFormats destFormat,
    size_t numPixels, const T& defaultAlpha)
{
    /* Validate parameters */
    ASSERT_POINTER(srcBuffer);
    ASSERT_POINTER(destBuffer);

    if (srcFormat < ImageColorFormats::Gray || srcFormat > ImageColorFormats::BGRA)
        throw InvalidArgumentException(__FUNCTION__, "srcFormat", "Invalid source color format for image buffer conversion");
    if (destFormat < ImageColorFormats::Gray || destFormat > ImageColorFormats::BGRA)
        throw InvalidArgumentException(__FUNCTION__, "destFormat", "Invalid destination color format for image buffer conversion");

    if (!numPixels)
        return;

    /* Determine conversion meta data */
    const int rgbOrderSrc   = ( srcFormat  == ImageColorFormats::BGR || srcFormat  == ImageColorFormats::BGRA ? 1 : 0 );
    const int rgbOrderDest  = ( destFormat == ImageColorFormats::BGR || destFormat == ImageColorFormats::BGRA ? 1 : 0 );

    const auto compSrc      = NumColorComponents( srcFormat  );
    const auto compDest     = NumColorComponents( destFormat );

    const bool swapRGB      = ((rgbOrderSrc ^ rgbOrderDest) != 0 && compDest >= 3);

    #ifdef FORK_DEBUG
    if (compSrc < 1 || compSrc > 4 || compDest < 1 || compDest > 4)
    {
        throw InvalidStateException(
            __FUNCTION__,
            "A color format has invalid number of components (Must be 1, 2, 3 or 4)"
        );
    }
    #endif

    /* Check if memory must only be copied */
    if (compSrc == compDest)
    {
        /* Only copy image buffer from source to destination */
        std::copy(srcBuffer, srcBuffer + numPixels * compSrc, destBuffer);
    }
    else
    {
        /* Use temporary pointers for iteration */
        auto src = srcBuffer;
        auto dst = destBuffer;

        /* Choose destination color components */
        switch (compDest)
        {
            case 1: // gray
                for (size_t i = 0; i < numPixels; ++i)
                {
                    switch (compSrc)
                    {
                        case 2: // gray-alpha
                            dst[0] = src[0];
                            break;
                        case 3: // rgb/bgr
                        case 4: // rgba/bgra
                            dst[0] = AvgColorValue(ColorRGB<T>(src[0], src[1], src[2]));
                            break;
                    }
                    src += compSrc;
                    dst += compDest;
                }
                break;

            case 2: // gray-alpha
                for (size_t i = 0; i < numPixels; ++i)
                {
                    switch (compSrc)
                    {
                        case 1: // gray
                            dst[0] = src[0];
                            dst[1] = defaultAlpha;
                            break;
                        case 3: // rgb/bgr
                            dst[0] = AvgColorValue(ColorRGB<T>(src[0], src[1], src[2]));
                            dst[1] = defaultAlpha;
                            break;
                        case 4: // rgba/bgra
                            dst[0] = AvgColorValue(ColorRGB<T>(src[0], src[1], src[2]));
                            dst[1] = src[3];
                            break;
                    }
                    src += compSrc;
                    dst += compDest;
                }
                break;

            case 3: // rgb/bgr
                for (size_t i = 0; i < numPixels; ++i)
                {
                    switch (compSrc)
                    {
                        case 1: // gray
                        case 2: // gray-alpha
                            dst[0] = src[0];
                            dst[1] = src[0];
                            dst[2] = src[0];
                            break;
                        case 4: // rgba/bgra
                            dst[0] = src[0];
                            dst[1] = src[1];
                            dst[2] = src[2];
                            break;
                    }
                    src += compSrc;
                    dst += compDest;
                }
                break;

            case 4: // rgba/bgra
                for (size_t i = 0; i < numPixels; ++i)
                {
                    switch (compSrc)
                    {
                        case 1: // gray
                            dst[0] = src[0];
                            dst[1] = src[0];
                            dst[2] = src[0];
                            dst[3] = defaultAlpha;
                            break;
                        case 2: // gray-alpha
                            dst[0] = src[0];
                            dst[1] = src[0];
                            dst[2] = src[0];
                            dst[3] = src[1];
                            break;
                        case 3: // rgb/bgr
                            dst[0] = src[0];
                            dst[1] = src[1];
                            dst[2] = src[2];
                            dst[3] = defaultAlpha;
                            break;
                    }
                    src += compSrc;
                    dst += compDest;
                }
                break;
        }
    }

    /* Check if additional color swapping is required */
    if (swapRGB)
    {
        /* Swap the destination buffer colors from RGB to BGR */
        for (size_t i = 0; i < numPixels; ++i)
        {
            std::swap(destBuffer[0], destBuffer[2]);
            destBuffer += compDest;
        }
    }
}


} // /namespace ImageConverter

} // /namespace Video

} // /namespace Fork


#endif



// ========================