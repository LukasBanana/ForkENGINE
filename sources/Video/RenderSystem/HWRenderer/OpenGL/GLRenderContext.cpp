/*
 * OpenGL render context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLRenderContext.h"
#include "GLParamMapper.h"
#include "GLCore.h"
#include "GLQuery.h"
#include "Core/CastToConcreteObject.h"

#include "HardwareBuffer/GLVertexBuffer.h"
#include "HardwareBuffer/GLIndexBuffer.h"

#include "RenderState/GLBlendState.h"
#include "RenderState/GLRasterizerState.h"
#include "RenderState/GLDepthStencilState.h"
#include "RenderState/GLSamplerState.h"
#include "RenderState/GLStateManager.h"

#include "Texture/GLTexture1D.h"
#include "Texture/GLTexture2D.h"
#include "Texture/GLTexture3D.h"
#include "Texture/GLTextureCube.h"
#include "Texture/GLRenderTarget.h"

#include "Shader/GLShaderComposition.h"
#include "HardwareBuffer/GLConstantBuffer.h"

#include "Video/RenderSystem/RenderSystem.h"
#include "IO/Core/Log.h"
#include "Platform/Core/Desktop.h"
#include "GLExtensions.h"

#include "Core/Exception/PointerConversionException.h"
#include "Core/Exception/InvalidStateException.h"
#include "Video/RenderSystem/RenderState/RenderStateException.h"

#ifdef FORK_DEBUG
#include <chrono>
#endif


namespace Fork
{

namespace Video
{


/* --- Internal members --- */

static const unsigned int maxNumTextureLayers = 32;
static const int maxNumDebugTrail = 10;

#define GLSTATE_ENABLE(s)       GLSTATEMNGR_ENABLE(*stateMngr_, s)
#define GLSTATE_DISABLE(s)      GLSTATEMNGR_DISABLE(*stateMngr_, s)
#define GLSTATE_CHANGE(s, v)    GLSTATEMNGR_CHANGE(*stateMngr_, s, v)

#define GLSTATE_PUSH(s, v)      GLSTATEMNGR_PUSH(*stateMngr_, s, v)


/*
 * GLRenderContext class
 */

GLRenderContext::GLRenderContext(
    GLRenderContext* sharedRenderContext,
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame) :
        RenderContext{ videoMode, contextDesc }
{
    /* Query a GL state manager */
    QueryStateManager(sharedRenderContext);

    /* Initialize multi-sample pixel format array */
    ClearMSPixelFormats();

    /* Create frame and render context */
    CreateFrame(frameTitle, parentFrame);
    CreateContext(sharedRenderContext);

    /* Setup initial configurations */
    SetupInitialConfig();
}
GLRenderContext::~GLRenderContext()
{
    /* Delete shared resources if this is the shared render context */
    if (IsSharedContext())
    {
        ScopedActiveRenderContext activeContext(this);
        DeleteSharedResources();
    }

    /* Delete render context */
    DeleteContext();

    /* Restore from fullscreen mode */
    if (videoMode_.isFullscreen)
        Platform::Desktop::ResetVideoMode();
}

/* --- Common --- */

Adapter GLRenderContext::QueryAdapter() const
{
    Adapter adapter;

    /* Check if "glGetString" returns a valud string -> otherwise render context is invalid */
    auto rendererStr = glGetString(GL_RENDERER);

    if (rendererStr)
    {
        /* Setup renderer info structure via "glGetString" */
        adapter.graphicsDeviceDesc  = ToStr(rendererStr);
        adapter.hardwareVendor      = ToStr(glGetString(GL_VENDOR));
        adapter.rendererVersion     = "OpenGL " + ToStr(glGetString(GL_VERSION));
        adapter.shaderModel         = ToStr(glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    return adapter;
}

RendererDetails GLRenderContext::QueryRendererDetails() const
{
    RendererDetails details;

    details.hasOriginLeftTop                = globalRenderState_.isInvScreen;
    details.hasLeftHandedCoordinateSystem   = false;

    return details;
}

RendererSupport GLRenderContext::QueryRendererSupport() const
{
    RendererSupport support;

    /* Query renderer support boolean attributes */
    support.hasRenderTargets        = (glGenFramebuffers        != nullptr);
    support.hasGeometryShaders      = (glProgramParameteri      != nullptr);
    support.hasTessellationShaders  = (glPatchParameteri        != nullptr);
    support.hasComputeShaders       = (glDispatchCompute        != nullptr);
    support.hasArrayTextures        = true;                                         //!!! <-- check for "GL_ARB_texture_cube_map_array"
    support.hasHardwareInstancing   = (glDrawArraysInstanced    != nullptr);
    support.hasViewportArray        = (glViewportArrayv         != nullptr);

    /* Query renderer support integral attributes */
    auto QueryAttrib = [](const GLenum param) -> unsigned int
    {
        GLint attr = 0;
        glGetIntegerv(param, &attr);
        return static_cast<unsigned int>(attr);
    };

    support.maxNumArrayTextureLayers        = QueryAttrib( GL_MAX_ARRAY_TEXTURE_LAYERS     );
    support.maxNumRenderTargetAttachments   = QueryAttrib( GL_MAX_DRAW_BUFFERS             );
    support.maxConstantBufferSize           = QueryAttrib( GL_MAX_UNIFORM_BLOCK_SIZE       );
    //support.maxComputeShaderWorkGroupNum    = QueryAttrib( GL_MAX_COMPUTE_WORK_GROUP_COUNT ); //! <-- 'glGetIntegeri_v' must be used
    //support.maxComputeShaderWorkGroupSize   = QueryAttrib( GL_MAX_COMPUTE_WORK_GROUP_SIZE  ); //! <-- 'glGetIntegeri_v' must be used
    support.maxAnisotropy                   = 16; //!!!

    /* Query maximum texture dimensions */
    GLint querySizeBase = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &querySizeBase);

    /* Generate proxy texture */
    GLuint proxyTex = 0;
    glGenTextures(1, &proxyTex);

    /* --- Query 1D texture max size --- */
    GLint querySize = querySizeBase;

    while (support.maxTexture1DSize == 0 && querySize > 0)
    {
        glTexImage1D(GL_PROXY_TEXTURE_1D, 0, GL_RGBA, querySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &support.maxTexture1DSize);
        querySize /= 2;
    }

    /* --- Query 2D texture max size --- */
    querySize = querySizeBase;

    while (support.maxTexture2DSize == 0 && querySize > 0)
    {
        glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_RGBA, querySize, querySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &support.maxTexture2DSize);
        querySize /= 2;
    }

    /* --- Query 3D texture max size --- */
    querySize = querySizeBase;

    while (support.maxTexture3DSize == 0 && querySize > 0)
    {
        glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_RGBA, querySize, querySize, querySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, GL_TEXTURE_WIDTH, &support.maxTexture3DSize);
        querySize /= 2;
    }

    /* --- Query 3D texture max size --- */
    querySize = querySizeBase;

    while (support.maxTextureCubeSize == 0 && querySize > 0)
    {
        glTexImage2D(GL_PROXY_TEXTURE_CUBE_MAP, 0, GL_RGBA, querySize, querySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glGetTexLevelParameteriv(GL_PROXY_TEXTURE_CUBE_MAP, 0, GL_TEXTURE_WIDTH, &support.maxTextureCubeSize);
        querySize /= 2;
    }

    /* Delete temporary proxy texture */
    glDeleteTextures(1, &proxyTex);

    /* Flush GL error state (ignore return value here) */
    glGetError();

    return support;
}

