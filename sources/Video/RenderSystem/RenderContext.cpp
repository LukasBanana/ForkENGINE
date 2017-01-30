/*
 * Render context file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/FileHandler/ImageFileHandler.h"
#include "Platform/Core/Frame.h"
#include "Platform/Core/Desktop.h"
#include "IO/Core/Log.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Video
{


RenderContext* RenderContext::activeRenderContext_ = nullptr;

FrameReCreateCallback RenderContext::frameReCreateCallback_;
void* RenderContext::frameReCreateUserData_ = nullptr;

RenderContext::RenderContext(
    const VideoMode& videoMode, const ContextDescription& contextDesc) :
    videoMode_  { videoMode   },
    contextDesc_{ contextDesc }
{
}
RenderContext::~RenderContext()
{
}

RenderContext* RenderContext::Active()
{
    return activeRenderContext_;
}

void RenderContext::SetFrameReCreationCallback(const FrameReCreateCallback& callback, void* userData)
{
    frameReCreateCallback_ = callback;
    frameReCreateUserData_ = userData;
}

Shader::Versions RenderContext::HighestShaderModel() const
{
    auto shaderModels = ShaderModels();
    Shader::Versions highestShaderModel = Shader::Versions::__Unspecified__;

    for (auto version : shaderModels)
    {
        if (highestShaderModel < version)
            highestShaderModel = version;
    }

    return highestShaderModel;
}

bool RenderContext::IsActivated() const
{
    return RenderContext::activeRenderContext_ == this;
}

bool RenderContext::IsSharedContext() const
{
    return RenderSystem::Active()->GetSharedRenderContext() == this;
}

void RenderContext::SetupClearColor(const ColorRGBAf& color)
{
    clearColor_ = color;
}

void RenderContext::SetupClearDepth(float depth)
{
    clearDepth_ = depth;
}

void RenderContext::SetupClearStencil(unsigned char stencil)
{
    clearStencil_ = stencil;
}

bool RenderContext::ResizeResolution(const Math::Size2ui& resolution)
{
    auto videoMode = GetVideoMode();
    videoMode.resolution = resolution;
    return ChangeVideoMode(videoMode);
}

bool RenderContext::EnableFullscreen(bool enable)
{
    auto videoMode = GetVideoMode();
    videoMode.isFullscreen = enable;
    return ChangeVideoMode(videoMode);
}

bool RenderContext::EnableVSync(bool enable)
{
    auto contextDesc = GetDesc();
    contextDesc.vsyncDesc.isEnabled = enable;
    return ChangeDesc(contextDesc);
}

/* --- Render states --- */

void RenderContext::PushRenderAttribute(const RenderAttributes state)
{
    switch (state)
    {
        case RenderAttributes::Viewport:
            renderAttribStack_.push(GetRenderState().viewport, state);
            break;
        case RenderAttributes::Scissor:
            renderAttribStack_.push(GetRenderState().scissor, state);
            break;
        default:
            throw InvalidArgumentException(__FUNCTION__, "state", "Unknown render state");
            break;
    }
}

void RenderContext::PushRenderAttribute(const std::initializer_list<RenderAttributes>& stateList)
{
    for (const auto& state : stateList)
        PushRenderAttribute(state);
}

void RenderContext::PopRenderAttribute()
{
    if (!renderAttribStack_.empty())
    {
        switch (renderAttribStack_.top_id())
        {
            case RenderAttributes::Viewport:
                SetupViewport(renderAttribStack_.top<Viewport>());
                break;
            case RenderAttributes::Scissor:
                SetupScissor(renderAttribStack_.top<Scissor>());
                break;
        }
        renderAttribStack_.pop();
    }
}

void RenderContext::PopRenderAttribute(size_t count)
{
    while (count-- > 0)
        PopRenderAttribute();
}

void RenderContext::SetupPointSize(float size)
{
    globalRenderState_.pointSize = size;
}

void RenderContext::SetupLineSize(float size)
{
    globalRenderState_.lineSize = size;
}

void RenderContext::SetupViewport()
{
    SetupViewport({ {}, GetVideoMode().resolution.Cast<int>() });
}

void RenderContext::SetupViewport(const Viewport& viewport)
{
    globalRenderState_.viewport = viewport;
    SetupMultiViewport({ viewport });
}

void RenderContext::SetupScissor(const Scissor& scissor)
{
    globalRenderState_.scissor = scissor;
    SetupMultiScissor({ scissor });
}

void RenderContext::SetupProjectionMatrix(const Math::Matrix4f& projectionMatrix)
{
    /* Store projection matrix */
    globalRenderState_.projectionMatrix = projectionMatrix;

    /* Extract near- and far clipping planes */
    Projectionf::ExtractClippingPlanesLH(
        projectionMatrix, globalRenderState_.nearPlane, globalRenderState_.farPlane
    );
}

