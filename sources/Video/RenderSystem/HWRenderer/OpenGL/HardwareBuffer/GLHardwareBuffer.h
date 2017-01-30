/*
 * OpenGL harware buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_HARDWARE_BUFFER_H__
#define __FORK_GL_HARDWARE_BUFFER_H__


#include "../GLExtensions.h"
#include "../GLCore.h"
#include "../RenderState/GLStateManager.h"


namespace Fork
{

namespace Video
{


//! Base class for all GL hardware buffers.
template <GLenum bufferTarget> class GLHardwareBuffer
{
    
    public:
        
        //! OpenGL buffer target.
        static const GLenum target = bufferTarget;

        GLHardwareBuffer()
        {
            glGenBuffers(1, &bufferID_);
            ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glGenBuffers", bufferID_);
        }
        virtual ~GLHardwareBuffer()
        {
            ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glDeleteBuffers", bufferID_);
            glDeleteBuffers(1, &bufferID_);
        }

        GLHardwareBuffer(const GLHardwareBuffer&) = delete;
        GLHardwareBuffer& operator = (const GLHardwareBuffer&) = delete;

        void Bind() const
        {
            activeStateMngr->BindBuffer(target, GetBufferID());
        }
        void Unbind() const
        {
            activeStateMngr->BindBuffer(target, 0);
        }

        void Bind(const GLuint index) const
        {
            activeStateMngr->BindBufferBase(target, index, GetBufferID());
        }
        void Unbind(const GLuint index) const
        {
            activeStateMngr->BindBufferBase(target, index, 0);
        }

        void Write(const void* data, const GLsizeiptr size, const GLenum usage = GL_STATIC_DRAW)
        {
            glBufferData(target, size, data, usage);
        }
        void WriteSub(const void* data, const GLsizeiptr size, const GLintptr offset)
        {
            glBufferSubData(target, offset, size, data);
        }

        //! Returns the OpenGL buffer ID number.
        inline GLuint GetBufferID() const
        {
            return bufferID_;
        }

    private:
        
        GLuint bufferID_ = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================