static bool HasBoundedTexture(GLenum target)
{
    GLint result = 0;
    glGetIntegerv(target, &result);
    return result != 0;
}

RenderStateReflection GLRenderContext::QueryRenderStateReflection() const
{
    RenderStateReflection rs;

    //todo...

    /* Query bounded texture layers */
    for (unsigned int i = 0; i < 32; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        for (GLenum target : { GL_TEXTURE_BINDING_1D,
                               GL_TEXTURE_BINDING_1D_ARRAY,
                               GL_TEXTURE_BINDING_2D,
                               GL_TEXTURE_BINDING_2D_ARRAY,
                               GL_TEXTURE_BINDING_2D_MULTISAMPLE,
                               GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,
                               GL_TEXTURE_BINDING_3D,
                               GL_TEXTURE_BINDING_BUFFER,
                               GL_TEXTURE_BINDING_CUBE_MAP,
                               GL_TEXTURE_BINDING_RECTANGLE })
        {
            if (HasBoundedTexture(target))
            {
                rs.boundedTextureLayers.push_back(i);
                break;
            }
        }
    }

    /* Query viewport states */
    GLint viewportState[4];
    glGetIntegerv(GL_VIEWPORT, viewportState);

    GLfloat depthRange[2];
    glGetFloatv(GL_DEPTH_RANGE, depthRange);

    Viewport viewport;
    {
        viewport.position   = { viewportState[0], viewportState[1] };
        viewport.size       = { viewportState[2], viewportState[3] };
        viewport.minDepth   = depthRange[0];
        viewport.maxDepth   = depthRange[1];
    }
    rs.viewports.push_back(viewport);

    /* Query scissor states */
    if (glIsEnabled(GL_SCISSOR_TEST))
    {
        GLint scissorState[4];
        glGetIntegerv(GL_SCISSOR_BOX, scissorState);

        Scissor scissor;
        {
            scissor.position    = { scissorState[0], scissorState[1] };
            scissor.size        = { scissorState[2], scissorState[3] };
        }
        rs.scissors.push_back(scissor);
    }

    return rs;
}

ShaderCode::Languages GLRenderContext::ShadingLanguage() const
{
    return ShaderCode::Languages::GLSL;
}

std::string GLRenderContext::ShadingLanguageFileExt() const
{
    return "glsl";
}

std::vector<Shader::Versions> GLRenderContext::ShaderModels() const
{
    std::vector<Shader::Versions> shaderModels;

    for (int i = static_cast<int>(Shader::Versions::GLSL_110); i <= static_cast<int>(Shader::Versions::GLSL_440); ++i)
        shaderModels.push_back(static_cast<Shader::Versions>(i));

    return shaderModels;
}

void GLRenderContext::SetupClearColor(const ColorRGBAf& color)
{
    Activate();
    RenderContext::SetupClearColor(color);
    glClearColor(clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a);
}

void GLRenderContext::SetupClearDepth(float depth)
{
    Activate();
    RenderContext::SetupClearDepth(depth);
    glClearDepth(depth);
}

void GLRenderContext::SetupClearStencil(unsigned char stencil)
{
    Activate();
    RenderContext::SetupClearStencil(stencil);
    glClearStencil(static_cast<GLint>(stencil));
}

void GLRenderContext::ClearBuffers(const ClearBuffersFlags::DataType flags)
{
    Activate();

    /* Setup bit mask for "glClear" function */
    GLbitfield mask = 0;

    if ((flags & ClearBuffersFlags::Color) != 0)
        mask |= GL_COLOR_BUFFER_BIT;
    if ((flags & ClearBuffersFlags::Depth) != 0)
        mask |= GL_DEPTH_BUFFER_BIT;
    if ((flags & ClearBuffersFlags::Stencil) != 0)
        mask |= GL_STENCIL_BUFFER_BIT;

    glClear(mask);
}

