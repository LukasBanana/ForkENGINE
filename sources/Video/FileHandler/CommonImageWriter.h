/*
 * Common image writer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMMON_IMAGE_WRITER_H__
#define __FORK_COMMON_IMAGE_WRITER_H__


#include "Video/FileHandler/ImageWriter.h"


namespace Fork
{

namespace Video
{


class CommonImageWriter : public ImageWriter
{
    
    public:
        
        CommonImageWriter(const ImageFileFormats fileFormat);

        bool WriteImage(const ImageUBytePtr& image, const std::string& filename);
        bool WriteImageHDR(const ImageFloatPtr& image, const std::string& filename);

    private:
        
        ImageFileFormats fileFormat_ = ImageFileFormats::__Unknown__;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================