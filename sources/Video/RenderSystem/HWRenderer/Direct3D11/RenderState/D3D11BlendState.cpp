/*
 * Direct3D 11 blend state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11BlendState.h"
#include "../D3D11ParamMapper.h"
#include "../D3D11Core.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Video
{


using namespace D3D11ParamMapper;

D3D11BlendState::D3D11BlendState(ID3D11Device* device, const Description& desc) :
    BlendState{ desc }
{
    ChangeDesc(device, desc);
}
D3D11BlendState::~D3D11BlendState()
{
    DXSafeRelease(stateObject_);
}

bool D3D11BlendState::ChangeDesc(ID3D11Device* device, const Description& desc)
{
    ASSERT_POINTER(device);

    /* Setup sample bit-mask */
    sampleMask_ = 0;

    if (desc.colorMask.r) sampleMask_ |= 0xff000000;
    if (desc.colorMask.g) sampleMask_ |= 0x00ff0000;
    if (desc.colorMask.b) sampleMask_ |= 0x0000ff00;
    if (desc.colorMask.a) sampleMask_ |= 0x000000ff;

    /* Map parameters */
    auto srcBlend       = Map( desc.srcBlend       );
    auto destBlend      = Map( desc.destBlend      );
    auto srcBlendAlpha  = Map( desc.srcBlendAlpha  );
    auto destBlendAlpha = Map( desc.destBlendAlpha );

    /* Setup state description */
    D3D11_BLEND_DESC blendDesc;
    InitRawMemory(blendDesc);

    blendDesc.AlphaToCoverageEnable     = FALSE;
    blendDesc.IndependentBlendEnable    = FALSE;

    /* Only setup first render target blend state (IndependentBlendEnable == FALSE) */
    auto& targetDesc = blendDesc.RenderTarget[0];

    targetDesc.BlendEnable              = DXBool(desc.isBlendEnabled);
        
    targetDesc.SrcBlend                 = srcBlend;
    targetDesc.DestBlend                = destBlend;
    targetDesc.BlendOp                  = D3D11_BLEND_OP_ADD;

    targetDesc.SrcBlendAlpha            = srcBlendAlpha;
    targetDesc.DestBlendAlpha           = destBlendAlpha;
    targetDesc.BlendOpAlpha             = D3D11_BLEND_OP_ADD;

    targetDesc.RenderTargetWriteMask    = D3D11_COLOR_WRITE_ENABLE_ALL;

    /* Create D3D11 state object */
    DXSafeRelease(stateObject_);
    auto result = device->CreateBlendState(&blendDesc, &stateObject_);

    if (!DXAssert(result, "Creating D3D11 blend state failed", false))
        return false;

    desc_ = desc;

    return true;
}


} // /namespace Video

} // /namespace Fork



// ========================