bool GLRenderContext::ChangeVideoMode(const VideoMode& videoMode)
{
    if (videoMode_ != videoMode)
    {
        /* Setup new description */
        auto frameDesc = frame_->GetDesc();
        
        frameDesc.isBorderless = videoMode.isFullscreen;
        if (videoMode.isFullscreen)
            frameDesc.isResizable = false;
        
        if (!frame_->GetParentWindow())
        {
            /* Update frame appearance */
            frame_->ChangeDesc(
                frameDesc,
                GetFramePositionForVideoMode(videoMode),
                videoMode.resolution.Cast<int>()
            );

            /* Change desktop video mode */
            if (!Platform::Desktop::ChangeVideoMode(videoMode))
                return false;
        }

        /* Store new settings */
        videoMode_ = videoMode;
    }
    return true;
}

bool GLRenderContext::ChangeDesc(const ContextDescription& contextDesc)
{
    bool result = true;

    /* Active this render context */
    auto activeContext = RenderContext::Active();
    Activate();

    /* Store new context description */
    contextDesc_ = contextDesc;
    
    /* Re-setup VSync interval */
    if (!SetupVSyncInterval())
        result = false;

    /* Reset previous active render context */
    activeContext->Activate();

    return result;
}

/* --- Render states --- */

void GLRenderContext::ChangeState(const RasterizerState* state)
{
    globalRenderState_.rasterizerState = state;

    /* Get GL rasterizer state */
    auto stateGL = CAST_TO_GL_OBJECT(RasterizerState const, state);

    const auto& desc = stateGL->GetDesc();

    /* Setup polygon mode, polygon offset and front facing mode */
    glPolygonMode(GL_FRONT_AND_BACK, stateGL->GetFillMode());
    //glPolygonOffset();
    SetupFrontFace(desc.isFrontCCW);

    /* Setup cull mode */
    if (desc.cullMode != RasterizerState::CullModes::None)
    {
        GLSTATE_ENABLE(CULL_FACE);
        glCullFace(stateGL->GetCullMode());
    }
    else
        GLSTATE_DISABLE(CULL_FACE);
    
    /* Setup GL boolean states */
    GLSTATE_CHANGE(DEPTH_CLAMP, !desc.isDepthClipEnabled);
    GLSTATE_CHANGE(SCISSOR_TEST, desc.isScissorTestEnabled);
    GLSTATE_CHANGE(MULTISAMPLE, desc.isMultiSampleEnable);
    GLSTATE_CHANGE(LINE_SMOOTH, desc.isAntiAliasedLineEnable);
}

void GLRenderContext::ChangeState(const DepthStencilState* state, unsigned int stencilRef)
{
    globalRenderState_.depthStencilState = state;

    /* Get GL depth-stencil state */
    auto stateGL = CAST_TO_GL_OBJECT(DepthStencilState const, state);

    const auto& desc = stateGL->GetDesc();

    /* Setup depth function */
    if (desc.isDepthTestEnabled)
    {
        GLSTATE_ENABLE(DEPTH_TEST);
        glDepthFunc(stateGL->GetDepthFunc());
    }
    else
        GLSTATE_DISABLE(DEPTH_TEST);

    glDepthMask(desc.isDepthWriteEnabled ? GL_TRUE : GL_FALSE);

    /* Setup stencil operations */
    if (desc.isStencilTestEnabled)
    {
        GLSTATE_ENABLE(STENCIL_TEST);

        //!TODO! -> better check for supported extensions !!!
        if (glStencilFuncSeparate && desc.isStencilSeparated)
        {
            /* Stencil operation <front face> */
            const auto& frontDesc = stateGL->GetFrontFace();
            glStencilFuncSeparate(GL_FRONT, frontDesc.stencilFunc, static_cast<GLint>(stencilRef), desc.stencilReadMask);
            glStencilOpSeparate(GL_FRONT, frontDesc.stencilFailOp, frontDesc.stencilZFailOp, frontDesc.stencilZPassOp);

            /* Stencil operation <back face> */
            const auto& backDesc = stateGL->GetBackFace();
            glStencilFuncSeparate(GL_BACK, backDesc.stencilFunc, static_cast<GLint>(stencilRef), desc.stencilReadMask);
            glStencilOpSeparate(GL_BACK, backDesc.stencilFailOp, backDesc.stencilZFailOp, backDesc.stencilZPassOp);

            glStencilMask(desc.stencilWriteMask);
        }
        else
        {
            /* Stencil operation <front and back face> */
            const auto& frontDesc = stateGL->GetFrontFace();
            glStencilFunc(frontDesc.stencilFunc, static_cast<GLint>(stencilRef), desc.stencilReadMask);
            glStencilMask(desc.stencilWriteMask);
            glStencilOp(frontDesc.stencilFailOp, frontDesc.stencilZFailOp, frontDesc.stencilZPassOp);
        }
    }
    else
        GLSTATE_DISABLE(STENCIL_TEST);
}

void GLRenderContext::ChangeState(const BlendState* state)
{
    globalRenderState_.blendState = state;

    /* Get GL blend state */
    auto stateGL = CAST_TO_GL_OBJECT(BlendState const, state);

    const auto& desc = stateGL->GetDesc();

    /* Setup blending operation */
    if (desc.isBlendEnabled)
    {
        GLSTATE_ENABLE(BLEND);
        //glBlendFunc(stateGL->GetSrcBlend(), stateGL->GetDestBlend());
        glBlendFuncSeparate(
            stateGL->GetSrcBlend(), stateGL->GetDestBlend(),
            stateGL->GetSrcBlendAlpha(), stateGL->GetDestBlendAlpha()
        );
    }
    else
        GLSTATE_DISABLE(BLEND);

    /* Setup color mask */
    glColorMask(
        GLBool(desc.colorMask.r),
        GLBool(desc.colorMask.g),
        GLBool(desc.colorMask.b),
        GLBool(desc.colorMask.a)
    );
}

