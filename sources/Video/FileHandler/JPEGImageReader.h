/*
 * JPEG image reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_JPEG_IMAGE_READER_H__
#define __FORK_JPEG_IMAGE_READER_H__


#include "Video/FileHandler/ImageReader.h"


namespace Fork
{

namespace Video
{


class JPEGImageReader : public ImageReader
{
    
    public:
        
        ImageUBytePtr ReadImage(const std::string& filename);
        ImageFloatPtr ReadImageHDR(const std::string& filename, float gamma = 2.2f, float scale = 1.0f);

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================