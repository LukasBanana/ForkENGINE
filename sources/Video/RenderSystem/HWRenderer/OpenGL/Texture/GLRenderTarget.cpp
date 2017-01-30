/*
 * OpenGL render target file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLRenderTarget.h"
#include "GLTexture1D.h"
#include "GLTexture2D.h"
#include "GLTexture3D.h"
#include "GLTextureCube.h"
#include "Core/CastToConcreteObject.h"
#include "../GLCore.h"
#include "../GLExtensions.h"
#include "IO/Core/Log.h"
#include "Core/Exception/PointerConversionException.h"
#include "Core/Exception/NullPointerException.h"

#include <algorithm>


namespace Fork
{

namespace Video
{


bool GLRenderTarget::Attach(
    const Texture1DPtr& texture, unsigned int attachment, unsigned int layer, unsigned int mipLevel)
{
    /* Validate input parameters */
    if (!ValidateMIPLevelInput(mipLevel, texture->HasMIPMaps()))
        return false;

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(Texture1D, texture.get());

    /* Initialize render buffer, initailize multi-sampled frame buffer and add attachment */
    InitRenderBuffer({ texture->GetSize(), 1 });
    AddAttachmentAndInitMS(attachment);

    bool result = false;

    frameBuffer_.Bind();
    {
        /* Attach render buffer to frame buffer */
        AttachDepthBuffer();

        /* Attach texture to frame buffer */
        if (textureGL->HasArray())
        {
            frameBuffer_.AttachTextureLayer(
                textureGL, layer, AttachmentPoint(attachment),
                GL_FRAMEBUFFER, static_cast<GLint>(mipLevel)
            );
        }
        else
        {
            frameBuffer_.AttachTexture(
                textureGL, AttachmentPoint(attachment),
                GL_FRAMEBUFFER, static_cast<GLint>(mipLevel)
            );
        }

        result = CheckStatus();

        /* Update draw buffer settings for this frame buffer */
        if (!frameBufferMS_)
            SetupDrawBuffers();
    }
    frameBuffer_.Unbind();

    requireRenderBufferInit_ = false;

    return result;
}

bool GLRenderTarget::Attach(
    const Texture2DPtr& texture, unsigned int attachment, unsigned int layer, unsigned int mipLevel)
{
    /* Validate input parameters */
    if (!ValidateMIPLevelInput(mipLevel, texture->HasMIPMaps()))
        return false;

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(Texture2D, texture.get());

    /* Initialize render buffer, initailize multi-sampled frame buffer and add attachment */
    #if 1
    InitRenderBuffer(texture->MIPLevelSize(mipLevel));//???
    #else
    InitRenderBuffer(texture->GetSize());//???
    #endif
    AddAttachmentAndInitMS(attachment);

    bool result = false;

    frameBuffer_.Bind();
    {
        /* Attach render buffer to frame buffer */
        AttachDepthBuffer();

        /* Attach texture to frame buffer */
        if (textureGL->HasArray())
        {
            frameBuffer_.AttachTextureLayer(
                textureGL, layer, AttachmentPoint(attachment),
                GL_FRAMEBUFFER, static_cast<GLint>(mipLevel)
            );
        }
        else
        {
            frameBuffer_.AttachTexture(
                textureGL, AttachmentPoint(attachment),
                GL_FRAMEBUFFER, static_cast<GLint>(mipLevel)
            );
        }

        result = CheckStatus();

        /* Update draw buffer settings for this frame buffer */
        if (!frameBufferMS_)
            SetupDrawBuffers();
    }
    frameBuffer_.Unbind();

    requireRenderBufferInit_ = false;

    return result;
}

bool GLRenderTarget::Attach(
    const Texture3DPtr& texture, unsigned int attachment, unsigned int zOffset, unsigned int mipLevel)
{
    /* Validate input parameters */
    if (!ValidateMIPLevelInput(mipLevel, texture->HasMIPMaps()))
        return false;

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(Texture3D, texture.get());

    /* Initialize render buffer, initailize multi-sampled frame buffer and add attachment */
    InitRenderBuffer(texture->GetSize().Sz2());
    AddAttachmentAndInitMS(attachment);

    bool result = false;

    frameBuffer_.Bind();
    {
        /* Attach render buffer to frame buffer */
        AttachDepthBuffer();

        /* Attach texture to frame buffer */
        frameBuffer_.AttachTexture(
            textureGL, static_cast<GLint>(zOffset), AttachmentPoint(attachment),
            GL_FRAMEBUFFER, static_cast<GLint>(mipLevel)
        );

        result = CheckStatus();

        /* Update draw buffer settings for this frame buffer */
        if (!frameBufferMS_)
            SetupDrawBuffers();
    }
    frameBuffer_.Unbind();

    requireRenderBufferInit_ = false;

    return result;
}