void GLRenderContext::SetupPointSize(float size)
{
    RenderContext::SetupPointSize(size);
    glPointSize(size);
}

void GLRenderContext::SetupLineSize(float size)
{
    RenderContext::SetupLineSize(size);
    glLineWidth(size);
}

void GLRenderContext::SetupViewport(const Viewport& viewport)
{
    auto& viewportRS = globalRenderState_.viewport;

    /* Update viewport with current screen space */
    //if (viewportRS.position != viewport.position || viewportRS.size != viewport.size)
    {
        /* Store viewport rectangle and update render state */
        viewportRS.position = viewport.position;
        viewportRS.size     = viewport.size;
        UpdateViewport();
    }

    //if (viewportRS.minDepth != viewport.minDepth || viewportRS.maxDepth != viewport.maxDepth)
    {
        /* Store depth range */
        viewportRS.minDepth = viewport.minDepth;
        viewportRS.maxDepth = viewport.maxDepth;

        /*
        Also set the depth range for D3D compatibility.
        -> In D3D11 a viewport always consists of these 6 data fields (x, y, width, height, near, far).
        */
        glDepthRange(viewportRS.minDepth, viewportRS.maxDepth);
    }
}

void GLRenderContext::SetupMultiViewport(const std::vector<Viewport>& viewports)
{
    //todo...
}

void GLRenderContext::SetupScissor(const Scissor& scissor)
{
    //if (globalRenderState_.scissor != scissor)
    {
        /* Store scissor and update render state */
        globalRenderState_.scissor = scissor;
        UpdateScissor();
    }
}

void GLRenderContext::SetupMultiScissor(const std::vector<Scissor>& scissors)
{
    //todo...
}

void GLRenderContext::SetupProjectionMatrix(const Math::Matrix4f& projectionMatrix)
{
    /* Store projection matrix */
    auto& proj = globalRenderState_.projectionMatrix;
    proj = projectionMatrix;

    /* Extract near- and far clipping planes */
    Projectionf::ExtractClippingPlanesRH(
        projectionMatrix, globalRenderState_.nearPlane, globalRenderState_.farPlane
    );

    if (globalRenderState_.isInvScreen)
    {
        /* Invert Y-axis in projection matrix when the screen in inversed. */
        proj.FlipYAxis();
    }
}

void GLRenderContext::SetupProjectionMatrix(const Projectionf& projection)
{
    #ifdef FORK_DEBUG
    if (!projection.IsValid())
        IO::Log::Debug(__FUNCTION__, "Invalid projection", maxNumDebugTrail);
    #endif

    /* Store projection matrix */
    auto& proj = globalRenderState_.projectionMatrix;
    proj = projection.GetMatrixRH();

    /* Store near- and far clipping planes */
    globalRenderState_.nearPlane = projection.GetNearPlane();
    globalRenderState_.farPlane = projection.GetFarPlane();

    if (globalRenderState_.isInvScreen)
    {
        /* Invert Y-axis in projection matrix when the screen in inversed. */
        proj.FlipYAxis();
    }
}

void GLRenderContext::SetupViewMatrix(const Math::Matrix4f& viewMatrix)
{
    globalRenderState_.cameraMatrix = viewMatrix.Inverse();

    auto& view = globalRenderState_.viewMatrix;

    view = viewMatrix;

    /* Invert view matrix to convert from left-handed to right-handed coordinate system. */
    view.FlipZAxis();
}

void GLRenderContext::SetupDrawMode(const GeometryPrimitives type)
{
    globalRenderState_.drawPrimitive = type;
    globalGLRenderState_.drawMode = GLParamMapper::Map(type);
}

void GLRenderContext::BindSamplerState(const SamplerState* state, unsigned int layer)
{
    /* Bind sampler */
    auto stateGL = CAST_TO_GL_OBJECT(SamplerState const, state);
    stateGL->Bind(layer);
}

void GLRenderContext::UnbindSamplerState(unsigned int layer)
{
    /* Unbind sampler */
    glBindSampler(layer, 0);
}

/* --- Hardware buffers --- */

void GLRenderContext::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    /* Bind vertex buffer */
    auto vertexBufferGL = CAST_TO_GL_OBJECT(VertexBuffer, vertexBuffer);
    
    #ifdef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_
    UnbindVertexBuffer();
    globalGLRenderState_.vertexBuffer = vertexBufferGL;
    vertexBufferGL->BindFormatted();
    #else
    vertexBufferGL->BindFormatted();
    #endif
}

void GLRenderContext::UnbindVertexBuffer()
{
    #ifdef _DEB_CUSTOM_VERTEX_FORMAT_BINDING_
    /* Unbind vertex buffer */
    if (globalGLRenderState_.vertexBuffer)
    {
        globalGLRenderState_.vertexBuffer->UnbindFormatted();
        globalGLRenderState_.vertexBuffer = nullptr;
    }
    #else
    stateMngr_->BindVertexArray(0);
    #endif
}

void GLRenderContext::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    /* Bind index buffer */
    auto indexBufferGL = CAST_TO_GL_OBJECT(IndexBuffer, indexBuffer);
    indexBufferGL->Bind();

    /* Setup new global render states */
    globalGLRenderState_.indexBufferType = GLParamMapper::Map(indexBuffer->GetFormat().GetDataType());
}

