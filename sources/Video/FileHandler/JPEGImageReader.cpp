/*
 * JPEG image reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "JPEGImageReader.h"
#include "IO/Core/Log.h"
#include "IO/FileSystem/FileStreamHelper.h"
#include "../../Plugins/jpeglib/jpeglib.h"

#include <vector>
#include <fstream>


namespace Fork
{

namespace Video
{


static ImageUBytePtr DecompressJPEG(jpeg_decompress_struct& info)
{
    /* Start the decompression */
    if (jpeg_read_header(&info, true) == JPEG_SUSPENDED)
    {
        IO::Log::Error("Reading JPEG header failed");
        return nullptr;
    }
    
    jpeg_start_decompress(&info);
    
    /* Choose image color format */
    if (info.num_components < 1 || info.num_components > 4)
    {
        IO::Log::Error("Invalid number of color components in JPEG image");
        return nullptr;
    }

    const ImageColorFormats imageFormat = ChooseImageFormat(info.num_components);

    /* Create image object */
    auto image = std::make_shared<ImageUByte>(
        ImageUByte::SizeType(info.image_width, info.image_height, 1),
        imageFormat
    );

    /* Store pointers to each scanline in the image buffer */
    const auto rowSpan = info.image_width * info.num_components;
    
    unsigned char* buffer = image->RawBuffer();

    std::vector<unsigned char*> rowPtr(info.image_height);

    for (size_t i = 0; i < info.image_height; ++i)
    {
        rowPtr[i] = buffer;
        buffer += rowSpan;
    }

    /* Fill the image buffer for each scanline */
    size_t rowsRead = 0;
    while (info.output_scanline < info.output_height)
    {
        rowsRead += jpeg_read_scanlines(
            &info,
            &rowPtr[rowsRead],
            info.output_height - rowsRead
        );
    }

    /* Finish the decompression */
    jpeg_finish_decompress(&info);

    return image;
}

ImageUBytePtr JPEGImageReader::ReadImage(const std::string& filename)
{
    /* Open file for reading */
    std::ifstream file(filename, std::ios::binary);

    if (!file.good())
    {
        IO::Log::Error("Loading image file \"" + filename + "\" failed");
        return nullptr;
    }

    /* Read complete file into a buffer */
    const auto fileSize = static_cast<unsigned int>(IO::QueryFileSize(file));

    std::vector<unsigned char> buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    /* Initialize JPEG lib */
    jpeg_decompress_struct info;
    jpeg_error_mgr jerr;

    info.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&info);

    /* Decompress the JPEG image data */
    jpeg_mem_src(&info, buffer.data(), fileSize);

    auto image = DecompressJPEG(info);

    jpeg_destroy_decompress(&info);

    return image;
}

ImageFloatPtr JPEGImageReader::ReadImageHDR(const std::string& filename, float gamma, float scale)
{
    IO::Log::Error("HDR image not supported for JPEG images");
    return nullptr;
}


} // /namespace Video

} // /namespace Fork



// ========================