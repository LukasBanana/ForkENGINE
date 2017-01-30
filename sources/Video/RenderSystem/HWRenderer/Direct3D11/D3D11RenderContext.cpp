/*
 * Direct3D 11 render context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11RenderContext.h"
#include "D3D11RenderSystem.h"
#include "D3D11Core.h"
#include "D3D11ParamMapper.h"
#include "D3D11FeatureSupport.h"
#include "D3D11Query.h"
#include "Core/CastToConcreteObject.h"
#include "Video/Monitor/VendorIDs.h"

#include "HardwareBuffer/D3D11VertexBuffer.h"
#include "HardwareBuffer/D3D11IndexBuffer.h"

#include "RenderState/D3D11BlendState.h"
#include "RenderState/D3D11RasterizerState.h"
#include "RenderState/D3D11DepthStencilState.h"
#include "RenderState/D3D11SamplerState.h"

#include "Texture/D3D11Texture1D.h"
#include "Texture/D3D11Texture2D.h"
#include "Texture/D3D11Texture3D.h"
#include "Texture/D3D11TextureCube.h"
#include "Texture/D3D11RenderTarget.h"

#include "Shader/D3D11ShaderComposition.h"

#include "IO/Core/Log.h"

#include "Core/Exception/PointerConversionException.h"
#include "Core/Exception/InvalidStateException.h"
#include "Video/RenderSystem/RenderContextException.h"


namespace Fork
{

namespace Video
{


/* --- Internal members --- */

static const unsigned int maxNumShaderRrsources = 128;


/*
 * D3D11RenderContext class
 */

D3D11RenderContext::D3D11RenderContext(
    D3D11RenderSystem* renderSystem, ID3D11DeviceContext* context,
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame) :
        RenderContext   ( videoMode, contextDesc ),
        renderSystem_   ( renderSystem           ),
        context_        ( context                )
{
    mainFrameBuffer_.renderTargetViews.resize(1);

    /* Create frame and render context */
    CreateFrame(frameTitle, parentFrame);
    CreateContext();
}
D3D11RenderContext::~D3D11RenderContext()
{
    ReleaseContext();
}

/* --- Common --- */

Adapter D3D11RenderContext::QueryAdapter() const
{
    const auto& videoAdapters = renderSystem_->videoAdapters_;
    return videoAdapters.empty() ? Adapter() : videoAdapters.front().info;
}

RendererDetails D3D11RenderContext::QueryRendererDetails() const
{
    RendererDetails details;

    details.hasOriginLeftTop                = true;
    details.hasLeftHandedCoordinateSystem   = true;

    return details;
}

RendererSupport D3D11RenderContext::QueryRendererSupport() const
{
    return DXFeatureSupport(renderSystem_->featureLevel_);
}

RenderStateReflection D3D11RenderContext::QueryRenderStateReflection() const
{
    RenderStateReflection rs;

    //todo...

    /* Query viewport state */
    UINT numViewports = 0;
    context_->RSGetViewports(&numViewports, nullptr);

    std::vector<D3D11_VIEWPORT> viewportStates(numViewports);
    context_->RSGetViewports(&numViewports, viewportStates.data());

    for (const auto& viewportState : viewportStates)
    {
        Viewport viewport;
        {
            viewport.position   = Math::Point2f(viewportState.TopLeftX, viewportState.TopLeftY).Cast<int>();
            viewport.size       = Math::Size2f(viewportState.Width, viewportState.Height).Cast<int>();
            viewport.minDepth   = viewportState.MinDepth;
            viewport.maxDepth   = viewportState.MaxDepth;
        }
        rs.viewports.push_back(viewport);
    }

    /* Query scissor states */
    UINT numScissors = 0;
    context_->RSGetScissorRects(&numScissors, nullptr);

    std::vector<D3D11_RECT> scissorStates(numScissors);
    context_->RSGetScissorRects(&numScissors, scissorStates.data());

    for (const auto& scissorState : scissorStates)
    {
        Scissor scissor;
        {
            scissor.position    = { scissorState.left, scissorState.top };
            scissor.size        = { scissorState.right - scissorState.left, scissorState.bottom - scissorState.top };
        }
        rs.scissors.push_back(scissor);
    }

    return rs;
}