void GLRenderContext::UnbindIndexBuffer()
{
    /* Unbind index buffer */
    stateMngr_->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/* --- Textures --- */

void GLRenderContext::BindTexture(const Texture* texture, unsigned int layer)
{
    /* Bind texture to layer */
    if (layer < maxNumTextureLayers)
    {
        //auto textureGL = CastToRendererObject<const GLTextureBase>(texture, __FUNCTION__, "Texture", "GLTextureBase");
        auto textureGL = dynamic_cast<const GLTextureBase*>(texture);
        if (!textureGL)
            throw PointerConversionException(__FUNCTION__, "Texture", "GLTextureBase");
        textureGL->Bind(layer);
    }
}

void GLRenderContext::UnbindTexture(const Texture* texture, unsigned int layer)
{
    /* Unbind texture from layer */
    if (layer < maxNumTextureLayers)
    {
        auto textureGL = dynamic_cast<const GLTextureBase*>(texture);
        if (!textureGL)
            throw PointerConversionException(__FUNCTION__, "Texture", "GLTextureBase");
        textureGL->Unbind(layer);
    }
}

void GLRenderContext::CopyTextureImage(
    Texture2D* texture, const Math::Recti& sourceArea,
    const Math::Point2i& destOffset, unsigned int mipLevel)
{
    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(Texture2D, texture);
    
    #ifdef FORK_DEBUG
    if (GetRenderState().renderTarget != GetMainRenderTarget())
        IO::Log::Debug(__FUNCTION__, "Copying image data from main frame buffer while a render-target is bound", maxNumDebugTrail);
    #endif

    /* Copy frame buffer data to the texture */
    textureGL->Bind();

    FLUSH_GL_ERROR;

    glCopyTexSubImage2D(
        textureGL->GetTarget(),
        static_cast<GLint>(mipLevel),
        destOffset.x,
        destOffset.y,
        sourceArea.left,
        ScreenHeight() - sourceArea.Height() - sourceArea.top,
        sourceArea.Width(),
        sourceArea.Height()
    );

    SHOW_GL_ERROR("glCopyTexSubImage2D");

    textureGL->Unbind();

    /* Flip image data on Y-axis */
    //todo ...
}

void GLRenderContext::CopyTextureImage(
    Texture2D* texture, const RenderTarget* renderTarget, const Math::Recti& sourceArea,
    const Math::Point2i& destOffset, unsigned int mipLevel)
{
    /* Get GL texture and render target */
    auto textureGL = CAST_TO_GL_OBJECT(Texture2D, texture);
    auto renderTargetGL = CAST_TO_GL_OBJECT(RenderTarget const, renderTarget);
    
    #ifdef FORK_DEBUG
    if (GetRenderState().renderTarget != GetMainRenderTarget())
        IO::Log::Debug(__FUNCTION__, "Copying image data from render-target while other render-target is bound", maxNumDebugTrail);
    #endif

    /* Copy frame buffer data to the texture */
    textureGL->Bind();
    renderTargetGL->GetFrameBuffer()->Bind();

    FLUSH_GL_ERROR;

    glCopyTexSubImage2D(
        textureGL->GetTarget(),
        static_cast<GLint>(mipLevel),
        destOffset.x,
        destOffset.y,
        sourceArea.left,
        sourceArea.top,
        sourceArea.Width(),
        sourceArea.Height()
    );

    SHOW_GL_ERROR("glCopyTexSubImage2D");

    renderTargetGL->GetFrameBuffer()->Unbind();
    textureGL->Unbind();
}

/*
This function generates all MIP-maps for the specified texture
in the default process (using "glGenerateMipmap").
*/
void GLRenderContext::GenerateMIPMaps(Texture* texture)
{
    /* Get GL texture */
    //auto textureGL = CastToRendererObject<GLTextureBase>(texture, __FUNCTION__, "Texture", "GLTextureBase");
    auto textureGL = dynamic_cast<GLTextureBase*>(texture);
    if (!textureGL)
        throw PointerConversionException(__FUNCTION__, "Texture", "GLTextureBase");

    /* Generate MIP-maps */
    const auto target = textureGL->GetTarget();

    glBindTexture(target, textureGL->GetTextureID());
    {
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    glBindTexture(target, 0);

    /* Store information that this texture has now MIP-maps */
    ChangeTextureMIPState(texture, true);
}

/*
This functions generates all MIP-maps for the specified 2D texture with a custom shader.
The shared intermediate framebuffer object is used to attach each MIP-map level as render target.
A fullscreen triangle is renderd with the specified shader.
While rendering into a specific MIP-map level the next upper MIP-map is bound as texture sampler.
This allows parallel reading and writing into the same texture.
(This is used to generate Hi-Z maps for instance)
*/
void GLRenderContext::GenerateMIPMaps(
    Texture2D* destTexture, const Texture2D* sourceTexture, const ShaderComposition* shader,
    unsigned int layer, const GenerateMIPMapsShaderCallback& shaderCallback)
{
    /* Get GL texture and shader */
    auto destTextureGL  = CAST_TO_GL_OBJECT(Texture2D, destTexture);
    auto srcTextureGL   = CAST_TO_GL_OBJECT(Texture2D const, sourceTexture);
    auto shaderGL       = CAST_TO_GL_OBJECT(ShaderComposition const, shader);

    if (destTextureGL->GetTarget() != srcTextureGL->GetTarget())
    {
        throw InvalidArgumentException(
            __FUNCTION__, "sourceTexture",
            "Target mismatch or source- and destination textures"
        );
    }

    /* Get texture resource data */
    const auto target           = destTextureGL->GetTarget();
    const auto destTextureID    = destTextureGL->GetTextureID();
    const auto srcTextureID     = srcTextureGL->GetTextureID();

    /* Bind resources: texture, framebuffer, empty vertex buffer, shader */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(target, destTextureID);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFramebuffer);

    BindVertexBuffer(RenderSystem::Active()->GetEmptyVertexBuffer());
    BindShader(shader);
    
    /* Check if MIP chain has already been allocated */
    if (!destTexture->HasMIPMaps())
    {
        /* Allocate MIP chain and store information that this texture has now MIP-maps */
        glGenerateMipmap(target);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        ChangeTextureMIPState(destTexture, true);
    }

    /* Store previous texture parameters */
    if (destTextureID != srcTextureID)
        glBindTexture(target, srcTextureID);

    #if 0//!!!
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    #endif

    GLint prevBaseLevel = 0;
    glGetTexParameteriv(target, GL_TEXTURE_BASE_LEVEL, &prevBaseLevel);

    GLint prevMaxLevel = 1000;
    glGetTexParameteriv(target, GL_TEXTURE_MAX_LEVEL, &prevMaxLevel);

    /* Push render states */
    GLSTATE_PUSH( DEPTH_TEST, false );
    GLSTATE_PUSH( BLEND,      false );
    GLSTATE_PUSH( CULL_FACE,  false );

    /* Map texture layer to GL value */
    const GLint texLayer = (destTexture->HasArray() && layer < destTexture->GetArraySize() ? static_cast<GLint>(layer) : -1);

    /* Map texture parameters to GL values */
    auto size = destTexture->GetSize();
    GLsizei width = size.width;
    GLsizei height = size.height;
    GLint mipLevel = 0;

    /* Iterate over each MIP level (except the first one) */
    while (width > 1 || height > 1)
    {
        /* Increase MIP level by halving width and height */
        if (width > 1)
            width /= 2;
        if (height > 1)
            height /= 2;
        ++mipLevel;

        /* Attach current MIP-map level which is to be rendered */
        if (texLayer >= 0)
            glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, destTextureID, mipLevel, texLayer);
        else
            glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, destTextureID, mipLevel);

        /*
        Setup base MIP-map level to sample from the texture
        (one higher than the currently attached MIP map)
        */
        glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, mipLevel - 1);
        glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, mipLevel - 1);

        /* Emit shader callback */
        if (shaderCallback)
            shaderCallback({ width, height }, static_cast<unsigned int>(mipLevel));

        /* Render current MIP map */
        glViewport(0, 0, width, height);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    }

    /* Reset previous texture parameters */
    glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, prevBaseLevel);
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, prevMaxLevel);

    /* Reset render states */
    stateMngr_->Pop(3);
    stateMngr_->InvalidateViewport();
    UpdateViewport();

    /* Unbind resources */
    UnbindShader();
    UnbindVertexBuffer();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(target, 0);
}

