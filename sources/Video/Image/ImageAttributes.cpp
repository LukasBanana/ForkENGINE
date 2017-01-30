/*
 * Image attributes file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Image/ImageAttributes.h"


namespace Fork
{

namespace Video
{


FORK_EXPORT size_t NumColorComponents(const ImageColorFormats format)
{
    switch (format)
    {
        case ImageColorFormats::Gray:
            return 1;
        case ImageColorFormats::GrayAlpha:
            return 2;
        case ImageColorFormats::RGB:
        case ImageColorFormats::BGR:
            return 3;
        case ImageColorFormats::RGBA:
        case ImageColorFormats::BGRA:
        case ImageColorFormats::DepthComponent:
        case ImageColorFormats::DepthStencil:
            return 4;
    }
    return 0;
}

FORK_EXPORT bool HasAlphaChannel(const ImageColorFormats format)
{
    return format == ImageColorFormats::RGBA || format == ImageColorFormats::BGRA;
}

FORK_EXPORT ImageColorFormats ChooseImageFormat(unsigned int numColorComponents)
{
    switch (numColorComponents)
    {
        case 2:
            return ImageColorFormats::GrayAlpha;
        case 3:
            return ImageColorFormats::RGB;
        case 4:
            return ImageColorFormats::RGBA;
        default:
            break;
    }
    return ImageColorFormats::Gray;
}


} // /namespace Video

} // /namespace Fork



// ========================