ShaderCode::Languages D3D11RenderContext::ShadingLanguage() const
{
    return ShaderCode::Languages::HLSL;
}

std::string D3D11RenderContext::ShadingLanguageFileExt() const
{
    return "hlsl";
}

std::vector<Shader::Versions> D3D11RenderContext::ShaderModels() const
{
    std::vector<Shader::Versions> shaderModels;

    for (int i = static_cast<int>(Shader::Versions::HLSL_4_0); i <= static_cast<int>(Shader::Versions::HLSL_5_0); ++i)
        shaderModels.push_back(static_cast<Shader::Versions>(i));

    return shaderModels;
}

bool D3D11RenderContext::Activate()
{
    if (!IsActivated())
    {
        RenderContext::SetActiveRenderContext(this);
        BindFrameBuffer();
    }
    return true;
}

bool D3D11RenderContext::Deactivate()
{
    if (IsActivated())
    {
        UnbindFrameBuffer();
        RenderContext::SetActiveRenderContext(nullptr);
    }
    return true;
}

void D3D11RenderContext::SetupClearColor(const ColorRGBAf& color)
{
    Activate();
    RenderContext::SetupClearColor(color);
}

void D3D11RenderContext::SetupClearDepth(float depth)
{
    Activate();
    RenderContext::SetupClearDepth(depth);
}

void D3D11RenderContext::SetupClearStencil(unsigned char stencil)
{
    Activate();
    RenderContext::SetupClearStencil(stencil);
}

void D3D11RenderContext::ClearBuffers(const ClearBuffersFlags::DataType flags)
{
    Activate();

    /* Get D3D clear flags */
    UINT dsFlags = 0;
    
    if (flags & ClearBuffersFlags::Depth)
        dsFlags |= D3D11_CLEAR_DEPTH;
    if (flags & ClearBuffersFlags::Stencil)
        dsFlags |= D3D11_CLEAR_STENCIL;
    
    /* Clear color- and depth-stencil buffers for main render-target */
    if (activeFrameBuffer_)
    {
        if (flags & ClearBuffersFlags::Color)
        {
            for (auto& renderTargetView : activeFrameBuffer_->renderTargetViews)
                context_->ClearRenderTargetView(renderTargetView, clearColor_.Ptr());
        }
        if (dsFlags && activeFrameBuffer_->depthStencilView)
            context_->ClearDepthStencilView(activeFrameBuffer_->depthStencilView, dsFlags, clearDepth_, clearStencil_);
    }
}

void D3D11RenderContext::Present()
{
    swapChain_->Present(syncInterval_, 0);
}

bool D3D11RenderContext::ChangeVideoMode(const VideoMode& videoMode)
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
            if (videoMode_.isFullscreen != videoMode.isFullscreen)
                swapChain_->SetFullscreenState(videoMode.isFullscreen, nullptr);
        }

        /* Resize back buffer */
        if (videoMode_.resolution != videoMode.resolution && !ResizeBackBuffer(videoMode.resolution))
            return false;

        /* Store new settings */
        videoMode_ = videoMode;
    }
    return true;
}

bool D3D11RenderContext::ChangeDesc(const ContextDescription& contextDesc)
{
    if (GetDesc().antiAliasingDesc != contextDesc.antiAliasingDesc)
    {
        /* Store new settings and recreate context */
        contextDesc_ = contextDesc;
        CreateContext(false);
    }
    else
    {
        /* Store new settings and update synchronisation interval */
        contextDesc_ = contextDesc;
        UpdateSyncInterval();
    }
    return true;
}

/* --- Render states --- */

void D3D11RenderContext::ChangeState(const RasterizerState* state)
{
    /* Set D3D rasterizer state */
    auto stateD3D = CAST_TO_D3D11_OBJECT(RasterizerState const, state);
    context_->RSSetState(stateD3D->GetStateObject());
}

void D3D11RenderContext::ChangeState(const DepthStencilState* state, unsigned int stencilRef)
{
    /* Set D3D depth-stencil state */
    auto stateD3D = CAST_TO_D3D11_OBJECT(DepthStencilState const, state);
    context_->OMSetDepthStencilState(stateD3D->GetStateObject(), stencilRef);
}

