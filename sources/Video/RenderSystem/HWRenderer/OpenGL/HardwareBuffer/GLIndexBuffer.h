/*
 * OpenGL index buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_INDEX_BUFFER_H__
#define __FORK_GL_INDEX_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/IndexBuffer.h"
#include "GLHardwareBuffer.h"


namespace Fork
{

namespace Video
{


class GLIndexBuffer : public IndexBuffer, public GLHardwareBuffer<GL_ELEMENT_ARRAY_BUFFER>
{
    
    public:
        
        GLIndexBuffer() = default;
        ~GLIndexBuffer();

        GLIndexBuffer(const GLIndexBuffer&) = delete;
        GLIndexBuffer& operator = (const GLIndexBuffer&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================