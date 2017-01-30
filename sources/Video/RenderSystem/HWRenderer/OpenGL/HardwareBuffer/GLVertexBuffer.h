/*
 * OpenGL vertex buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_VERTEX_BUFFER_H__
#define __FORK_GL_VERTEX_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/VertexBuffer.h"
#include "GLHardwareBuffer.h"


namespace Fork
{

namespace Video
{


//#define _DEB_CUSTOM_VERTEX_FORMAT_BINDING_


class GLVertexBuffer : public VertexBuffer, public GLHardwareBuffer<GL_ARRAY_BUFFER>
{
    
    public:
        
        GLVertexBuffer();
        ~GLVertexBuffer();

        GLVertexBuffer(const GLVertexBuffer&) = delete;
        GLVertexBuffer& operator = (const GLVertexBuffer&) = delete;

        void SetupFormat(const VertexFormat& format) override;

        void BindFormatted();
        void UnbindFormatted();

    private:
        
        #ifndef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_
        GLuint vaoID_ = 0; //!< VAO (Vertex Array Object) ID.
        #endif

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================