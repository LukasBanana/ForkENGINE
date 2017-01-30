/*
 * Direct3D 11 render system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11RenderSystem.h"
#include "D3D11RenderContext.h"
#include "D3D11Core.h"
#include "D3D11ParamMapper.h"
#include "D3D11FeatureSupport.h"
#include "D3D11Query.h"
#include "Core/CastToConcreteObject.h"
#include "Core/STLHelper.h"

#include "HardwareBuffer/D3D11VertexBuffer.h"
#include "HardwareBuffer/D3D11IndexBuffer.h"
#include "HardwareBuffer/D3D11ConstantBuffer.h"
#include "HardwareBuffer/D3D11StorageBuffer.h"

#include "RenderState/D3D11BlendState.h"
#include "RenderState/D3D11RasterizerState.h"
#include "RenderState/D3D11DepthStencilState.h"
#include "RenderState/D3D11SamplerState.h"

#include "Shader/D3D11VertexShader.h"
#include "Shader/D3D11PixelShader.h"
#include "Shader/D3D11GeometryShader.h"
#include "Shader/D3D11TessControlShader.h"
#include "Shader/D3D11TessEvaluationShader.h"
#include "Shader/D3D11ComputeShader.h"
#include "Shader/D3D11ShaderComposition.h"
#include "Shader/D3D11ShaderProvider.h"

#include "Texture/D3D11Texture1D.h"
#include "Texture/D3D11Texture2D.h"
#include "Texture/D3D11Texture3D.h"
#include "Texture/D3D11TextureCube.h"
#include "Texture/D3D11RenderTarget.h"

#include "Core/StringModifier.h"
#include "Core/Exception/PointerConversionException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Core/Exception/NullPointerException.h"
#include "Video/RenderSystem/RenderSystemException.h"
#include "Video/Monitor/VendorIDs.h"

#include "IO/Core/Log.h"
#include "Video/RenderSystem/ScopedActiveRenderContext.h"
#include "Math/Common/ExtMathFunctions.h"


namespace Fork
{

namespace Video
{


D3D11RenderSystem::D3D11RenderSystem()
{
    try
    {
        device_         = D3D11RenderSystem::CreateDevice(nullptr, &featureLevel_, nullptr, context_);
        factory_        = D3D11RenderSystem::QueryFactory(device_);
        videoAdapters_  = D3D11RenderSystem::QueryVideoAdapters(factory_, featureLevel_);
    }
    catch (const std::exception& err)
    {
        throw RenderSystemException(err.what());
    }
}
D3D11RenderSystem::~D3D11RenderSystem()
{
    DXSafeRelease( context_ );
    DXSafeRelease( factory_ );
    DXSafeRelease( device_  );
}

RenderContextPtr D3D11RenderSystem::CreateRenderContext(
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame)
{
    /* Create Direct3D 11 render context */
    return InitRenderContext(
        std::make_shared<D3D11RenderContext>(
            this, context_,
            videoMode, contextDesc,
            frameTitle, parentFrame
        ),
        videoMode, contextDesc
    );
    return nullptr;
}

/* --- Render states --- */

RasterizerStatePtr D3D11RenderSystem::CreateRasterizerState(const RasterizerState::Description& desc)
{
    return std::make_shared<D3D11RasterizerState>(device_, desc);
}

DepthStencilStatePtr D3D11RenderSystem::CreateDepthStencilState(const DepthStencilState::Description& desc)
{
    return std::make_shared<D3D11DepthStencilState>(device_, desc);
}

BlendStatePtr D3D11RenderSystem::CreateBlendState(const BlendState::Description& desc)
{
    return std::make_shared<D3D11BlendState>(device_, desc);
}

SamplerStatePtr D3D11RenderSystem::CreateSamplerState(const SamplerState::Description& desc)
{
    return std::make_shared<D3D11SamplerState>(device_, desc);
}

void D3D11RenderSystem::ChangeStateDesc(RasterizerState* state, const RasterizerState::Description& desc)
{
    auto stateD3D = CAST_TO_D3D11_OBJECT(RasterizerState, state);
    stateD3D->ChangeDesc(device_, desc);
}