void D3D11RenderContext::ChangeState(const BlendState* state)
{
    /* Set D3D blend state */
    auto stateD3D = CAST_TO_D3D11_OBJECT(BlendState const, state);
    const FLOAT blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context_->OMSetBlendState(stateD3D->GetStateObject(), blendFactor, stateD3D->GetSampleMask());
}

void D3D11RenderContext::SetupViewport(const Viewport& viewport)
{
    //if (globalRenderState_.viewport != viewport)
    {
        /* Store viewport configuration */
        globalRenderState_.viewport = viewport;

        /* Set D3D viewport */
        D3D11_VIEWPORT dxViewport
        {
            static_cast<float>(viewport.position.x),
            static_cast<float>(viewport.position.y),
            static_cast<float>(viewport.size.width),
            static_cast<float>(viewport.size.height),
            viewport.minDepth,
            viewport.maxDepth
        };
        context_->RSSetViewports(1, &dxViewport);
    }
}

void D3D11RenderContext::SetupMultiViewport(const std::vector<Viewport>& viewports)
{
    const auto numViewports = viewports.size();
    std::vector<D3D11_VIEWPORT> dxViewports(numViewports);

    for (size_t i = 0; i < numViewports; ++i)
    {
        const auto& viewport = viewports[i];
        auto& dxViewport = dxViewports[i];

        dxViewport.TopLeftX = static_cast<float>(viewport.position.x);
        dxViewport.TopLeftY = static_cast<float>(viewport.position.y);
        dxViewport.Width    = static_cast<float>(viewport.size.width);
        dxViewport.Height   = static_cast<float>(viewport.size.height);
        dxViewport.MinDepth = viewport.minDepth;
        dxViewport.MaxDepth = viewport.maxDepth;
    }

    context_->RSSetViewports(static_cast<UINT>(numViewports), dxViewports.data());
}

void D3D11RenderContext::SetupScissor(const Scissor& scissor)
{
    //if (globalRenderState_.scissor != scissor)
    {
        /* Store viewport configuration */
        globalRenderState_.scissor = scissor;

        /* Set D3D scissor */
        D3D11_RECT rect
        {
            scissor.position.x,
            scissor.position.y,
            scissor.position.x + scissor.size.width,
            scissor.position.y + scissor.size.height
        };
        context_->RSSetScissorRects(1, &rect);
    }
}

void D3D11RenderContext::SetupMultiScissor(const std::vector<Scissor>& scissors)
{
    const auto numScissors = scissors.size();
    std::vector<D3D11_RECT> rects(numScissors);

    for (size_t i = 0; i < numScissors; ++i)
    {
        const auto& scissor = scissors[i];
        auto& rect = rects[i];

        rect.left   = scissor.position.x;
        rect.top    = scissor.position.y;
        rect.right  = scissor.position.x + scissor.size.width;
        rect.bottom = scissor.position.y + scissor.size.height;
    }

    context_->RSSetScissorRects(static_cast<UINT>(numScissors), rects.data());
}

void D3D11RenderContext::SetupDrawMode(const GeometryPrimitives type)
{
    globalRenderState_.drawPrimitive = type;

    #ifdef FORK_DEBUG

    auto topology = D3D11ParamMapper::Map(type);
    
    if (topology == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED)
    {
        IO::Log::Debug(
            "RenderSystem::SetupDrawMode",
            "Invalid geometry primitive for D3D11 render system", 1
        );
    }
    
    context_->IASetPrimitiveTopology(topology);

    #else

    context_->IASetPrimitiveTopology(D3D11ParamMapper::Map(type));

    #endif
}

static void SetupSamplerToAllShaderStages(
    ID3D11DeviceContext* context, ID3D11SamplerState* samplerState, UINT slot)
{
    context->VSSetSamplers(slot, 1, &samplerState);
    context->PSSetSamplers(slot, 1, &samplerState);
    context->GSSetSamplers(slot, 1, &samplerState);
    context->HSSetSamplers(slot, 1, &samplerState);
    context->DSSetSamplers(slot, 1, &samplerState);
    context->CSSetSamplers(slot, 1, &samplerState);
}

void D3D11RenderContext::BindSamplerState(const SamplerState* state, unsigned int layer)
{
    /* Bind sampler to all shader stages */
    auto stateD3D = CAST_TO_D3D11_OBJECT(SamplerState const, state);
    SetupSamplerToAllShaderStages(context_, stateD3D->GetStateObject(), layer);
}

