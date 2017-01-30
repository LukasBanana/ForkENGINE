/*
 * OpenGL render buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_RENDER_BUFFER_H__
#define __FORK_GL_RENDER_BUFFER_H__


#include "Math/Core/Size2.h"
#include "../GLImport.h"


namespace Fork
{

namespace Video
{


class GLRenderBuffer
{
    
    public:
        
        GLRenderBuffer();
        ~GLRenderBuffer();

        GLRenderBuffer(const GLRenderBuffer&) = delete;
        GLRenderBuffer& operator = (const GLRenderBuffer&) = delete;
        
        void Bind();
        void Unbind();

        void SetupStorage(GLenum internalFormat, const Math::Size2i& size, GLsizei samples = 0);

        /**
        Recreates the render buffer object.
        \note Some drivers have problems to resize a framebuffer, so we recreate it completely.
        */
        void Recreate();

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