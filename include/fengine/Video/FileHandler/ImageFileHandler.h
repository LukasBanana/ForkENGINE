/*
 * Image file handler header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_FILE_HANDLER_H__
#define __FORK_IMAGE_FILE_HANDLER_H__


#include "Core/Export.h"
#include "Video/Image/Image.h"
#include "Video/FileHandler/ImageFileFormats.h"

#include <string>


namespace Fork
{

namespace Video
{

namespace ImageFileHandler
{


/**
Reads the specified image from file and automatically detects its file format.
\see ImageReader::ReadImage
*/
FORK_EXPORT ImageUBytePtr ReadImage(const std::string& filename);

/**
Reads the specified image from file and automatically detects its file format.
\note HDR means in this contect, that the image contains floating-point data, i.e. can also contain high-dynamic range values.
\see ImageReader::ReadImageHDR
\todo Not yet implemented!
*/
FORK_EXPORT ImageFloatPtr ReadImageHDR(const std::string& filename, float gamma = 2.2f, float scale = 1.0f);

/**
Writes the specified image to file using the specified image writer. By default PNG format is used.
\see ImageWriter::WriteImage
*/
FORK_EXPORT bool WriteImage(
    const ImageUBytePtr& image, const std::string& filename, const ImageFileFormats format = ImageFileFormats::PNG
);

/**
Writes the specified image to file using the specified image writer. By default HDR format is used.
\note HDR means in this contect, that the image contains floating-point data, i.e. can also contain high-dynamic range values.
\see ImageWriter::WriteImageHDR
\todo Not yet implemented!
*/
FORK_EXPORT bool WriteImageHDR(
    const ImageFloatPtr& image, const std::string& filename, const ImageFileFormats format = ImageFileFormats::HDR
);


} // /namespace ImageFileHandler

} // /namespace Video

} // /namespace Fork


#endif



// ========================