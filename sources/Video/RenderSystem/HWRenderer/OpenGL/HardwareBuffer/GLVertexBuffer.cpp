/*
 * OpenGL vertex buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLVertexBuffer.h"
#include "..\GLParamMapper.h"

#include "IO/Core/Log.h"//!!!


namespace Fork
{

namespace Video
{


GLVertexBuffer::GLVertexBuffer()
{
    #ifndef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_
    glGenVertexArrays(1, &vaoID_);
    #endif
}
GLVertexBuffer::~GLVertexBuffer()
{
    #ifndef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_
    glDeleteVertexArrays(1, &vaoID_);
    #endif
}

void GLVertexBuffer::SetupFormat(const VertexFormat& format)
{
    #ifndef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_

    /* Bind VAO which is to be configured */
    activeStateMngr->BindVertexArray(vaoID_);

    /* Bind VBO to 'attach' it to the VAO */
    Bind();

    /* Setup each vertex attribute */
    GLuint index = 0;

    for (const auto& attrib : format.GetAttributes())
    {
        glEnableVertexAttribArray(index);

        glVertexAttribPointer(
            index,
            attrib.GetNumComponents(),
            GLParamMapper::Map(attrib.GetDataType()),
            GL_FALSE,
            format.GetFormatSize(),
            ((const char*)0) + attrib.GetOffset()
        );
        
        ++index;
    }

    activeStateMngr->BindVertexArray(0);

    #endif

    /* Store new vertex format */
    this->format = format;
}

void GLVertexBuffer::BindFormatted()
{
    #ifndef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_

    /* Bind VAO which contians the entire vertex format */
    activeStateMngr->BindVertexArray(vaoID_);

    #else

    Bind();

    /* Bind vertex format */
    GLuint index = 0;

    for (const auto& attrib : format_.GetAttributes())
    {
        glEnableVertexAttribArray(index);

        glVertexAttribPointer(
            index,
            attrib.GetNumComponents(),
            GLParamMapper::Map(attrib.GetDataType()),
            GL_FALSE,
            format_.GetFormatSize(),
            ((const char*)0) + attrib.GetOffset()
        );
        
        ++index;
    }

    #endif
}

void GLVertexBuffer::UnbindFormatted()
{
    #ifndef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_
    
    activeStateMngr->BindVertexArray(0);

    #else

    Bind();

    /* Unbind vertex format */
    GLuint index = 0;

    for (const auto& attrib : format_.GetAttributes())
    {
        glDisableVertexAttribArray(index);
        ++index;
    }

    Unbind();

    #endif
}


} // /namespace Video

} // /namespace Fork



// ========================