/* --- Render targets --- */

void GLRenderContext::BindRenderTarget(RenderTarget* renderTarget)
{
    if (renderTarget == GetRenderState().renderTarget)
        return;

    /* Get GL render target */
    GLRenderTarget* renderTargetGL = nullptr;
    if (renderTarget)
        renderTargetGL = CAST_TO_GL_OBJECT(RenderTarget, renderTarget);

    /* Unbind previous render target */
    if (globalGLRenderState_.renderTarget)
    {
        /* Check if frame buffers must be blitted when multi-sampling is used */
        if (globalGLRenderState_.renderTarget->HasMultiSampling())
            globalGLRenderState_.renderTarget->BlitMultiSampleFrameBuffers();
    }
    
    if (renderTargetGL)
    {
        /* Bind new render target */
        if (renderTargetGL->HasMultiSampling() && renderTargetGL->GetFrameBufferMS())
            renderTargetGL->GetFrameBufferMS()->Bind();
        else
            renderTargetGL->GetFrameBuffer()->Bind();
        
        /*
        Enable inverse-screen
        -> While drawing into a render-target, we want to
        inverse the OpenGL coordinate system Y-axis.
        */
        InverseScreen(true);
    }
    else
    {
        /* Bind main frame buffer */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /*
        Disable inverse-screen
        -> Now we continue to draw with the default OpenGL coordinate system.
        */
        InverseScreen(false);
    }

    /* Store new active render target */
    globalRenderState_.renderTarget     = renderTarget;
    globalGLRenderState_.renderTarget   = renderTargetGL;
}

/* --- Drawing --- */

//#define __DEB_ASSERT_BUFFER_BINDING__
#ifdef __DEB_ASSERT_BUFFER_BINDING__

static void AssertBoundVertexBuffer(const char* procName)
{
    GLint id = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
    if (!id)
        throw RenderStateException(procName, "No vertex buffer bound");
}

static void AssertBoundIndexBuffer(const char* procName)
{
    GLint id = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
    if (!id)
        throw RenderStateException(procName, "No index buffer bound");
}

#endif

void GLRenderContext::Draw(unsigned int numVertices, unsigned int firstVertex)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    #endif

    glDrawArrays(
        globalGLRenderState_.drawMode,
        static_cast<GLint>(firstVertex),
        static_cast<GLsizei>(numVertices)
    );
}

void GLRenderContext::DrawIndexed(unsigned int numVertices)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    AssertBoundIndexBuffer(__FUNCTION__);
    #endif

    glDrawElements(
        globalGLRenderState_.drawMode,
        static_cast<GLsizei>(numVertices),
        globalGLRenderState_.indexBufferType,
        nullptr
    );
}

void GLRenderContext::DrawIndexed(unsigned int numVertices, int indexOffset)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    AssertBoundIndexBuffer(__FUNCTION__);
    #endif

    glDrawElementsBaseVertex(
        globalGLRenderState_.drawMode,
        static_cast<GLsizei>(numVertices),
        globalGLRenderState_.indexBufferType,
        nullptr,
        indexOffset
    );
}

void GLRenderContext::DrawInstanced(
    unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    #endif

    glDrawArraysInstanced(
        globalGLRenderState_.drawMode,
        static_cast<GLint>(firstVertex),
        static_cast<GLsizei>(numVertices),
        static_cast<GLsizei>(numInstances)
    );
}

