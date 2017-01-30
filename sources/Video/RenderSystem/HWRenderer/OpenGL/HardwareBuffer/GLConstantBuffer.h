/*
 * OpenGL constant buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_CONSTANT_BUFFER_H__
#define __FORK_GL_CONSTANT_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/ConstantBuffer.h"
#include "GLHardwareBuffer.h"


namespace Fork
{

namespace Video
{


//! OpenGL shader constant buffer implementation.
class GLConstantBuffer : public ConstantBuffer, public GLHardwareBuffer<GL_UNIFORM_BUFFER>
{
    
    public:
        
        GLConstantBuffer(const Description& desc);
        ~GLConstantBuffer();

        GLConstantBuffer(const GLConstantBuffer&) = delete;
        GLConstantBuffer& operator = (const GLConstantBuffer&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================