void D3D11RenderContext::UnbindSamplerState(unsigned int layer)
{
    /* Unbind sampler from all shader stages */
    SetupSamplerToAllShaderStages(context_, nullptr, layer);
}

/* --- Hardware buffers --- */

void D3D11RenderContext::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    /* Bind a single vertex buffer */
    auto vertexBufferD3D = CAST_TO_D3D11_OBJECT(VertexBuffer, vertexBuffer);

    UINT stride = vertexBuffer->GetFormat().GetFormatSize();
    UINT offset = 0;

    ID3D11Buffer* buffer = vertexBufferD3D->GetBuffer();
    context_->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

void D3D11RenderContext::UnbindVertexBuffer()
{
    /* Unbind vertex buffers */
    context_->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
}

void D3D11RenderContext::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    /* Map index format and bind buffer */
    auto indexBufferD3D = CAST_TO_D3D11_OBJECT(IndexBuffer, indexBuffer);
    auto format = D3D11ParamMapper::Map(indexBuffer->GetFormat().GetDataType());
    context_->IASetIndexBuffer(indexBufferD3D->GetBuffer(), format, 0);
}

void D3D11RenderContext::UnbindIndexBuffer()
{
    /* Unbind index buffer */
    context_->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
}

/* --- Textures --- */

static void SetupResourceToAllShaderStages(
    ID3D11DeviceContext* context, ID3D11ShaderResourceView* resourceView, UINT slot)
{
    context->VSSetShaderResources(slot, 1, &resourceView);
    context->PSSetShaderResources(slot, 1, &resourceView);
    context->GSSetShaderResources(slot, 1, &resourceView);
    context->HSSetShaderResources(slot, 1, &resourceView);
    context->DSSetShaderResources(slot, 1, &resourceView);
    context->CSSetShaderResources(slot, 1, &resourceView);
}

void D3D11RenderContext::BindTexture(const Texture* texture, unsigned int layer)
{
    if (layer < maxNumShaderRrsources)
    {
        /* Get D3D11 texture */
        auto textureD3D = dynamic_cast<const D3D11TextureBase*>(texture);
        if (!textureD3D)
            throw PointerConversionException(__FUNCTION__, "Texture", "D3D11TextureBase");

        /* Bind texture to shader stages */
        SetupResourceToAllShaderStages(context_, textureD3D->GetResourceView(), layer);
    }
}

void D3D11RenderContext::UnbindTexture(const Texture* texture, unsigned int layer)
{
    if (layer < maxNumShaderRrsources)
    {
        /* Unbind texture from shader stages */
        SetupResourceToAllShaderStages(context_, nullptr, layer);
    }
}

void D3D11RenderContext::CopyTextureImage(
    Texture2D* texture, const Math::Recti& sourceArea,
    const Math::Point2i& destOffset, unsigned int mipLevel)
{
    /* Get D3D texture */
    auto textureD3D = CAST_TO_D3D11_OBJECT(Texture2D, texture);

    /* Copy resource from back buffer */
    CopyResource(textureD3D->AbstractResource(), backBuffer_, sourceArea, destOffset, mipLevel);
}

void D3D11RenderContext::CopyTextureImage(
    Texture2D* texture, const RenderTarget* renderTarget, const Math::Recti& sourceArea,
    const Math::Point2i& destOffset, unsigned int mipLevel)
{
    /* Get D3D texture and D3D render target */
    auto textureD3D = CAST_TO_D3D11_OBJECT(Texture2D, texture);
    auto renderTargetD3D = CAST_TO_D3D11_OBJECT(RenderTarget const, renderTarget);

    /* Copy resource from first render target texture */
    //CopyResource(textureD3D->AbstractResource(), ..., sourceArea, destOffset, mipLevel);
}

void D3D11RenderContext::GenerateMIPMaps(Texture* texture)
{
    /* Get D3D11 texture */
    auto textureD3D = dynamic_cast<const D3D11TextureBase*>(texture);
    if (!textureD3D)
        throw PointerConversionException(__FUNCTION__, "Texture", "D3D11TextureBase");
    
    /* Generate MIP-maps */
    if (textureD3D->GetResourceView())
        context_->GenerateMips(textureD3D->GetResourceView());

    /* Store information that this texture has now MIP-maps */
    ChangeTextureMIPState(texture, true);
}

