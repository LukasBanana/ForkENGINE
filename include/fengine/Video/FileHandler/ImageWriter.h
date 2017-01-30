/*
 * Image writer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_WRITER_H__
#define __FORK_IMAGE_WRITER_H__


#include "Core/Export.h"
#include "Video/Image/Image.h"
#include "Video/FileHandler/ImageFileFormats.h"

#include <string>


namespace Fork
{

namespace Video
{


//! Image writer interface.
class FORK_EXPORT ImageWriter
{
    
    public:
        
        virtual ~ImageWriter()
        {
        }

        /**
        Writes the image data to the specified file.
        \param[in] filename Specifies the filename which is to be written (or rather created).
        \return True on success, otherwise an error occured which will be printed into the log output.
        \throws NullPointerException If 'image' is null.
        */
        virtual bool WriteImage(const ImageUBytePtr& image, const std::string& filename) = 0;
        /**
        Writes the HDR image data to the specified file.
        \param[in] filename Specifies the filename which is to be written (or rather created).
        \return True on success, otherwise an error occured which will be printed into the log output.
        \throws NullPointerException If 'image' is null.
        */
        virtual bool WriteImageHDR(const ImageFloatPtr& image, const std::string& filename) = 0;

    protected:
        
        ImageWriter() = default;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================