void GLRenderContext::DrawInstanced(
    unsigned int numVertices, unsigned int firstVertex,
    unsigned int numInstances, unsigned int instanceOffset)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    #endif

    glDrawArraysInstancedBaseInstance(
        globalGLRenderState_.drawMode,
        static_cast<GLint>(firstVertex),
        static_cast<GLsizei>(numVertices),
        static_cast<GLsizei>(numInstances),
        instanceOffset
    );
}

void GLRenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    AssertBoundIndexBuffer(__FUNCTION__);
    #endif

    glDrawElementsInstanced(
        globalGLRenderState_.drawMode,
        static_cast<GLsizei>(numVertices),
        globalGLRenderState_.indexBufferType,
        nullptr,
        static_cast<GLsizei>(numInstances)
    );
}

void GLRenderContext::DrawInstancedIndexed(
    unsigned int numVertices, unsigned int numInstances, int indexOffset)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    AssertBoundIndexBuffer(__FUNCTION__);
    #endif

    glDrawElementsInstancedBaseVertex(
        globalGLRenderState_.drawMode,
        static_cast<GLsizei>(numVertices),
        globalGLRenderState_.indexBufferType,
        nullptr,
        static_cast<GLsizei>(numInstances),
        indexOffset
    );
}

void GLRenderContext::DrawInstancedIndexed(
    unsigned int numVertices, unsigned int numInstances,
    int indexOffset, unsigned int instanceOffset)
{
    #ifdef __DEB_ASSERT_BUFFER_BINDING__
    AssertBoundVertexBuffer(__FUNCTION__);
    AssertBoundIndexBuffer(__FUNCTION__);
    #endif

    glDrawElementsInstancedBaseVertexBaseInstance(
        globalGLRenderState_.drawMode,
        static_cast<GLsizei>(numVertices),
        globalGLRenderState_.indexBufferType,
        nullptr,
        static_cast<GLsizei>(numInstances),
        indexOffset,
        instanceOffset
    );
}

/* --- Shaders --- */

void GLRenderContext::BindShader(const ShaderComposition* shaderComposition)
{
    /* Check if shader composition is already bound */
    if (globalRenderState_.shaderComposition == shaderComposition)
        return;

    globalRenderState_.shaderComposition = shaderComposition;

    if (!shaderComposition->HasCompiledSuccessful())
    {
        #ifdef FORK_DEBUG
        IO::Log::Debug(__FUNCTION__, "Binding shader which was not successfully compiled", maxNumDebugTrail);
        #endif
        return;
    }

    /* Get GL shader composition */
    auto shaderCompositionGL = CAST_TO_GL_OBJECT(ShaderComposition const, shaderComposition);

    /* Bind GL program */
    glUseProgram(shaderCompositionGL->GetProgramID());

    /* Bind all uniform buffers */
    for (const auto& bufferMetaData : shaderCompositionGL->GetConstantBufferMetaData())
    {
        stateMngr_->BindBufferBase(
            GL_UNIFORM_BUFFER,
            bufferMetaData.bindIndex,
            bufferMetaData.bufferID
        );
    }

    /* Bind all storage buffers */
    for (const auto& bufferMetaData : shaderCompositionGL->GetStorageBufferMetaData())
    {
        stateMngr_->BindBufferBase(
            GL_SHADER_STORAGE_BUFFER,
            bufferMetaData.bindIndex,
            bufferMetaData.bufferID
        );
    }
}

void GLRenderContext::UnbindShader()
{
    if (globalRenderState_.shaderComposition)
    {
        /* Unbind GL program */
        glUseProgram(0);
        globalRenderState_.shaderComposition = nullptr;
    }
}

void GLRenderContext::Dispatch(const Math::Size3ui& numThreadGroups)
{
    glDispatchCompute(numThreadGroups.width, numThreadGroups.height, numThreadGroups.depth);
}

/* --- Query --- */

void GLRenderContext::BeginQuery(Query* query)
{
    /* Get GL query object */
    auto queryGL = CAST_TO_GL_OBJECT(Query, query);

    /* Check query state */
    if ((queryStateFlags_ & Query::StateFlags::ReadyToBegin) == 0)
        throw InvalidStateException(__FUNCTION__, "Query object not ready to begin");

    /* Begin with query */
    glBeginQuery(queryGL->GetTarget(), queryGL->GetObjectID());

    /* Change query state from 'read to begin' to 'waiting for end' */
    queryStateFlags_ = Query::StateFlags::WaitingForEnd;
}

void GLRenderContext::EndQuery(Query* query)
{
    /* Get GL query object */
    auto queryGL = CAST_TO_GL_OBJECT(Query, query);

    /* Check query state */
    if ((queryStateFlags_ & Query::StateFlags::WaitingForEnd) == 0)
        throw InvalidStateException(__FUNCTION__, "Query object not waiting for end");

    /* End with query */
    glEndQuery(queryGL->GetTarget());

    /* Change query state to 'read to begin' and 'result available' */
    queryStateFlags_ = (Query::StateFlags::ReadyToBegin | Query::StateFlags::ResultAvailable);
}

unsigned int GLRenderContext::QueryResult(Query* query)
{
    /* Get GL query object */
    auto queryGL = CAST_TO_GL_OBJECT(Query, query);

    /* Check query state */
    if ((queryStateFlags_ & Query::StateFlags::ResultAvailable) == 0)
        throw InvalidStateException(__FUNCTION__, "Query object has no available result");

    /* Flush command buffer */
    glFlush();
    
    /* Wait until the result is available */
    GLint available = 0;
    
    #ifdef FORK_DEBUG
    auto startDeadline = std::chrono::system_clock::now();
    #endif

    while (!available)
    {
        #ifdef FORK_DEBUG
        /* Check if query takes too long */
        auto timeDeadline = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(timeDeadline - startDeadline).count() >= 3)
            throw InvalidStateException(__FUNCTION__, "Deadlock caused by query object");
        #endif

        /* Query result from GL query object */
        glGetQueryObjectiv(queryGL->GetObjectID(), GL_QUERY_RESULT_AVAILABLE, &available);
    }
    
    /* Return result from the GL query object */
    GLuint result = 0;
    glGetQueryObjectuiv(queryGL->GetObjectID(), GL_QUERY_RESULT, &result);

    return result;
}