void D3D11RenderSystem::ChangeStateDesc(DepthStencilState* state, const DepthStencilState::Description& desc)
{
    auto stateD3D = CAST_TO_D3D11_OBJECT(DepthStencilState, state);
    stateD3D->ChangeDesc(device_, desc);
}

void D3D11RenderSystem::ChangeStateDesc(BlendState* state, const BlendState::Description& desc)
{
    auto stateD3D = CAST_TO_D3D11_OBJECT(BlendState, state);
    stateD3D->ChangeDesc(device_, desc);
}

void D3D11RenderSystem::ChangeStateDesc(SamplerState* state, const SamplerState::Description& desc)
{
    auto stateD3D = CAST_TO_D3D11_OBJECT(SamplerState, state);
    stateD3D->ChangeDesc(device_, desc);
}

/* --- Hardware buffers --- */

VertexBufferPtr D3D11RenderSystem::CreateVertexBuffer()
{
    return std::make_shared<D3D11VertexBuffer>();
}

IndexBufferPtr D3D11RenderSystem::CreateIndexBuffer()
{
    return std::make_shared<D3D11IndexBuffer>();
}

ConstantBufferPtr D3D11RenderSystem::CreateConstantBuffer(const ConstantBuffer::Description& desc)
{
    return std::make_shared<D3D11ConstantBuffer>(device_, desc);
}

StorageBufferPtr D3D11RenderSystem::CreateStorageBuffer(const StorageBuffer::Description& desc)
{
    return std::make_shared<D3D11StorageBuffer>(device_, desc);
}

void D3D11RenderSystem::WriteBuffer(
    VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    /* Write D3D vertex buffer and change buffer format */
    auto vertexBufferD3D = CAST_TO_D3D11_OBJECT(VertexBuffer, vertexBuffer);
    ChangeBufferFormat(vertexBuffer, format);
    
    auto result = vertexBufferD3D->WriteVertices(device_, data, dataSize, D3D11ParamMapper::Map(usage));

    #ifdef FORK_DEBUG
    DXAssert(result, "Writing D3D11 vertex buffer failed", false);
    #endif
}

void D3D11RenderSystem::WriteBuffer(
    IndexBuffer* indexBuffer, const IndexFormat& format, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    /* Write D3D index buffer and change buffer format */
    auto indexBufferD3D = CAST_TO_D3D11_OBJECT(IndexBuffer, indexBuffer);
    ChangeBufferFormat(indexBuffer, format);

    auto result = indexBufferD3D->WriteIndices(device_, data, dataSize, D3D11ParamMapper::Map(usage));

    #ifdef FORK_DEBUG
    DXAssert(result, "Writing D3D11 index buffer failed", false);
    #endif
}

void D3D11RenderSystem::WriteBuffer(
    ConstantBuffer* constantBuffer, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    /* Write D3D constant buffer (Always use DEFAULT usage) */
    auto constantBufferD3D = CAST_TO_D3D11_OBJECT(ConstantBuffer, constantBuffer);
    
    auto result = constantBufferD3D->WriteConstants(device_, data, dataSize, D3D11ParamMapper::Map(usage));

    #ifdef FORK_DEBUG
    DXAssert(result, "Writing D3D11 constant buffer failed", false);
    #endif
}

void D3D11RenderSystem::WriteBuffer(
    StorageBuffer* storageBuffer, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    //todo!
}

void D3D11RenderSystem::WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset)
{
    /* Update D3D11 vertex buffer */
    auto vertexBufferD3D = CAST_TO_D3D11_OBJECT(VertexBuffer, vertexBuffer);
    vertexBufferD3D->WriteSub(context_, data, offset, dataSize);
}

void D3D11RenderSystem::WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset)
{
    /* Update D3D11 index buffer */
    auto indexBufferD3D = CAST_TO_D3D11_OBJECT(IndexBuffer, indexBuffer);
    indexBufferD3D->WriteSub(context_, data, offset, dataSize);
}

void D3D11RenderSystem::WriteSubBuffer(ConstantBuffer* constantBuffer, const void* data, size_t dataSize, size_t offset)
{
    /* Update D3D11 constant buffer (a D3D constant buffer must always be filled entirely!) */
    auto constantBufferD3D = CAST_TO_D3D11_OBJECT(ConstantBuffer, constantBuffer);
    constantBufferD3D->WriteSub(context_, data, offset, dataSize);
}

