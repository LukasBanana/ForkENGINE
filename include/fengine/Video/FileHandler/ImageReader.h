/*
 * Image reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_READER_H__
#define __FORK_IMAGE_READER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Image/Image.h"
#include "Video/FileHandler/ImageFileFormats.h"

#include <string>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(ImageReader);

//! Image reader interface.
class FORK_EXPORT ImageReader
{
    
    public:
        
        virtual ~ImageReader()
        {
        }

        /**
        Reads the image data from the specified file.
        \param[in] filename Specifies the filename from which the image is to be read.
        \return Shared pointer to the unsigned byte image object. If an error occured while loading the image,
        the return value is null and the error message will be printed into the log output.
        */
        virtual ImageUBytePtr ReadImage(const std::string& filename) = 0;
        /**
        Reads the HDR image data from the specified file.
        \param[in] filename Specifies the filename from which the image is to be read.
        \param[in] gamma Specifies the gamma correction value. By default 2.2.
        \param[in] scale Specifies the image scaling factor. By default 1.0.
        \return Shared pointer to the floating-point HDR image object. If an error occured while loading the image,
        the return value is null and the error message will be printed into the log output.
        */
        virtual ImageFloatPtr ReadImageHDR(const std::string& filename, float gamma = 2.2f, float scale = 1.0f) = 0;

    protected:
        
        ImageReader() = default;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================