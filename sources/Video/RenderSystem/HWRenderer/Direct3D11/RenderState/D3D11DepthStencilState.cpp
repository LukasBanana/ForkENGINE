/*
 * Direct3D 11 depth stencil state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11DepthStencilState.h"
#include "../D3D11ParamMapper.h"
#include "../D3D11Core.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Video
{


using namespace D3D11ParamMapper;

D3D11DepthStencilState::D3D11DepthStencilState(ID3D11Device* device, const Description& desc) :
    DepthStencilState{ desc }
{
    ChangeDesc(device, desc);
}
D3D11DepthStencilState::~D3D11DepthStencilState()
{
    DXSafeRelease(stateObject_);
}

bool D3D11DepthStencilState::ChangeDesc(ID3D11Device* device, const Description& desc)
{
    ASSERT_POINTER(device);

    /* Setup state description */
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable        = DXBool(desc.isDepthTestEnabled);
    depthStencilDesc.DepthWriteMask     = (desc.isDepthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO);
    depthStencilDesc.DepthFunc          = Map(desc.depthFunc);
    
    depthStencilDesc.StencilEnable      = DXBool(desc.isStencilTestEnabled);
    depthStencilDesc.StencilReadMask    = desc.stencilReadMask;
    depthStencilDesc.StencilWriteMask   = desc.stencilWriteMask;

    auto MapStencilFace = [](D3D11_DEPTH_STENCILOP_DESC& out, const DepthStencilState::StencilOpDescription& in)
    {
        out.StencilFailOp       = Map( in.stencilFailOp  );
        out.StencilDepthFailOp  = Map( in.stencilZFailOp );
        out.StencilPassOp       = Map( in.stencilZPassOp );
        out.StencilFunc         = Map( in.stencilFunc    );
    };

    MapStencilFace( depthStencilDesc.FrontFace, desc.frontFace );
    MapStencilFace( depthStencilDesc.BackFace,  desc.backFace  );

    /* Create D3D11 state object */
    DXSafeRelease(stateObject_);
    auto result = device->CreateDepthStencilState(&depthStencilDesc, &stateObject_);

    if (!DXAssert(result, "Creating D3D11 depth-stencil state failed", false))
        return false;

    desc_ = desc;

    return true;
}


} // /namespace Video

} // /namespace Fork



// ========================