void* D3D11RenderSystem::MapBuffer(StorageBuffer* storageBuffer, const HardwareBufferCPUAccess cpuAccess)
{
    //!todo!
    return nullptr;
}

void D3D11RenderSystem::UnmapBuffer(StorageBuffer* storageBuffer)
{
    //!todo!
}

/* --- Textures --- */

Texture1DPtr D3D11RenderSystem::CreateTexture1D()
{
    return std::make_shared<D3D11Texture1D>();
}

Texture2DPtr D3D11RenderSystem::CreateTexture2D()
{
    return std::make_shared<D3D11Texture2D>();
}

Texture3DPtr D3D11RenderSystem::CreateTexture3D()
{
    return std::make_shared<D3D11Texture3D>();
}

TextureCubePtr D3D11RenderSystem::CreateTextureCube()
{
    return std::make_shared<D3D11TextureCube>();
}

void D3D11RenderSystem::WriteTexture(
    Texture1D* texture,
    const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    //...
}

void D3D11RenderSystem::WriteSubTexture(
    Texture1D* texture,
    const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    //...
}

void D3D11RenderSystem::WriteTexture(
    Texture2D* texture,
    const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    ValidateImageDataType(imageDataType);

    /* Get D3D11 texture */
    auto textureD3D = CAST_TO_D3D11_OBJECT(Texture2D, texture);

    /* Setup texture attributes */
    textureD3D->SetupSize(textureSize);
    textureD3D->SetupArray(arraySize);

    ChangeTextureFormat(texture, textureFormat);

    /* Setup texture description */
    D3D11_TEXTURE2D_DESC textureDesc;

    textureDesc.Width               = static_cast<UINT>(textureSize.width);
    textureDesc.Height              = static_cast<UINT>(textureSize.height);
    textureDesc.MipLevels           = 0;
    textureDesc.ArraySize           = std::max(1u, arraySize);
    textureDesc.Format              = D3D11ParamMapper::Map(textureFormat);
    textureDesc.SampleDesc.Count    = 1;
    textureDesc.SampleDesc.Quality  = 0;
    textureDesc.Usage               = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags           = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags      = 0;
    textureDesc.MiscFlags           = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    /* Write texture data */
    textureD3D->CreateResource(device_, textureDesc, nullptr);

    if (imageData && arraySize <= 1)
        WriteSubTexture(texture, {}, textureSize, 0, imageFormat, imageDataType, imageData);
}

void D3D11RenderSystem::WriteSubTexture(
    Texture2D* texture,
    const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    /* Get D3D11 texture */
    auto textureD3D = CAST_TO_D3D11_OBJECT(Texture2D, texture);

    /* Setup image buffer pitch and destination box */
    auto pitchPerPixel = GetDataTypeSize(imageDataType)*NumColorComponents(imageFormat);

    D3D11_BOX destBox;

    destBox.left    = static_cast<UINT>(position.x);
    destBox.top     = static_cast<UINT>(position.y);
    destBox.front   = 0;

    destBox.right   = static_cast<UINT>(position.x + size.width);
    destBox.bottom  = static_cast<UINT>(position.y + size.height);
    destBox.back    = 1;

    /* Update texture sub resource */
    auto subResourceIndex = D3D11CalcSubresource(mipLevel, arrayIndex, Math::NumMIPLevelsBySize(texture->GetSize()));

    context_->UpdateSubresource(
        textureD3D->GetResource(),
        subResourceIndex,
        &destBox,
        imageData,
        size.width*pitchPerPixel,
        size.width*size.height*pitchPerPixel
    );
}

void D3D11RenderSystem::WriteTexture(
    Texture3D* texture,
    const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    //...
}

void D3D11RenderSystem::WriteSubTexture(
    Texture3D* texture,
    const Texture3D::PositionType& position, const Texture3D::SizeType& size,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    //...
}