/*
 * ======= Protected: =======
 */

void GLRenderContext::InitRenderStateConstBuffer(GLConstantBuffer* constBuffer)
{
    renderStateConstBuffer_ = constBuffer;
}


/*
 * ======= Private: =======
 */

void GLRenderContext::QueryStateManager(GLRenderContext* sharedRenderContext)
{
    if (sharedRenderContext)
    {
        /* Share state manager with shared render context */
        stateMngr_ = sharedRenderContext->stateMngr_;
    }
    else
    {
        /* Create a new shared state manager */
        stateMngr_ = std::make_shared<GLStateManager>();
    }
}

void GLRenderContext::SetupInitialConfig()
{
    /* Setup initial GL configuration */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // (D3D10+ has this per default)

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    /*
    Set pixel storage to byte-alignment (default is word-alignment).
    This is required so that texture formats like RGB (which is not word-aligned) can be used.
    */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Initialize GL state manager */
    stateMngr_->InitStates();
}

void GLRenderContext::ClearMSPixelFormats()
{
    #if defined(FORK_WINDOWS_PLATFORM)
    numMSPixelFormats_ = 0;
    std::fill(multiSamplePixelFormats_, multiSamplePixelFormats_ + GLRenderContext::numMaxMSPixelFormats, 0);
    #endif
}

void GLRenderContext::QueryGLVersion()
{
    /* Query GL version major- and minor numbers */
    glGetIntegerv(GL_MAJOR_VERSION, &versionGL_.major);
    glGetIntegerv(GL_MINOR_VERSION, &versionGL_.minor);
}

/*
This function manages internal states, to easily invert the Z+ axis.
This is due to the right-handed coordinate system of OpenGL (where Z+ points out of the screen),
since the engine uses a left-handed coordinate system (where Z+ points into the screen).
Also the screen origin in OpenGL is on the lower-left, but the engine defines the screen origin
to be on the upper-left (like in DirectX).
This function makes a work-around to be compatible between OpenGL and Direct3D renderers.
It is primarily called inside the "BindRenderTarget" function.
*/
void GLRenderContext::InverseScreen(bool enable)
{
    if (globalRenderState_.isInvScreen != enable)
    {
        /* Set new inverse-screen mode and update front face */
        globalRenderState_.isInvScreen = enable;

        /* Update screen space */
        SetupFrontFace(globalGLRenderState_.isFrontCCW);
        UpdateViewport();
        UpdateScissor();
        globalRenderState_.projectionMatrix.FlipYAxis();

        /* Update inverse-screen constant buffer */
        FLUSH_GL_ERROR;

        const Math::Vector4i bufferParam { (globalRenderState_.isInvScreen ? 1 : 0), 0, 0, 0 };
        renderStateConstBuffer_->Bind();
        renderStateConstBuffer_->WriteSub(&bufferParam, sizeof(bufferParam), 0);

        SHOW_GL_ERROR("GLRenderContext::InverseScreen/RenderStateConstantBuffer");
    }
}

void GLRenderContext::SetupFrontFace(bool isFrontCCW)
{
    if (globalRenderState_.isInvScreen)
        glFrontFace(isFrontCCW ? GL_CW : GL_CCW);
    else
        glFrontFace(isFrontCCW ? GL_CCW : GL_CW);
    globalGLRenderState_.isFrontCCW = isFrontCCW;
}

/*
When "isInvScreen" is enabled, the viewport will be set up in the normal way.
This is due to the Y-fliped OpenGL coordinate system. The mechanism of supporting
both normal and inverted screen is used for compatibility between OpenGL and Direct3D.
When rendering into a texture, the normal way is used (isInvScreen = true), otherwise
invert the Y axis (isInvScreen = false).
*/
void GLRenderContext::UpdateViewport()
{
    const auto& viewport = globalRenderState_.viewport;

    if (globalRenderState_.isInvScreen)
    {
        /* Setup viewport in normal way */
        stateMngr_->SetupViewport(
            viewport.position.x,
            viewport.position.y,
            viewport.size.width,
            viewport.size.height
        );
    }
    else
    {
        /* Setup viewport with inverted Y axis */
        stateMngr_->SetupViewport(
            viewport.position.x,
            ScreenHeight() - viewport.size.height - viewport.position.y,
            viewport.size.width,
            viewport.size.height
        );
    }
}

void GLRenderContext::UpdateScissor()
{
    const auto& scissor = globalRenderState_.scissor;

    if (globalRenderState_.isInvScreen)
    {
        /* Setup scissor in normal way */
        stateMngr_->SetupScissor(
            scissor.position.x,
            scissor.position.y,
            scissor.size.width,
            scissor.size.height
        );
    }
    else
    {
        /* Setup scissor with inverted Y axis */
        stateMngr_->SetupScissor(
            scissor.position.x,
            ScreenHeight() - scissor.size.height - scissor.position.y,
            scissor.size.width,
            scissor.size.height
        );
    }
}


/*
 * GLVersion structure
 */

GLRenderContext::GLVersion::GLVersion(int ma, int mi) :
    major{ ma },
    minor{ mi }
{
}


} // /namespace Video

} // /namespace Fork



// ========================