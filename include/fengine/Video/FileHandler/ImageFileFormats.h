/*
 * Image file formats header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_FILE_FORMATS_H__
#define __FORK_IMAGE_FILE_FORMATS_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Video
{


//! Image file formats.
enum class ImageFileFormats
{
    __Unknown__,    //!< Unknown file format.
    BMP,            //!< Windows Bitmaps. Uncompressed RGB image data. File extensions are: .bmp, .dib.
    JPG,            //!< Joint Photographics Experts Group. Supports high compression (with data loss). File extensions are: .jpg, .jpeg, .jpe, .jfif.
    PNG,            //!< Portable Network Graphics. Supports alpha channel and loss free data compression. File extensions are: .png.
    TGA,            //!< Truevision Advanced Raster Graphics Array. File extensions are: .tga, .tpic.
    DDS,            //!< Direct Draw Surface. Supports 2D-, 3D- and Cubemap images. File extensions are: .dds.
    PCX,            //!< PiCture eXchange. Simple compression used in legacy video games (e.g. Quake II). File extensions are: .pcx.
    HDR,            //!< High Dynamic Range. File extensions are: .hdr.
};


/**
Returns the format type of the specified image file.
\see ImageFileFormats
*/
FORK_EXPORT ImageFileFormats DetectFileFormat(const std::string& filename);


} // /namespace Video

} // /namespace Fork


#endif



// ========================