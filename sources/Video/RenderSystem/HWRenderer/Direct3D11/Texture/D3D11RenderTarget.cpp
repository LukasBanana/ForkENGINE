/*
 * Direct3D 11 render target file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11RenderTarget.h"
#include "../D3D11RenderSystem.h"
#include "D3D11Texture1D.h"
#include "D3D11Texture2D.h"
#include "D3D11Texture3D.h"
#include "D3D11TextureCube.h"
#include "Core/CastToConcreteObject.h"
#include "../D3D11Core.h"
#include "../D3D11ParamMapper.h"
#include "IO/Core/Log.h"
#include "Core/Exception/PointerConversionException.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Video
{


D3D11RenderTarget::D3D11RenderTarget(D3D11RenderSystem* renderSystem) :
    renderSystem_(renderSystem)
{
    ASSERT_POINTER(renderSystem);
}
D3D11RenderTarget::~D3D11RenderTarget()
{
    Clear();
}

bool D3D11RenderTarget::Attach(
    const Texture1DPtr& texture, unsigned int attachment, unsigned int layer, unsigned int mipLevel)
{
    //...
    return false;
}

bool D3D11RenderTarget::Attach(
    const Texture2DPtr& texture, unsigned int attachment, unsigned int layer, unsigned int mipLevel)
{
    /* Validate input parameters */
    if (!ValidateMIPLevelInput(mipLevel, texture->HasMIPMaps()))
        return false;
    if (layer > 0 && layer >= texture->GetArraySize())
        throw IndexOutOfBoundsException(__FUNCTION__, layer);

    /* Get D3D texture */
    auto textureD3D = CAST_TO_D3D11_OBJECT(Texture2D, texture.get());
    
    /* Setup render target view description */
    D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
    InitRawMemory(viewDesc);
    
    viewDesc.Format = D3D11ParamMapper::Map(textureD3D->GetFormat());

    if (texture->HasArray())
    {
        viewDesc.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        viewDesc.Texture2DArray.MipSlice        = mipLevel;
        viewDesc.Texture2DArray.FirstArraySlice = layer;
        viewDesc.Texture2DArray.ArraySize       = 1;
    }
    else
    {
        viewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipSlice = mipLevel;
    }

    /* Create render target view */
    auto renderTargetView = CreateRenderTargetView(textureD3D->AbstractResource(), viewDesc);
    if (!renderTargetView)
        return false;

    frameBuffer_.renderTargetViews.push_back(renderTargetView);

    /* Create depth texture (if there is none) */
    if (!depthStencil_)
        CreateDepthTexture(texture->GetSize().Cast<unsigned int>());
    
    return true;
}

bool D3D11RenderTarget::Attach(
    const Texture3DPtr& texture, unsigned int attachment, unsigned int zOffset, unsigned int mipLevel)
{
    //...
    return false;
}

bool D3D11RenderTarget::Attach(
    const TextureCubePtr& texture, const TextureCube::Faces face, unsigned int attachment, unsigned int layer, unsigned int mipLevel)
{
    /* Validate input parameters */
    if (!ValidateMIPLevelInput(mipLevel, texture->HasMIPMaps()))
        return false;
    if (layer > 0 && layer >= texture->GetArraySize())
        throw IndexOutOfBoundsException(__FUNCTION__, layer);

    /* Get D3D texture */
    auto textureD3D = CAST_TO_D3D11_OBJECT(TextureCube, texture.get());
    
    /* Setup render target view description */
    D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
    InitRawMemory(viewDesc);
    
    viewDesc.Format = D3D11ParamMapper::Map(textureD3D->GetFormat());

    viewDesc.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    viewDesc.Texture2DArray.MipSlice        = mipLevel;
    viewDesc.Texture2DArray.FirstArraySlice = layer*6 + static_cast<UINT>(face);
    viewDesc.Texture2DArray.ArraySize       = 1;

    /* Create render target view */
    auto renderTargetView = CreateRenderTargetView(textureD3D->AbstractResource(), viewDesc);
    if (!renderTargetView)
        return false;

    frameBuffer_.renderTargetViews.push_back(renderTargetView);

    /* Create depth texture (if there is none) */
    if (!depthStencil_)
        CreateDepthTexture(texture->GetSize().Cast<unsigned int>());
    
    return true;
}

void D3D11RenderTarget::Clear()
{
    /* Release all render target resources */
    DXSafeReleaseList(unorderedAccessViews_);
    DXSafeReleaseList(frameBuffer_.renderTargetViews);
}

void D3D11RenderTarget::SetupMultiSamples(int numSamples)
{
    #if 0
    if (multiSamples_ != numSamples)
    {
        /* Store new multi-sampling setting */
        multiSamples_ = numSamples;

        //!TODO -> update frame buffers ...
    }
    #endif
}


/*
 * ======= Private: =======
 */

ID3D11RenderTargetView* D3D11RenderTarget::CreateRenderTargetView(
    ID3D11Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC& desc)
{
    return renderSystem_->CreateRenderTargetView(resource, &desc);
}

bool D3D11RenderTarget::CreateDepthTexture(const Math::Size2ui& resolution)
{
    /* Release previous depth texture and DSV */
    ReleaseDepthTexture();

    /* Create depth stencil */
    auto numSamples = static_cast<unsigned int>(std::max(1, GetMultiSamples()));

    depthStencil_ = renderSystem_->CreateDepthStencil(resolution, numSamples);
    if (!depthStencil_)
        return false;

    /* Create depth stencil view */
    frameBuffer_.depthStencilView = renderSystem_->CreateDepthStencilView(depthStencil_, HasMultiSampling());
    if (!frameBuffer_.depthStencilView)
        return false;

    return true;
}

void D3D11RenderTarget::ReleaseDepthTexture()
{
    DXSafeRelease(depthStencil_);
    DXSafeRelease(frameBuffer_.depthStencilView);
}



} // /namespace Video

} // /namespace Fork



// ========================