void D3D11RenderSystem::WriteTexture(
    TextureCube* texture,
    const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    ValidateImageDataType(imageDataType);

    /* Get D3D11 texture */
    auto textureD3D = CAST_TO_D3D11_OBJECT(TextureCube, texture);

    /* Setup texture attributes */
    textureD3D->SetupSize(textureSize);
    textureD3D->SetupArray(arraySize);

    ChangeTextureFormat(texture, textureFormat);

    /* Setup texture description */
    D3D11_TEXTURE2D_DESC textureDesc;

    textureDesc.Width               = static_cast<UINT>(textureSize.width);
    textureDesc.Height              = static_cast<UINT>(textureSize.height);
    textureDesc.MipLevels           = 0;
    textureDesc.ArraySize           = std::max(1u, arraySize) * 6;
    textureDesc.Format              = D3D11ParamMapper::Map(textureFormat);
    textureDesc.SampleDesc.Count    = 1;
    textureDesc.SampleDesc.Quality  = 0;
    textureDesc.Usage               = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags           = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags      = 0;
    textureDesc.MiscFlags           = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

    /* Write texture data */
    textureD3D->CreateResource(device_, textureDesc, nullptr);

    if (imageData && arraySize <= 1)
    {
        //WriteSubTexture(texture, {}, textureSize, 0, imageFormat, imageDataType, imageData);
        //...
    }
}

void D3D11RenderSystem::WriteSubTexture(
    TextureCube* texture,
    const TextureCube::PositionType& position, const TextureCube::SizeType& size,
    const TextureCube::Faces faceDirection, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    /* Get D3D11 texture */
    auto textureD3D = CAST_TO_D3D11_OBJECT(TextureCube, texture);

    /* Setup image buffer pitch and destination box */
    auto pitchPerPixel = GetDataTypeSize(imageDataType)*NumColorComponents(imageFormat);

    const auto faceIndex = static_cast<UINT>(faceDirection);

    D3D11_BOX destBox;

    destBox.left    = static_cast<UINT>(position.x);
    destBox.top     = static_cast<UINT>(position.y);
    destBox.front   = 0;

    destBox.right   = static_cast<UINT>(position.x + size.width);
    destBox.bottom  = static_cast<UINT>(position.y + size.height);
    destBox.back    = 1;

    /* Update texture sub resource */
    auto subResourceIndex = D3D11CalcSubresource(mipLevel, arrayIndex*6 + faceIndex, Math::NumMIPLevelsBySize(texture->GetSize()));

    context_->UpdateSubresource(
        textureD3D->GetResource(),
        subResourceIndex,
        &destBox,
        imageData,
        size.width*pitchPerPixel,
        size.width*size.height*pitchPerPixel
    );
}

bool D3D11RenderSystem::ReadTexture(const Texture* texture, ImageUByte& image, unsigned int mipLevel)
{
    return false; //todo ...
}

bool D3D11RenderSystem::ReadTexture(const Texture* texture, ImageFloat& image, unsigned int mipLevel)
{
    return false; //todo ...
}

/* --- Render targets --- */

RenderTargetPtr D3D11RenderSystem::CreateRenderTarget()
{
    return std::make_shared<D3D11RenderTarget>(this);
}

/* --- Shaders --- */

VertexShaderPtr D3D11RenderSystem::CreateVertexShader()
{
    return std::make_shared<D3D11VertexShader>();
}

PixelShaderPtr D3D11RenderSystem::CreatePixelShader()
{
    return std::make_shared<D3D11PixelShader>();
}

GeometryShaderPtr D3D11RenderSystem::CreateGeometryShader()
{
    return std::make_shared<D3D11GeometryShader>();
}

TessControlShaderPtr D3D11RenderSystem::CreateTessControlShader()
{
    return std::make_shared<D3D11TessControlShader>();
}

TessEvaluationShaderPtr D3D11RenderSystem::CreateTessEvaluationShader()
{
    return std::make_shared<D3D11TessEvaluationShader>();
}

ComputeShaderPtr D3D11RenderSystem::CreateComputeShader()
{
    return std::make_shared<D3D11ComputeShader>();
}

ShaderCompositionPtr D3D11RenderSystem::CreateShaderComposition()
{
    return std::make_shared<D3D11ShaderComposition>(device_);
}

