/*
 * Image attributes header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_ATTRIBUTES_H__
#define __FORK_IMAGE_ATTRIBUTES_H__


#include "Core/Export.h"


namespace Fork
{

namespace Video
{


//! Image buffer color formats.
enum class ImageColorFormats
{
    Gray,           //!< Single color component: Gray or rather brightness.
    GrayAlpha,      //!< Two color components: Gray, Alpha.
    RGB,            //!< Three color components: Red, Green, Blue.
    BGR,            //!< Three color components: Blue, Green, Red.
    RGBA,           //!< Four color components: Red, Green, Blue, Alpha.
    BGRA,           //!< Four color components: Blue, Green, Red, Alpha.
    DepthComponent, //!< Single color component used as depth component.
    DepthStencil,   //!< Pair of depth and stencil component.
};

/**
Image resize modes.
\see Video::Image::Resize
*/
enum class ImageResizeModes
{
    None,       //!< No pixel interpolation will be computed for an image resize, i.e. the new image buffer will be uninitialized!
    Nearest,    //!< Nearest pixel will be choosen when the image is scaled. This is fast but looks poor.
    Linear,     //!< Linear interpolate surrounding pixels when the image is scaled. This is slow but looks better.
};


/**
Returns the number of color components of the specified image color format.
\see ImageColorFormats
*/
FORK_EXPORT size_t NumColorComponents(const ImageColorFormats format);

/**
Returns true if the specified image color format has an alpha channel.
This is only true for ImageColorFormats::RGBA and ImageColorFormats::BGRA.
\see ImageColorFormats
*/
FORK_EXPORT bool HasAlphaChannel(const ImageColorFormats format);

/**
Returns an image color format by the number of color components.
\param[in] numColorComponents Specifies the number of color components the choosen format should has.
This must be 1, 2, 3 or 4. If this parameter is out of range, the return value is always ImageColorFormats::Gray.
\return The choosen image color format.
\see ImageColorFormats
*/
FORK_EXPORT ImageColorFormats ChooseImageFormat(unsigned int numColorComponents);


} // /namespace Video

} // /namespace Fork


#endif



// ========================