void D3D11RenderContext::GenerateMIPMaps(
    Texture2D* destTexture, const Texture2D* sourceTexture, const ShaderComposition* shader,
    unsigned int layer, const GenerateMIPMapsShaderCallback& shaderCallback)
{
    /* Get D3D texture and shader */
    auto destTextureD3D = CAST_TO_D3D11_OBJECT(Texture2D, destTexture);
    auto shaderD3D = CAST_TO_D3D11_OBJECT(ShaderComposition const, shader);

    /* Check if MIP chain has already been allocated */
    if (/*!texture->HasMIPMaps() && */destTextureD3D->GetResourceView())
    {
        /* Allocate MIP chain and store information that this texture has now MIP-maps */
        context_->GenerateMips(destTextureD3D->GetResourceView());
        ChangeTextureMIPState(destTexture, true);
    }

    //...
}

/* --- Render targets --- */

void D3D11RenderContext::BindRenderTarget(RenderTarget* renderTarget)
{
    if (renderTarget == GetRenderState().renderTarget)
        return;

    /* Get D3D render target */
    D3D11RenderTarget* renderTargetD3D = nullptr;
    if (renderTarget)
        renderTargetD3D = CAST_TO_D3D11_OBJECT(RenderTarget, renderTarget);
    
    if (renderTargetD3D)
        BindFrameBuffer(&(renderTargetD3D->GetFrameBuffer()));
    else
        BindMainFrameBuffer();

    /* Store new active render target */
    globalRenderState_.renderTarget = renderTarget;
}

/* --- Drawing --- */

void D3D11RenderContext::Draw(unsigned int numVertices, unsigned int firstVertex)
{
    context_->Draw(numVertices, firstVertex);
}

void D3D11RenderContext::DrawIndexed(unsigned int numVertices)
{
    context_->DrawIndexed(numVertices, 0, 0);
}

void D3D11RenderContext::DrawIndexed(unsigned int numVertices, int indexOffset)
{
    context_->DrawIndexed(numVertices, 0, indexOffset);
}

void D3D11RenderContext::DrawInstanced(
    unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances)
{
    context_->DrawInstanced(numVertices, numInstances, firstVertex, 0);
}

void D3D11RenderContext::DrawInstanced(
    unsigned int numVertices, unsigned int firstVertex,
    unsigned int numInstances, unsigned int instanceOffset)
{
    context_->DrawInstanced(numVertices, numInstances, firstVertex, instanceOffset);
}

void D3D11RenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances)
{
    context_->DrawIndexedInstanced(numVertices, numInstances, 0, 0, 0);
}

void D3D11RenderContext::DrawInstancedIndexed(
    unsigned int numVertices, unsigned int numInstances, int indexOffset)
{
    context_->DrawIndexedInstanced(numVertices, numInstances, 0, indexOffset, 0);
}

void D3D11RenderContext::DrawInstancedIndexed(
    unsigned int numVertices, unsigned int numInstances,
    int indexOffset, unsigned int instanceOffset)
{
    context_->DrawIndexedInstanced(numVertices, numInstances, 0, indexOffset, instanceOffset);
}

/* --- Shaders --- */

void D3D11RenderContext::BindShader(const ShaderComposition* shaderComposition)
{
    /* Check if shader composition is already bound */
    if (globalRenderState_.shaderComposition == shaderComposition)
        return;

    globalRenderState_.shaderComposition = shaderComposition;

    if (!shaderComposition->HasCompiledSuccessful())
    {
        #ifdef FORK_DEBUG
        IO::Log::Debug(__FUNCTION__, "Binding shader which was not successfully compiled", 10);
        #endif
        return;
    }

    /* Bind D3D shader */
    auto shaderCompositionD3D = CAST_TO_D3D11_OBJECT(ShaderComposition const, shaderComposition);

    shaderCompositionD3D->BindShader(context_);
}

void D3D11RenderContext::UnbindShader()
{
    if (globalRenderState_.shaderComposition)
    {
        /* Unbind shader stages */
        context_->VSSetShader(nullptr, nullptr, 0);
        context_->HSSetShader(nullptr, nullptr, 0);
        context_->DSSetShader(nullptr, nullptr, 0);
        context_->GSSetShader(nullptr, nullptr, 0);
        context_->PSSetShader(nullptr, nullptr, 0);
        context_->CSSetShader(nullptr, nullptr, 0);

        globalRenderState_.shaderComposition = nullptr;
    }
}