/* --- Query --- */

QueryPtr D3D11RenderSystem::CreateQuery(const Query::Types type)
{
    try
    {
        return std::make_shared<D3D11Query>(device_, type);
    }
    catch (HRESULT)
    {
        return nullptr;
    }
}


/*
 * ======= Protected: =======
 */

std::shared_ptr<D3D11RenderContext> D3D11RenderSystem::InitRenderContext(
    const std::shared_ptr<D3D11RenderContext>& renderContext, const VideoMode& videoMode, const ContextDescription& contextDesc)
{
    /* Create default resources only once when the first render context is created */
    if (renderContexts_.empty())
    {
        InitSharedRenderContext(renderContext.get());
        CreateDefaultResources(contextDesc);
    }

    renderContexts_.push_back(renderContext);

    /* Show context frame */
    renderContext->GetFrame()->Show();

    return renderContext;
}

IDXGISwapChain* D3D11RenderSystem::CreateSwapChain(
    const VideoMode& videoMode, const ContextDescription& contextDesc, HWND windowHandle)
{
    /* Setup swap chain description */
    DXGI_SWAP_CHAIN_DESC swpChnDesc;
    InitRawMemory(swpChnDesc);
    
    swpChnDesc.BufferCount                          = 1; /* 1 back-buffer */
    swpChnDesc.BufferDesc.Width                     = videoMode.resolution.width;
    swpChnDesc.BufferDesc.Height                    = videoMode.resolution.height;
    swpChnDesc.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    swpChnDesc.BufferDesc.RefreshRate.Numerator     = contextDesc.vsyncDesc.refreshRate;
    swpChnDesc.BufferDesc.RefreshRate.Denominator   = contextDesc.vsyncDesc.interval;
    swpChnDesc.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swpChnDesc.OutputWindow                         = windowHandle;
    swpChnDesc.SampleDesc.Count                     = contextDesc.antiAliasingDesc.ActualSamples();
    swpChnDesc.SampleDesc.Quality                   = 0;
    swpChnDesc.Windowed                             = !videoMode.isFullscreen;
    
    /* Create shwap chain with main DXGI factory */
    IDXGISwapChain* swapChain = nullptr;

    auto result = factory_->CreateSwapChain(device_, &swpChnDesc, &swapChain);
    if (!DXAssert(result, "Creating DXGI swap chain failed", false))
        return nullptr;
    
    return swapChain;
}

ID3D11Texture2D* D3D11RenderSystem::QueryBackBuffer(IDXGISwapChain* swapChain)
{
    ASSERT_POINTER(swapChain);

    /* Get back buffer from swap chain */
    ID3D11Texture2D* backBuffer = nullptr;

    auto result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (!DXAssert(result, "Getting back buffer from DXGI swap chain failed", false))
        return nullptr;

    return backBuffer;
}

ID3D11RenderTargetView* D3D11RenderSystem::CreateRenderTargetView(ID3D11Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC* desc)
{
    /* Create render target view for resource */
    ID3D11RenderTargetView* renderTargetView = nullptr;

    auto result = device_->CreateRenderTargetView(resource, desc, &renderTargetView);
    if (!DXAssert(result, "Creating D3D11 render target view failed", false))
        return nullptr;

    return renderTargetView;
}

ID3D11Texture2D* D3D11RenderSystem::CreateDepthStencil(const Math::Size2ui& resolution, unsigned int numSamples)
{
    /* Setup depth stencil description */
    D3D11_TEXTURE2D_DESC texDesc;
    InitRawMemory(texDesc);
    
    texDesc.Width               = resolution.width;
    texDesc.Height              = resolution.height;
    texDesc.MipLevels           = 1;
    texDesc.ArraySize           = 1;
    texDesc.Format              = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texDesc.Usage               = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags           = D3D11_BIND_DEPTH_STENCIL;
    texDesc.CPUAccessFlags      = 0;
    texDesc.MiscFlags           = 0;
    texDesc.SampleDesc.Count    = numSamples;
    texDesc.SampleDesc.Quality  = 0;
    
    /* Create depth stencil */
    ID3D11Texture2D* depthStencil = nullptr;
    
    auto result = device_->CreateTexture2D(&texDesc, 0, &depthStencil);
    if (!DXAssert(result, "Creating D3D11 depth stencil for back buffer failed", false))
        return nullptr;
    
    return depthStencil;
}

