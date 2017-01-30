/*
 * Direct3D 11 rasterizer state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11RasterizerState.h"
#include "../D3D11ParamMapper.h"
#include "../D3D11Core.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Video
{


using namespace D3D11ParamMapper;

D3D11RasterizerState::D3D11RasterizerState(ID3D11Device* device, const Description& desc) :
    RasterizerState{ desc }
{
    ChangeDesc(device, desc);
}
D3D11RasterizerState::~D3D11RasterizerState()
{
    DXSafeRelease(stateObject_);
}

bool D3D11RasterizerState::ChangeDesc(ID3D11Device* device, const Description& desc)
{
    ASSERT_POINTER(device);

    /* Setup state description */
    D3D11_RASTERIZER_DESC rasterizerDesc;
    InitRawMemory(rasterizerDesc);

    rasterizerDesc.FillMode                 = Map(desc.fillMode);
    rasterizerDesc.CullMode                 = Map(desc.cullMode);
    rasterizerDesc.FrontCounterClockwise    = DXBool(desc.isFrontCCW);
    
    rasterizerDesc.DepthBias                = desc.depthBias;
    rasterizerDesc.DepthBiasClamp           = desc.depthBiasClamp;
    rasterizerDesc.SlopeScaledDepthBias     = desc.slopeScaledDepthBias;
    
    rasterizerDesc.DepthClipEnable          = DXBool( desc.isDepthClipEnabled      );
    rasterizerDesc.ScissorEnable            = DXBool( desc.isScissorTestEnabled    );
    rasterizerDesc.MultisampleEnable        = DXBool( desc.isMultiSampleEnable     );
    rasterizerDesc.AntialiasedLineEnable    = DXBool( desc.isAntiAliasedLineEnable );

    /* Create D3D11 state object */
    DXSafeRelease(stateObject_);
    auto result = device->CreateRasterizerState(&rasterizerDesc, &stateObject_);

    if (!DXAssert(result, "Creating D3D11 rasterizer state failed", false))
        return false;

    desc_ = desc;

    return true;
}


} // /namespace Video

} // /namespace Fork



// ========================