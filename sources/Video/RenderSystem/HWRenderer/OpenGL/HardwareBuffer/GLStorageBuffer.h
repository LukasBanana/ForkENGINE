/*
 * OpenGL storage buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_STORAGE_BUFFER_H__
#define __FORK_GL_STORAGE_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/StorageBuffer.h"
#include "GLHardwareBuffer.h"


namespace Fork
{

namespace Video
{


//! OpenGL shader storage buffer implementation.
class GLStorageBuffer : public StorageBuffer, public GLHardwareBuffer<GL_SHADER_STORAGE_BUFFER>
{
    
    public:
        
        GLStorageBuffer(const Description& desc);
        ~GLStorageBuffer();

        GLStorageBuffer(const GLStorageBuffer&) = delete;
        GLStorageBuffer& operator = (const GLStorageBuffer&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================