void D3D11RenderContext::Dispatch(const Math::Size3ui& numThreadGroups)
{
    context_->Dispatch(numThreadGroups.width, numThreadGroups.height, numThreadGroups.depth);
}

/* --- Query --- */

void D3D11RenderContext::BeginQuery(Query* query)
{
    /* Get D3D query object */
    auto queryD3D = CAST_TO_D3D11_OBJECT(Query, query);

    /* Check query state */
    if ((queryStateFlags_ & Query::StateFlags::ReadyToBegin) == 0)
        throw InvalidStateException(__FUNCTION__, "Query object not read to begin");

    /* Begin with query */
    context_->Begin(queryD3D->GetQueryObject());

    /* Change query state from 'read to begin' to 'waiting for end' */
    queryStateFlags_ = Query::StateFlags::WaitingForEnd;
}

void D3D11RenderContext::EndQuery(Query* query)
{
    /* Get D3D query object */
    auto queryD3D = CAST_TO_D3D11_OBJECT(Query, query);

    /* Check query state */
    if ((queryStateFlags_ & Query::StateFlags::WaitingForEnd) == 0)
        throw InvalidStateException(__FUNCTION__, "Query object not waiting to end");

    /* End with query */
    context_->End(queryD3D->GetQueryObject());

    /* Change query state to 'read to begin' and 'result available' */
    queryStateFlags_ = (Query::StateFlags::ReadyToBegin | Query::StateFlags::ResultAvailable);
}

unsigned int D3D11RenderContext::QueryResult(Query* query)
{
    /* Get D3D query object */
    auto queryD3D = CAST_TO_D3D11_OBJECT(Query, query);

    /* Check query state */
    if ((queryStateFlags_ & Query::StateFlags::ResultAvailable) == 0)
        throw InvalidStateException(__FUNCTION__, "Query object has no available result");

    /* Wait until the result is available */
    UINT64 result;
    while (context_->GetData(queryD3D->GetQueryObject(), &result, sizeof(result), 0) != S_OK)
    {
        // dummy
    }

    /* Return result from the D3D query object */
    return static_cast<unsigned int>(result);
}


/*
 * ======= Private: =======
 */

void D3D11RenderContext::UpdateSyncInterval()
{
    syncInterval_ = GetDesc().vsyncDesc.ActualInternal();
}

void D3D11RenderContext::CreateContext(bool activate)
{
    /* Release previous D3D objects */
    ReleaseContext();

    /* Create all render context resources */
    if ( CreateSwapChain() &&
         CreateBackBufferAndRTV() &&
         CreateDepthStencilAndDSV(GetVideoMode().resolution) )
    {
        /* Make the main frame buffer the active one */
        activeFrameBuffer_ = (&mainFrameBuffer_);
        if (activate)
            Activate();
    }
    else
        throw RenderContextException("Creating Direct3D 11 render context failed");

    /* Update synchronisation interval */
    UpdateSyncInterval();
}

void D3D11RenderContext::ReleaseContext()
{
    DXSafeRelease( MainDepthStencilView() );
    DXSafeRelease( MainRenderTargetView() );
    DXSafeRelease( backBuffer_            );
    DXSafeRelease( depthStencil_          );
    DXSafeRelease( swapChain_             );
}

bool D3D11RenderContext::CreateSwapChain()
{
    swapChain_ = renderSystem_->CreateSwapChain(
        GetVideoMode(), GetDesc(),
        *reinterpret_cast<const HWND*>(GetFrame()->GetNativeHandle())
    );
    return swapChain_ != nullptr;
}

bool D3D11RenderContext::CreateBackBufferAndRTV()
{
    backBuffer_ = renderSystem_->QueryBackBuffer(swapChain_);

    if (backBuffer_)
    {
        MainRenderTargetView() = renderSystem_->CreateRenderTargetView(backBuffer_);
        return MainRenderTargetView() != nullptr;
    }

    return false;
}

