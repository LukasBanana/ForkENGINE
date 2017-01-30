/*
 * OpenGL render buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLRenderBuffer.h"
#include "../GLExtensions.h"
#include "../GLCore.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Video
{


GLRenderBuffer::GLRenderBuffer()
{
    // Don't ensure shared context -> renderbuffer objects are not sharable!
    glGenRenderbuffers(1, &bufferID_);
}
GLRenderBuffer::~GLRenderBuffer()
{
    glDeleteRenderbuffers(1, &bufferID_);
}

void GLRenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, GetBufferID());
}

void GLRenderBuffer::Unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GLRenderBuffer::SetupStorage(GLenum internalFormat, const Math::Size2i& size, GLsizei samples)
{
    if (samples > 0)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, size.width, size.height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size.width, size.height);
}

void GLRenderBuffer::Recreate()
{
    glDeleteRenderbuffers(1, &bufferID_);
    glGenRenderbuffers(1, &bufferID_);
}


} // /namespace Video

} // /namespace Fork



// ========================