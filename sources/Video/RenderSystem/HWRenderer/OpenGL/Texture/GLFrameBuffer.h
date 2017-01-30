/*
 * OpenGL frame buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_FRAME_BUFFER_H__
#define __FORK_GL_FRAME_BUFFER_H__


#include "Math/Core/Size2.h"
#include "Math/Core/Rect.h"
#include "Math/Common/AxisDirections.h"
#include "../GLImport.h"


namespace Fork
{

namespace Video
{


class GLTexture1D;
class GLTexture2D;
class GLTexture3D;
class GLTextureCube;
class GLTextureBase;
class GLRenderBuffer;

class GLFrameBuffer
{
    
    public:
        
        GLFrameBuffer();
        ~GLFrameBuffer();

        GLFrameBuffer(const GLFrameBuffer&) = delete;
        GLFrameBuffer& operator = (const GLFrameBuffer&) = delete;
        
        void Bind(GLenum target = GL_FRAMEBUFFER) const;
        void Unbind(GLenum target = GL_FRAMEBUFFER) const;

        void AttachTexture(
            GLTexture1D* texture, GLenum attachment = GL_COLOR_ATTACHMENT0,
            GLenum target = GL_FRAMEBUFFER, GLint mipLevel = 0
        );
        void AttachTexture(
            GLTexture2D* texture, GLenum attachment = GL_COLOR_ATTACHMENT0,
            GLenum target = GL_FRAMEBUFFER, GLint mipLevel = 0
        );
        void AttachTexture(
            GLTexture3D* texture, GLint zOffset, GLenum attachment = GL_COLOR_ATTACHMENT0,
            GLenum target = GL_FRAMEBUFFER, GLint mipLevel = 0
        );
        void AttachTexture(
            GLTextureCube* texture, const Math::AxisDirections face, GLenum attachment = GL_COLOR_ATTACHMENT0,
            GLenum target = GL_FRAMEBUFFER, GLint mipLevel = 0
        );

        void AttachTextureLayer(
            GLTextureBase* texture, GLint layer, GLenum attachment = GL_COLOR_ATTACHMENT0,
            GLenum target = GL_FRAMEBUFFER, GLint mipLevel = 0
        );

        void AttachRenderBuffer(
            GLRenderBuffer* renderBuffer, GLenum attachment = GL_DEPTH_ATTACHMENT,
            GLenum target = GL_FRAMEBUFFER
        );

        void Detach(GLenum attachment, GLenum target = GL_FRAMEBUFFER);

        static void Blit(const Math::Size2i& size, GLenum mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST);
        static void Blit(const Math::Recti& srcRect, const Math::Recti& destRect, GLenum mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST);

        GLenum CheckStatus(GLenum target = GL_FRAMEBUFFER) const;

        /**
        Recreates the frame buffer object.
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