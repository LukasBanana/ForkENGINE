/*
 * OpenGL frame buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLFrameBuffer.h"
#include "GLRenderBuffer.h"
#include "GLTexture1D.h"
#include "GLTexture2D.h"
#include "GLTexture3D.h"
#include "GLTextureCube.h"
#include "../GLParamMapper.h"
#include "../GLExtensions.h"
#include "../GLCore.h"


namespace Fork
{

namespace Video
{


GLFrameBuffer::GLFrameBuffer()
{
    // Don' ensure shared context -> framebuffer objects are not sharable!
    glGenFramebuffers(1, &bufferID_);
}
GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1, &bufferID_);
}

void GLFrameBuffer::Bind(GLenum target) const
{
    glBindFramebuffer(target, GetBufferID());
}

void GLFrameBuffer::Unbind(GLenum target) const
{
    glBindFramebuffer(target, 0);
}

void GLFrameBuffer::AttachTexture(GLTexture1D* texture, GLenum attachment, GLenum target, GLint mipLevel)
{
    glFramebufferTexture1D(target, attachment, texture->GetTarget(), texture->GetTextureID(), mipLevel);
}

void GLFrameBuffer::AttachTexture(GLTexture2D* texture, GLenum attachment, GLenum target, GLint mipLevel)
{
    glFramebufferTexture2D(target, attachment, texture->GetTarget(), texture->GetTextureID(), mipLevel);
}

void GLFrameBuffer::AttachTexture(GLTexture3D* texture, GLint zOffset, GLenum attachment, GLenum target, GLint mipLevel)
{
    glFramebufferTexture3D(target, attachment, texture->GetTarget(), texture->GetTextureID(), mipLevel, zOffset);
}

void GLFrameBuffer::AttachTexture(GLTextureCube* texture, const Math::AxisDirections face, GLenum attachment, GLenum target, GLint mipLevel)
{
    glFramebufferTexture2D(target, attachment, GLParamMapper::Map(face), texture->GetTextureID(), mipLevel);
}

void GLFrameBuffer::AttachTextureLayer(GLTextureBase* texture, GLint layer, GLenum attachment, GLenum target, GLint mipLevel)
{
    glFramebufferTextureLayer(target, attachment, texture->GetTextureID(), mipLevel, layer);
}

void GLFrameBuffer::AttachRenderBuffer(GLRenderBuffer* renderBuffer, GLenum attachment, GLenum target)
{
    glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, renderBuffer->GetBufferID());
}

void GLFrameBuffer::Detach(GLenum attachment, GLenum target)
{
    glFramebufferTexture(target, attachment, 0, 0);
}

void GLFrameBuffer::Blit(const Math::Size2i& size, GLenum mask, GLenum filter)
{
    glBlitFramebuffer(
        0, 0, size.width, size.height,
        0, 0, size.width, size.height,
        mask, filter
    );
}

void GLFrameBuffer::Blit(const Math::Recti& srcRect, const Math::Recti& destRect, GLenum mask, GLenum filter)
{
    glBlitFramebuffer(
        srcRect.left, srcRect.top, srcRect.right, srcRect.bottom,
        destRect.left, destRect.top, destRect.right, destRect.bottom,
        mask, filter
    );
}

GLenum GLFrameBuffer::CheckStatus(GLenum target) const
{
    return glCheckFramebufferStatus(target);
}

void GLFrameBuffer::Recreate()
{
    glDeleteFramebuffers(1, &bufferID_);
    glGenFramebuffers(1, &bufferID_);
}


} // /namespace Video

} // /namespace Fork



// ========================