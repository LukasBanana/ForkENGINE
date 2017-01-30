/*
 * Image file handler file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/FileHandler/ImageFileHandler.h"
#include "Video/FileHandler/ImageFileFormats.h"
#include "IO/Core/Log.h"
#include "IO/Core/Console.h"

#include "CommonImageReader.h"
#include "CommonImageWriter.h"
#include "JPEGImageReader.h"


namespace Fork
{

namespace Video
{

namespace ImageFileHandler
{


FORK_EXPORT ImageUBytePtr ReadImage(const std::string& filename)
{
    /* Choose image reader by file format */
    auto fileFormat = Video::DetectFileFormat(filename);

    std::unique_ptr<ImageReader> imageReader;

    switch (fileFormat)
    {
        case ImageFileFormats::JPG:
            /*
            Use a separated image reader onyl for JPEGs,
            -> to support baseline and progressive JPEG images.
            */
            imageReader = std::make_unique<JPEGImageReader>();
            break;

        default:
            /* Use common image reader for default types */
            imageReader = std::make_unique<CommonImageReader>();
            break;
    }

    /* Read image data */
    return imageReader != nullptr ? imageReader->ReadImage(filename) : nullptr;
}

FORK_EXPORT ImageFloatPtr ReadImageHDR(const std::string& filename, float gamma, float scale)
{
    CommonImageReader imageReader;
    return imageReader.ReadImageHDR(filename, gamma, scale);
}

FORK_EXPORT bool WriteImage(
    const ImageUBytePtr& image, const std::string& filename, const ImageFileFormats format)
{
    /* Choose image writer by format */
    std::unique_ptr<ImageWriter> imageWriter;

    switch (format)
    {
        case ImageFileFormats::BMP:
        case ImageFileFormats::PNG:
        case ImageFileFormats::TGA:
            /* Use common image writer for default types */
            imageWriter = std::make_unique<CommonImageWriter>(format);
            break;

        default:
            IO::Log::Error("Unsupported file format to write image \"" + filename + "\"");
            break;
    }

    /* Write image data */
    return imageWriter != nullptr ? imageWriter->WriteImage(image, filename) : false;
}

FORK_EXPORT bool WriteImageHDR(
    const ImageFloatPtr& image, const std::string& filename, const ImageFileFormats format)
{
    IO::Log::Error("Video::ImageFileHandler::WriteImageHDR is not supported");
    return false;
}


} // /namespace ImageFileHandler

} // /namespace Video

} // /namespace Fork



// ========================