bool GLRenderTarget::Attach(
    const TextureCubePtr& texture, const TextureCube::Faces face, unsigned int attachment, unsigned int layer, unsigned int mipLevel)
{
    /* Validate input parameters */
    if (!ValidateMIPLevelInput(mipLevel, texture->HasMIPMaps()))
        return false;

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(TextureCube, texture.get());

    /* Initialize render buffer, initailize multi-sampled frame buffer and add attachment */
    InitRenderBuffer(texture->GetSize());
    AddAttachmentAndInitMS(attachment);

    bool result = false;

    frameBuffer_.Bind();
    {
        /* Attach render buffer to frame buffer */
        AttachDepthBuffer();

        /* Attach texture to frame buffer */
        if (textureGL->HasArray())
        {
            frameBuffer_.AttachTextureLayer(
                textureGL, static_cast<GLint>(TextureCube::MapArrayLayer(layer, face)), AttachmentPoint(attachment),
                GL_FRAMEBUFFER, static_cast<GLint>(mipLevel)
            );
        }
        else
        {
            frameBuffer_.AttachTexture(
                textureGL, face, AttachmentPoint(attachment),
                GL_FRAMEBUFFER, static_cast<GLint>(mipLevel)
            );
        }

        result = CheckStatus();

        /* Update draw buffer settings for this frame buffer */
        if (!frameBufferMS_)
            SetupDrawBuffers();
    }
    frameBuffer_.Unbind();

    requireRenderBufferInit_ = false;

    return result;
}

/*
Recreate framebuffer instead of simple clear.
-> Some drivers have problems to resize a framebuffer, so we recreate it completely.
*/
#define _DEB_RECREATE_FRAMEBUFFER_ON_CLEAR_

void GLRenderTarget::Clear()
{
    #ifdef _DEB_RECREATE_FRAMEBUFFER_ON_CLEAR_

    /* Recreate all framebuffer and renderbuffer objects */
    frameBuffer_.Recreate();
    renderBuffer_.Recreate();

    if (frameBufferMS_)
        frameBufferMS_->Recreate();

    #else

    /* Detach all texture attachments from the main frame buffer */
    frameBuffer_.Bind();
    {
        frameBuffer_.Detach(GL_DEPTH_ATTACHMENT);
        for (auto attachment : attachmentPoints_)
            frameBuffer_.Detach(attachment);
    }
    frameBuffer_.Unbind();

    if (frameBufferMS_)
    {
        /* Detach all render buffer attachments from the multi-samples frame buffer */
        frameBufferMS_->Bind();
        {
            frameBufferMS_->Detach(GL_DEPTH_ATTACHMENT);
            for (auto attachment : attachmentPoints_)
                frameBufferMS_->Detach(attachment);
        }
        frameBufferMS_->Unbind();
    }

    #endif

    /*
    Clear all lists and reset render-buffer-init flag.
    -> this will force the render target to re-create its render-buffer,
       when a new attachment will be added.
    */
    attachmentPoints_.clear();
    renderBuffersMS_.clear();

    requireRenderBufferInit_ = true;
}

void GLRenderTarget::SetupMultiSamples(int numSamples)
{
    if (multiSamples_ == numSamples)
        return;
    
    if (numSamples > 0)
    {
        /* Create multi-sampled frame buffer if we don't have one */
        if (!frameBufferMS_)
            frameBufferMS_ = std::make_unique<GLFrameBuffer>();
    }
    else
        frameBufferMS_ = nullptr;

    /* Store new multi-sampling setting */
    multiSamples_ = numSamples;

    //!TODO -> update frame buffers ...
}


/*
 * ======= Protected: =======
 */

