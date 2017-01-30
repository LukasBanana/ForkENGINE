/*
 * Render system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderSystemException.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Platform/Core/OS.h"
#include "Platform/Core/SystemInfo.h"
#include "Core/Version.h"
#include "Core/StaticConfig.h"

#include <algorithm>


namespace Fork
{

namespace Video
{


RenderSystem* RenderSystem::activeRenderSystem_ = nullptr;

RenderSystem::RenderSystem()
{
    RenderSystem::activeRenderSystem_ = this;
    
    /* Create common resource managers */
    textureMngr_ = std::make_unique<TextureManager>();
    shaderMngr_ = std::make_unique<ShaderManager>();
}
RenderSystem::~RenderSystem()
{
    /* Clean up objects in the correct order */
    textureMngr_                .reset();
    shaderMngr_                 .reset();

    defaultRasterizerState_     .reset();
    defaultDepthStencilState_   .reset();
    defaultBlendState_          .reset();

    emptyVertexBuffer_          .reset();

    /* Finally release render contexts */
    renderContexts_.clear();
}

RenderSystemPtr RenderSystem::Create(
    const Platform::DynamicLibraryPtr& library, bool createProfilerSystem)
{
    ASSERT_POINTER(library);

    const std::string procName = (createProfilerSystem ? "AllocProfilerRenderSystem" : "AllocRenderSystem");

    /* Load render system allocation procedure from library */
    DEFINE_PROC_INTERFACE(void*, PFNALLOCRENDERSYSTEMPROC, (void));

    auto AllocRenderSystem = reinterpret_cast<PFNALLOCRENDERSYSTEMPROC>(library->LoadProc(procName));
    if (!AllocRenderSystem)
    {
        throw RenderSystemException(
            "Procedure \"" + procName + "\" not found in render system library \"" + ToStr(library->GetName()) + "\""
        );
    }

    /* Allocate render system via library function */
    auto renderSystem = reinterpret_cast<RenderSystem*>(AllocRenderSystem());
    if (!renderSystem)
        throw RenderSystemException("Allocating render system from library \"" + ToStr(library->GetName()) + "\" failed"); 

    return std::shared_ptr<RenderSystem>(renderSystem);
}

RenderSystem* RenderSystem::Active()
{
    return activeRenderSystem_;
}

RenderSystem* RenderSystem::Active(bool isRequired)
{
    if (!activeRenderSystem_ && isRequired)
        throw RenderSystemException("RenderSystem is required but has not been created yet");
    return activeRenderSystem_;
}

bool RenderSystem::ReleaseRenderContext(const RenderContextPtr& renderContext)
{
    for (auto it = renderContexts_.begin(); it != renderContexts_.end(); ++it)
    {
        if (*it == renderContext)
        {
            /* Release render context by releaing it from the list */
            renderContexts_.erase(it);
            return true;
        }
    }
    return false;
}

/*RenderContext* RenderSystem::GetSharedRenderContext()
{
    return nullptr;
}*/

ScopedActiveRenderContext RenderSystem::ActivateSharedContext()
{
    return std::move(ScopedActiveRenderContext(GetSharedRenderContext()));
}

void RenderSystem::ResetProfiler()
{
    profilerModel_.Reset();
}

void RenderSystem::EnableDebugDump(bool enableDebugDump)
{
    // dummy
}

const Math::Matrix4f& RenderSystem::ProjectionMatrix(const Projectionf& projection) const
{
    return projection.GetMatrixLH();
}

/* --- Textures --- */

Texture1DPtr RenderSystem::CreateTexture1D(const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize)
{
    auto texture = CreateTexture1D();
    WriteTexture(texture.get(), textureFormat, textureSize, arraySize);
    return texture;
}

Texture2DPtr RenderSystem::CreateTexture2D(const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize)
{
    auto texture = CreateTexture2D();
    WriteTexture(texture.get(), textureFormat, textureSize, arraySize);
    return texture;
}

Texture3DPtr RenderSystem::CreateTexture3D(const TextureFormats textureFormat, const Texture3D::SizeType& textureSize)
{
    auto texture = CreateTexture3D();
    WriteTexture(texture.get(), textureFormat, textureSize);
    return texture;
}

TextureCubePtr RenderSystem::CreateTextureCube(const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize)
{
    auto texture = CreateTextureCube();
    WriteTexture(texture.get(), textureFormat, textureSize, arraySize);
    return texture;
}

void RenderSystem::WriteTexture(
    Texture1D* texture, const TextureFormats textureFormat,
    const Texture1D::SizeType& textureSize, unsigned int arraySize)
{
    WriteTexture(
        texture, textureFormat, textureSize, arraySize,
        ImageColorFormats::RGBA, RendererDataTypes::UByte, nullptr
    );
}

void RenderSystem::WriteTexture(Texture1D* texture, const ImageUByte& image)
{
    WriteTexture(
        texture,
        ChooseTextureFormat(image.GetFormat(), RendererDataTypes::UByte),
        static_cast<int>(image.GetSize().width),
        0,
        image.GetFormat(),
        RendererDataTypes::UByte,
        image.RawBuffer()
    );
}