ID3D11DepthStencilView* D3D11RenderSystem::CreateDepthStencilView(ID3D11Texture2D* depthStencil, bool hasAntiAliasing)
{
    /* Setup depth stencil view description */
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    InitRawMemory(dsvDesc);
    
    dsvDesc.Format              = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension       = (hasAntiAliasing ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D);
    dsvDesc.Texture2D.MipSlice  = 0;
    
    /* Create depth stencil view */
    ID3D11DepthStencilView* depthStencilView = nullptr;

    auto result = device_->CreateDepthStencilView(depthStencil, &dsvDesc, &depthStencilView);
    if (!DXAssert(result, "Creating D3D11 depth stencil view for back buffer failed", false))
        return nullptr;
    
    return depthStencilView;
}


/*
 * ======= Private: =======
 */

std::unique_ptr<ShaderProvider> D3D11RenderSystem::CreateShaderProvider()
{
    return std::make_unique<D3D11ShaderProvider>();
}

ID3D11Device* D3D11RenderSystem::CreateDevice(
    const D3D_FEATURE_LEVEL* featureLevel, D3D_FEATURE_LEVEL* featureLevelOut,
    IDXGIAdapter* adapter, ID3D11DeviceContext*& context)
{
    UINT flags = 0;

    #ifdef FORK_DEBUG
    //(slows down text drawing dramatically!)
    //flags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

    /* Create D3D device */
    ID3D11Device* device = nullptr;

    auto result = D3D11CreateDevice(
        adapter,                    // Video adapter
        D3D_DRIVER_TYPE_HARDWARE,   // Driver type
        0,                          // Software rasterizer module (none)
        flags,                      // Flags
        featureLevel,               // Feature level
        featureLevel ? 1 : 0,       // Num feature levels
        D3D11_SDK_VERSION,          // SDK version
        &device,                    // Output device
        featureLevelOut,            // Output feature level
        &context                    // Output device context
    );

    /* Check for errors */
    if (result != S_OK || !device || !context)
    {
        /* Release objects and return with error */
        DXSafeRelease(context);
        DXSafeRelease(device);
        throw std::runtime_error("Creating D3D11 device and device context failed");
    }

    return device;
}

IDXGIFactory* D3D11RenderSystem::QueryFactory(ID3D11Device* device)
{
    IDXGIFactory* factory = nullptr;

    /* Get DirectX factory */
    IDXGIDevice* dxDevice = nullptr;
    if (device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxDevice) != S_OK)
        throw std::runtime_error("Querying DXGI device failed");

    IDXGIAdapter* dxAdapter = nullptr;
    if (dxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxAdapter) != S_OK)
        throw std::runtime_error("Querying DXGI device failed");

    if (dxAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory) != S_OK)
        throw std::runtime_error("Querying DXGI device failed");

    return factory;
}

std::vector<D3D11RenderSystem::VideoAdapter> D3D11RenderSystem::QueryVideoAdapters(IDXGIFactory* factory, D3D_FEATURE_LEVEL featureLevel)
{
    std::vector<VideoAdapter> videoAdapters;

    VideoAdapter adapter;
    DXGI_ADAPTER_DESC adapterDesc;

    /* Eunmerate all video adapters */
    for (UINT i = 0; factory->EnumAdapters(i, &adapter.adapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        adapter.adapter->GetDesc(&adapterDesc);
        {
            adapter.info.graphicsDeviceDesc = UTF16toUTF8(adapterDesc.Description);
            adapter.info.hardwareVendor     = GetVendorNameByID(adapterDesc.VendorId);
            adapter.info.rendererVersion    = FeatureLevelToStr(D3D11ParamMapper::Unmap(featureLevel));
            adapter.info.shaderModel        = DXFeatureLevelToShaderModel(featureLevel);
        }
        videoAdapters.push_back(adapter);
    }

    return videoAdapters;
}


} // /namespace Video

} // /namespace Fork



// ========================