bool D3D11RenderContext::CreateDepthStencilAndDSV(const Math::Size2ui& resolution)
{
    depthStencil_ = renderSystem_->CreateDepthStencil(resolution, GetDesc().antiAliasingDesc.ActualSamples());

    if (depthStencil_)
    {
        mainFrameBuffer_.depthStencilView = renderSystem_->CreateDepthStencilView(depthStencil_, GetDesc().antiAliasingDesc.isEnabled);
        return mainFrameBuffer_.depthStencilView != nullptr;
    }

    return false;
}

bool D3D11RenderContext::ReCreateDepthStencilAndDSV(const Math::Size2ui& resolution)
{
    /* Release previous depth stencil and depth stencil view */
    DXSafeRelease( depthStencil_          );
    DXSafeRelease( MainDepthStencilView() );

    /* Create new depth stencil/ -view */
    return CreateDepthStencilAndDSV(resolution);
}

/*
This functions first releases all attachments from the back buffer.
This is required to succeed the "IDXGISwapChain::ResizeBuffers" function call.
After resizing the back buffer, the released objects will be recreated.
*/
bool D3D11RenderContext::ResizeBackBuffer(const Math::Size2ui& resolution)
{
    /* Temporarily disable render target (if this is the active context) */
    auto frameBuffer = activeFrameBuffer_;
    UnbindFrameBuffer();

    /* Releas previous back-buffer render target view (RTV) */
    DXSafeRelease(MainRenderTargetView());
    DXSafeRelease(backBuffer_);

    /*
    Resize swap chain buffers.
    Let DXGI find out the client window area and preserve buffer count and format.
    */
    if (swapChain_->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0) != S_OK)
    {
        IO::Log::Error("Resizing D3D11 swap chain buffers failed");
        return false;
    }

    /* ReCreate back buffer render target view (RTV), depth stencil and depth stencil view (DSV) */
    if (!CreateBackBufferAndRTV() || !ReCreateDepthStencilAndDSV(resolution))
        return false;

    /* Make this the current context again (if it was the active one) */
    BindFrameBuffer(frameBuffer);

    return true;
}

void D3D11RenderContext::BindFrameBuffer(const D3D11FrameBuffer* frameBuffer)
{
    if (IsActivated())
    {
        /* Set active render targets to output merger (if this is the active render context) */
        if (frameBuffer)
        {
            context_->OMSetRenderTargets(
                frameBuffer->renderTargetViews.size(),
                frameBuffer->renderTargetViews.data(),
                frameBuffer->depthStencilView
            );
        }
        else
            context_->OMSetRenderTargets(0, nullptr, nullptr);
    }

    /* Store active framebuffer settings */
    activeFrameBuffer_ = frameBuffer;
}

void D3D11RenderContext::BindMainFrameBuffer()
{
    BindFrameBuffer(&mainFrameBuffer_);
}

void D3D11RenderContext::BindFrameBuffer()
{
    BindFrameBuffer(activeFrameBuffer_);
}

void D3D11RenderContext::UnbindFrameBuffer()
{
    BindFrameBuffer(nullptr);
}

void D3D11RenderContext::CopyResource(
    ID3D11Resource* destination, ID3D11Resource* source, const Math::Recti& sourceArea,
    const Math::Point2i& destOffset, unsigned int mipLevel)
{
    if (IsRectFullscreen(sourceArea) && mipLevel == 0 && destOffset.x == 0 && destOffset.y == 0)
    {
        /* Copy entire back buffer into texture resource */
        context_->CopyResource(destination, source);
    }
    else
    {
        /* Copy partial back buffer into texture subresource */
        D3D11_BOX sourceBox;

        sourceBox.left      = sourceArea.left;
        sourceBox.top       = sourceArea.top;
        sourceBox.front     = 0;
        sourceBox.right     = sourceArea.right;
        sourceBox.bottom    = sourceArea.bottom;
        sourceBox.back      = 1;

        context_->CopySubresourceRegion(
            destination, mipLevel,
            destOffset.x, destOffset.y, 0,
            source, 0, &sourceBox
        );
    }
}

bool D3D11RenderContext::IsRectFullscreen(const Math::Recti& area) const
{
    return
        area.left   <= 0 &&
        area.top    <= 0 &&
        area.right  >= static_cast<int>(videoMode_.resolution.width) &&
        area.bottom >= static_cast<int>(videoMode_.resolution.height);
}


} // /namespace Video

} // /namespace Fork



// ========================