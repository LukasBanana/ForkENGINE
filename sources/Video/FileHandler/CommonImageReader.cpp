/*
 * Common image reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommonImageReader.h"
#include "IO/Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../Plugins/STB/stb_image.h"


namespace Fork
{

namespace Video
{


static ImageColorFormats GetColorFormatByComponents(int components)
{
    switch (components)
    {
        case 1:
            return ImageColorFormats::Gray;
        case 2:
            return ImageColorFormats::GrayAlpha;
        case 3:
            return ImageColorFormats::RGB;
        case 4:
            return ImageColorFormats::RGBA;
    }
    return ImageColorFormats::Gray;
}

ImageUBytePtr CommonImageReader::ReadImage(const std::string& filename)
{
    /* Load image using the STBI library */
    int width = 0, height = 0, components = 0;
    unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &components, 0);

    if (!imageData || width <= 0 || height <= 0 || components < 1 || components > 4)
    {
        IO::Log::Error("Loading image file \"" + filename + "\" failed");
        return nullptr;
    }

    /* Create image object */
    const auto colorFormat = GetColorFormatByComponents(components);

    const Math::Size3st imageSize
    {
        static_cast<size_t>(width),
        static_cast<size_t>(height),
        1u
    };

    auto image = std::make_shared<ImageUByte>(imageSize, colorFormat);

    /* Copy image data */
    const size_t imageDataSize = width*height*components;
    std::copy(imageData, imageData + imageDataSize, image->RawBuffer());

    /* Release STBI image data */
    stbi_image_free(imageData);

    return image;
}

ImageFloatPtr CommonImageReader::ReadImageHDR(const std::string& filename, float gamma, float scale)
{
    /* Setup HDR reading settings */
    stbi_hdr_to_ldr_gamma(gamma);
    stbi_hdr_to_ldr_scale(scale);

    /* Load image using the STBI library */
    int width = 0, height = 0, components = 0;
    float* imageData = stbi_loadf(filename.c_str(), &width, &height, &components, 0);

    if (!imageData || width <= 0 || height <= 0 || components < 1 || components > 4)
    {
        IO::Log::Error(ToStr("Loading HDR image file \"") + filename + ToStr("\" failed"));
        return nullptr;
    }

    /* Create image object */
    const auto colorFormat = GetColorFormatByComponents(components);

    const Math::Size3st imageSize
    {
        static_cast<size_t>(width),
        static_cast<size_t>(height),
        1u
    };

    auto image = std::make_shared<ImageFloat>(imageSize, colorFormat);

    /* Copy image data */
    const size_t imageDataSize = width*height*components;
    std::copy(imageData, imageData + imageDataSize, image->RawBuffer());

    /* Release STBI image data */
    stbi_image_free(imageData);

    return image;
}


} // /namespace Video

} // /namespace Fork



// ========================