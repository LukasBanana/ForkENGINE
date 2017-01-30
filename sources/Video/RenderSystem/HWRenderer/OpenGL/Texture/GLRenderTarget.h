/*
 * OpenGL render target header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_RENDER_TARGET_H__
#define __FORK_GL_RENDER_TARGET_H__


#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "GLFrameBuffer.h"
#include "GLRenderBuffer.h"


namespace Fork
{

namespace Video
{


//! OpenGL render target implementation.
class FORK_EXPORT GLRenderTarget : public RenderTarget
{
    
    public:
        
        GLRenderTarget() = default;

        GLRenderTarget(const GLRenderTarget&) = delete;
        GLRenderTarget& operator = (const GLRenderTarget&) = delete;
        
        bool Attach(
            const Texture1DPtr& texture, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        );
        bool Attach(
            const Texture2DPtr& texture, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        );
        bool Attach(
            const Texture3DPtr& texture, unsigned int attachment = 0,
            unsigned int zOffset = 0, unsigned int mipLevel = 0
        );
        bool Attach(
            const TextureCubePtr& texture, const TextureCube::Faces face, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        );

        void Clear();

        void SetupMultiSamples(int numSamples);

    protected:
        
        friend class GLRenderContext;

        typedef std::unique_ptr<GLRenderBuffer> GLRenderBufferPtr;
        typedef std::unique_ptr<GLFrameBuffer> GLFrameBufferPtr;

        /**
        Blits (or rather copies) the image data from the multi-sampled frame buffer into the main frame buffer.
        This should be called when drawing operations into a multi-sampled render target are finished.
        */
        void BlitMultiSampleFrameBuffers();

        inline GLFrameBuffer* GetFrameBuffer()
        {
            return &frameBuffer_;
        }
        inline const GLFrameBuffer* GetFrameBuffer() const
        {
            return &frameBuffer_;
        }

        inline GLRenderBuffer* GetRenderBuffer()
        {
            return &renderBuffer_;
        }
        inline const GLRenderBuffer* GetRenderBuffer() const
        {
            return &renderBuffer_;
        }

        inline GLFrameBuffer* GetFrameBufferMS()
        {
            return frameBufferMS_.get();
        }
        inline const GLFrameBuffer* GetFrameBufferMS() const
        {
            return frameBufferMS_.get();
        }

        inline const std::vector<GLRenderBufferPtr>& GetRenderBuffersMS() const
        {
            return renderBuffersMS_;
        }

    private:

        /* === Functions === */

        void InitRenderBuffer(const Math::Size2i& size);
        void CreateAndInitRenderBuffersMS(const Math::Size2i& size);

        bool CheckStatus();

        void AttachDepthBuffer();

        void AddAttachmentAndInitMS(unsigned int attachment);
        void AddAttachmentPoint(unsigned int attachment);
        GLenum AttachmentPoint(unsigned int attachment) const;

        void SetupDrawBuffers();

        /* === Members === */

        GLFrameBuffer                   frameBuffer_;                       //!< Main frame buffer.
        GLRenderBuffer                  renderBuffer_;                      //!< Main depth-map render buffer.

        /**
        Multi-samples frame buffer. This is required since we
        cannot directly draw into a texture when using multi-sampling.
        */
        GLFrameBufferPtr                frameBufferMS_;
        /**
        For multi-sampled render target we also need a render buffer for each attached texture.
        This is because we cannot directly draw into a texture when usign multi-sampling.
        */
        std::vector<GLRenderBufferPtr>  renderBuffersMS_;

        std::vector<GLenum>             attachmentPoints_;

        bool                            requireRenderBufferInit_ = true;    //!< Specifies whether the render buffer is required to be initialized.

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================