void RenderSystem::WriteTexture(
    Texture2D* texture, const TextureFormats textureFormat,
    const Texture2D::SizeType& textureSize, unsigned int arraySize)
{
    WriteTexture(
        texture, textureFormat, textureSize, arraySize,
        ImageColorFormats::RGBA, RendererDataTypes::UByte, nullptr
    );
}

void RenderSystem::WriteTexture(Texture2D* texture, const ImageUByte& image)
{
    WriteTexture(
        texture,
        ChooseTextureFormat(image.GetFormat(), RendererDataTypes::UByte),
        image.GetSize().Sz2().Cast<int>(),
        0,
        image.GetFormat(),
        RendererDataTypes::UByte,
        image.RawBuffer()
    );
}

void RenderSystem::WriteTexture(
    Texture3D* texture, const TextureFormats textureFormat, const Texture3D::SizeType& textureSize)
{
    WriteTexture(
        texture, textureFormat, textureSize,
        ImageColorFormats::RGBA, RendererDataTypes::UByte, nullptr
    );
}

void RenderSystem::WriteTexture(Texture3D* texture, const ImageUByte& image)
{
    WriteTexture(
        texture,
        ChooseTextureFormat(image.GetFormat(), RendererDataTypes::UByte),
        image.GetSize().Cast<int>(),
        image.GetFormat(),
        RendererDataTypes::UByte,
        image.RawBuffer()
    );
}

void RenderSystem::WriteTexture(
    TextureCube* texture, const TextureFormats textureFormat,
    const TextureCube::SizeType& textureSize, unsigned int arraySize)
{
    WriteTexture(
        texture, textureFormat, textureSize, arraySize,
        ImageColorFormats::RGBA, RendererDataTypes::UByte, nullptr
    );
}

/* --- Render targets --- */

RenderTargetPtr RenderSystem::CreateRenderTarget(const std::vector<TexturePtr>& textures)
{
    auto renderTarget = CreateRenderTarget();
    renderTarget->Attach(textures);
    return renderTarget;
}

TextureCube::FaceRenderTargets RenderSystem::CreateCubeRenderTargets(const TextureCubePtr& textureCube)
{
    TextureCube::FaceRenderTargets renderTargets;

    for (size_t i = 0; i < 6; ++i)
    {
        renderTargets[i] = CreateRenderTarget();
        renderTargets[i]->Attach(textureCube, static_cast<Video::TextureCube::Faces>(i));
    }

    return renderTargets;
}

TextureCube::FaceRenderTargets RenderSystem::CreateCubeRenderTargets(const std::vector<TextureCubePtr>& textureCubes)
{
    TextureCube::FaceRenderTargets renderTargets;

    for (size_t i = 0, numTex = textureCubes.size(); i < 6; ++i)
    {
        renderTargets[i] = CreateRenderTarget();
        for (size_t j = 0; j < numTex; ++j)
        {
            renderTargets[i]->Attach(
                textureCubes[j],
                static_cast<Video::TextureCube::Faces>(i),
                static_cast<unsigned int>(j)
            );
        }
    }

    return renderTargets;
}

/* --- Shaders --- */

ConstantBufferPtr RenderSystem::GetRenderStateConstBuffer() const
{
    return nullptr;
}


/*
 * ======= Protected: =======
 */

void RenderSystem::ChangeBufferFormat(IndexBuffer* indexBuffer, const IndexFormat& format)
{
    indexBuffer->format = format;
}

void RenderSystem::ChangeBufferFormat(VertexBuffer* vertexBuffer, const VertexFormat& format)
{
    vertexBuffer->format = format;
}

void RenderSystem::ChangeTextureFormat(Texture* texture, TextureFormats format)
{
    texture->format = format;
}

void RenderSystem::CreateDefaultResources(const ContextDescription& contextDesc)
{
    /* Create default rasterizer state */
    RasterizerState::Description rasterizerDesc;
    {
        if (contextDesc.antiAliasingDesc.isEnabled)
        {
            /*
            Enable anti-aliasing only when a render-
            context with anti-aliasing support is created.
            */
            rasterizerDesc.isAntiAliasedLineEnable  = true;
            rasterizerDesc.isMultiSampleEnable      = true;
        }
    }
    defaultRasterizerState_ = CreateRasterizerState(rasterizerDesc);

    /* Create default depth-stencil- and blend states */
    defaultDepthStencilState_   = CreateDepthStencilState   ({});
    defaultBlendState_          = CreateBlendState          ({});

    /* Create an empty vertex buffer */
    emptyVertexBuffer_ = CreateVertexBuffer();
    WriteBuffer(emptyVertexBuffer_.get(), {}, nullptr, 0);

    /* Create shader provider */
    shaderProvider_ = CreateShaderProvider();
}

void RenderSystem::InitSharedRenderContext(RenderContext* sharedRenderContext)
{
    /* Store reference to shared render context */
    if (!sharedRenderContext_)
        sharedRenderContext_ = sharedRenderContext;
}

void RenderSystem::ValidateImageDataType(const RendererDataTypes imageDataType)
{
    if (imageDataType == RendererDataTypes::Double)
        throw InvalidArgumentException(__FUNCTION__, "imageDataType", "'Video::RendererDataTypes::Double' is not allowed for texture data");
}


} // /namespace Video

} // /namespace Fork



// ========================