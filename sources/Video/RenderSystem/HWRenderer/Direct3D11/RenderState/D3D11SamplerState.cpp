/*
 * Direct3D 11 sampler state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11SamplerState.h"
#include "../D3D11ParamMapper.h"
#include "../D3D11Core.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Video
{


using namespace D3D11ParamMapper;

D3D11SamplerState::D3D11SamplerState(ID3D11Device* device, const Description& desc) :
    SamplerState{ desc }
{
    ChangeDesc(device, desc);
}
D3D11SamplerState::~D3D11SamplerState()
{
    DXSafeRelease(stateObject_);
}

bool D3D11SamplerState::ChangeDesc(ID3D11Device* device, const Description& desc)
{
    ASSERT_POINTER(device);

    /* Setup state description */
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));

    if (desc.anisotropy > 1)
        samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    else
        samplerDesc.Filter = D3D11ParamMapper::Map(desc.minFilter, desc.magFilter, desc.mipFilter);

    samplerDesc.AddressU        = D3D11ParamMapper::Map(desc.wrapMode.u);
    samplerDesc.AddressV        = D3D11ParamMapper::Map(desc.wrapMode.v);
    samplerDesc.AddressW        = D3D11ParamMapper::Map(desc.wrapMode.w);
    samplerDesc.MipLODBias      = desc.mipLODBias;
    samplerDesc.MaxAnisotropy   = desc.anisotropy;
    samplerDesc.ComparisonFunc  = D3D11_COMPARISON_NEVER;
    
    for (int i = 0; i < 4; ++i)
        samplerDesc.BorderColor[i] = 0.0f;

    if (desc.useMIPMaps)
    {
        samplerDesc.MinLOD = desc.minLOD;
        samplerDesc.MaxLOD = desc.maxLOD;
    }
    else
    {
        samplerDesc.MinLOD = 0.0f;
        samplerDesc.MaxLOD = 0.0f;
    }

    /* Create D3D11 state object */
    DXSafeRelease(stateObject_);
    auto result = device->CreateSamplerState(&samplerDesc, &stateObject_);

    if (!DXAssert(result, "Creating D3D11 sampler state failed", false))
        return false;

    desc_ = desc;

    return true;
}


} // /namespace Video

} // /namespace Fork



// ========================