void GLRenderTarget::BlitMultiSampleFrameBuffers()
{
    if (!frameBufferMS_)
        throw NullPointerException(__FUNCTION__, "[Internal] GLRenderTarget::frameBufferMS_");

    frameBuffer_.Bind(GL_DRAW_FRAMEBUFFER);
    frameBufferMS_->Bind(GL_READ_FRAMEBUFFER);

    for (auto attachment : attachmentPoints_)
    {
        /*
        Blit (or rather copy) each attachment from the
        multi-sample frame buffer into the main frame buffer
        */
        glReadBuffer(attachment);
        glDrawBuffer(attachment);

        GLFrameBuffer::Blit(GetSize());
    }

    frameBufferMS_->Unbind(GL_READ_FRAMEBUFFER);
    frameBuffer_.Unbind(GL_DRAW_FRAMEBUFFER);
}


/*
 * ======= Private: =======
 */

void GLRenderTarget::InitRenderBuffer(const Math::Size2i& size)
{
    if (!requireRenderBufferInit_)
        return;
    
    /* Store render target size */
    size_ = size;

    /* Setup render buffer storage */
    renderBuffer_.Bind();
    {
        renderBuffer_.SetupStorage(GL_DEPTH_COMPONENT24, size, GetMultiSamples());
    }
    renderBuffer_.Unbind();
}

void GLRenderTarget::CreateAndInitRenderBuffersMS(const Math::Size2i& size)
{
    while (renderBuffersMS_.size() < attachmentPoints_.size())
    {
        /*
        Create and initialize new render buffer and attach
        it to the multi-sampled frame buffer.
        */
        auto renderBuffer = std::make_unique<GLRenderBuffer>();

        /* Setup render buffer multi-sampling storage */
        renderBuffer->Bind();
        {
            auto internalFormat = GL_RGBA;// <--- !!!
            renderBuffer->SetupStorage(internalFormat, size, GetMultiSamples());
        }
        renderBuffer->Unbind();

        /* Attach render buffer to multi-sampled frame buffer */
        frameBufferMS_->AttachRenderBuffer(renderBuffer.get(), attachmentPoints_[renderBuffersMS_.size()]);

        renderBuffersMS_.push_back(std::move(renderBuffer));
    }
}

bool GLRenderTarget::CheckStatus()
{
    const auto status = frameBuffer_.CheckStatus();

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        #ifdef FORK_DEBUG
        IO::Log::Debug(
            "GL Error", "GLRenderTarget::Attach",
            "OpenGL render target attachment failed (" + GLErrorToStr(status) + ")",
            10
        );
        #endif
        return false;
    }

    return true;
}

void GLRenderTarget::AttachDepthBuffer()
{
    if (requireRenderBufferInit_ && !frameBufferMS_)
        frameBuffer_.AttachRenderBuffer(&renderBuffer_);
}

void GLRenderTarget::AddAttachmentAndInitMS(unsigned int attachment)
{
    if (frameBufferMS_)
    {
        frameBufferMS_->Bind();
        {
            if (requireRenderBufferInit_)
                frameBufferMS_->AttachRenderBuffer(&renderBuffer_);

            AddAttachmentPoint(attachment);

            SetupDrawBuffers();
        }
        frameBufferMS_->Unbind();
    }
    else
        AddAttachmentPoint(attachment);
}

void GLRenderTarget::AddAttachmentPoint(unsigned int attachment)
{
    auto attachmentPoint = AttachmentPoint(attachment);

    /*
    Append attachment point only if it's not a depth-attachment
    (only GL_COLOR_ATTACHMENT[n]) and if it has not already been added.
    */
    if ( attachment != RenderTarget::attachmentDepthMap &&
         std::find(attachmentPoints_.begin(), attachmentPoints_.end(), attachmentPoint) == attachmentPoints_.end() )
    {
        attachmentPoints_.push_back(attachmentPoint);

        if (frameBufferMS_)
            CreateAndInitRenderBuffersMS(GetSize());
    }
}

GLenum GLRenderTarget::AttachmentPoint(unsigned int attachment) const
{
    return attachment == RenderTarget::attachmentDepthMap ?
        GL_DEPTH_ATTACHMENT :
        GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(attachment);
}

void GLRenderTarget::SetupDrawBuffers()
{
    /*
    Tell OpenGL which buffers are to be written when drawing operations are performed.
    -> Each attachment has its own draw buffer (only the depth-map render buffer is ignored here).
    */
    if (attachmentPoints_.empty())
        glDrawBuffer(GL_NONE);
    else if (attachmentPoints_.size() == 1)
        glDrawBuffer(attachmentPoints_.front());
    else
        glDrawBuffers(static_cast<GLsizei>(attachmentPoints_.size()), attachmentPoints_.data());
}


} // /namespace Video

} // /namespace Fork



// ========================