void RenderContext::SetupProjectionMatrix(const Projectionf& projection)
{
    #ifdef FORK_DEBUG
    if (!projection.IsValid())
        IO::Log::Debug(__FUNCTION__, "Invalid projection", 1);
    #endif

    /* Store projection matrix */
    globalRenderState_.projectionMatrix = projection.GetMatrixLH();

    /* Store near- and far clipping planes */
    globalRenderState_.nearPlane = projection.GetNearPlane();
    globalRenderState_.farPlane = projection.GetFarPlane();
}

void RenderContext::SetupViewMatrix(const Math::Matrix4f& viewMatrix)
{
    globalRenderState_.cameraMatrix = viewMatrix.Inverse();
    globalRenderState_.viewMatrix = viewMatrix;
}

void RenderContext::SetupWorldMatrix(const Math::Matrix4f& worldMatrix)
{
    globalRenderState_.worldMatrix = worldMatrix;
}

Math::Matrix4f RenderContext::WVPMatrix() const
{
    Math::Matrix4f matrix;
    
    matrix = globalRenderState_.projectionMatrix;
    matrix *= globalRenderState_.viewMatrix;
    matrix *= globalRenderState_.worldMatrix;

    return matrix;
}

Math::Matrix4f RenderContext::WVMatrix() const
{
    Math::Matrix4f matrix;
    
    matrix = globalRenderState_.viewMatrix;
    matrix *= globalRenderState_.worldMatrix;

    return matrix;
}

Math::Matrix4f RenderContext::VPMatrix() const
{
    Math::Matrix4f matrix;
    
    matrix = globalRenderState_.projectionMatrix;
    matrix *= globalRenderState_.viewMatrix;

    return matrix;
}

/* --- Textures --- */

bool RenderContext::SaveScreenshot(const std::string& filename)
{
    /* Create temporary texture */
    auto resolution = Resolution().Cast<int>();
    auto tex = RenderSystem::Active()->CreateTexture2D(Video::TextureFormats::RGBA, resolution);
    
    /* Copy framebuffer into texture */
    CopyTextureImage(tex.get(), { 0, 0, resolution.width, resolution.height });

    /* Read image data back from GPU to CPU */
    auto image = std::make_shared<Video::ImageUByte>(
        Video::ImageUByte::SizeType(resolution.width, resolution.height, 1), Video::ImageColorFormats::RGBA, false
    );
    RenderSystem::Active()->ReadTexture(tex.get(), *image);

    /*
    Flip image data vertical. This is required particular for OpenGL,
    since the texture's image data are stored from the left-bottom.
    Thus the image data must be fliped to be unique to Direct3D.
    This may be a bottle neck for real-time applications!
    */
    if (!QueryRendererDetails().hasOriginLeftTop)
        Video::ImageConverter::FlipImageY(image->RawBuffer(), image->GetSize(), image->NumColorComponents());

    /* Save image to file */
    return Video::ImageFileHandler::WriteImage(image, filename);
}


/*
 * ======= Protected: =======
 */

void RenderContext::CreateFrame(const std::wstring& title, void* parentFrame)
{
    /* Setup frame position and size */
    const Math::Size2i frameSize = videoMode_.resolution.Cast<int>();
    const Math::Point2i framePosition = GetFramePositionForVideoMode(videoMode_);

    /* Setup frame description */
    Platform::FrameDescription frameDesc;

    frameDesc.isBorderless = videoMode_.isFullscreen;

    /* Create final context frame */
    frame_ = Platform::Frame::Create(frameDesc, title, frameSize, framePosition, parentFrame);
}

Math::Point2i RenderContext::GetFramePositionForVideoMode(const Video::VideoMode& videoMode) const
{
    /* Setup frame position and size */
    Math::Point2i framePosition;

    /* Setup frame description */
    if (!videoMode.isFullscreen)
    {
        /*
        Setup approximated screen-centered frame position
        -> Ignoring border because of unknown size at this point
        */
        auto desktopResolution = Platform::Desktop::QueryResolution().Cast<int>();

        const Math::Size2i frameSize(videoMode.resolution.Cast<int>());

        framePosition.x = desktopResolution.width/2 - frameSize.width/2;
        framePosition.y = desktopResolution.height/2 - frameSize.height/2;
    }

    return framePosition;
}

void RenderContext::SetActiveRenderContext(RenderContext* context)
{
    activeRenderContext_ = context;
}

void RenderContext::CallFrameReCreateCallback()
{
    if (RenderContext::frameReCreateCallback_)
    {
        RenderContext::frameReCreateCallback_(
            RenderContext::frameReCreateUserData_, frame_->GetParentWindow()
        );
    }
}

void RenderContext::ChangeTextureMIPState(Texture* texture, bool hasMIPMaps)
{
    texture->hasMIPMaps = hasMIPMaps;
}


} // /namespace Video

} // /namespace Fork



// ========================