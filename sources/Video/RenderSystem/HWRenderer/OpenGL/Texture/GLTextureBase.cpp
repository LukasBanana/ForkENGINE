/*
 * OpenGL texture base file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLTextureBase.h"
#include "../GLExtensions.h"
#include "../GLCore.h"


namespace Fork
{

namespace Video
{


GLTextureBase::GLTextureBase(GLenum target) :
    target_{ target }
{
    glGenTextures(1, &textureID_);
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glGenTextures", textureID_);
}
GLTextureBase::~GLTextureBase()
{
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glDeleteTextures", textureID_);
    glDeleteTextures(1, &textureID_);
}

void GLTextureBase::Bind() const
{
    glBindTexture(GetTarget(), GetTextureID());
}

void GLTextureBase::Unbind() const
{
    glBindTexture(GetTarget(), 0);
}

void GLTextureBase::Bind(unsigned int layer) const
{
    glActiveTexture(GL_TEXTURE0 + layer);
    glBindTexture(GetTarget(), GetTextureID());
}

void GLTextureBase::Unbind(unsigned int layer) const
{
    glActiveTexture(GL_TEXTURE0 + layer);
    glBindTexture(GetTarget(), 0);
}


} // /namespace Video

} // /namespace Fork



// ========================