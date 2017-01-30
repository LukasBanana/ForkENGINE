/*
 * Common image writer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "CommonImageWriter.h"
#include "IO/Core/Log.h"
#include "Core/Exception/NullPointerException.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../Plugins/STB/stb_image_write.h"


namespace Fork
{

namespace Video
{


CommonImageWriter::CommonImageWriter(const ImageFileFormats fileFormat) :
    fileFormat_{ fileFormat }
{
}

bool CommonImageWriter::WriteImage(const ImageUBytePtr& image, const std::string& filename)
{
    ASSERT_POINTER(image);

    /* Collect image parameters */
    int width       = static_cast<int>(image->GetSize().width);
    int height      = static_cast<int>(image->GetSize().height);
    int components  = static_cast<int>(image->NumColorComponents());
    int stride      = static_cast<int>(image->Stride());

    if (width <= 0 || height <= 0 || components < 1 || components > 4)
    {
        IO::Log::Error(ToStr("Writing image file \"") + filename + ToStr("\" failed"));
        return false;
    }

    /* Write image using the STBI library */
    int result = 0;

    switch (fileFormat_)
    {
        case ImageFileFormats::BMP:
            result = stbi_write_bmp(filename.c_str(), width, height, components, image->RawBuffer());
            break;
        case ImageFileFormats::PNG:
            result = stbi_write_png(filename.c_str(), width, height, components, image->RawBuffer(), stride);
            break;
        case ImageFileFormats::TGA:
            result = stbi_write_tga(filename.c_str(), width, height, components, image->RawBuffer());
            break;
        default:
            return false;
    }

    /* Check for errors */
    if (!result)
    {
        IO::Log::Error(ToStr("Writing image file \"") + filename + ToStr("\" failed"));
        return false;
    }

    return true;
}

bool CommonImageWriter::WriteImageHDR(const ImageFloatPtr& image, const std::string& filename)
{
    ASSERT_POINTER(image);
    return false;
}


} // /namespace Video

} // /namespace Fork



// ========================