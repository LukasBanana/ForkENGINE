/*
 * Direct3D 11 pixel shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11PixelShader.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11PixelShader::~D3D11PixelShader()
{
    DXSafeRelease(shader_);
}

const char* D3D11PixelShader::ShaderTypeName() const
{
    return "pixel";
}

const char* D3D11PixelShader::TargetVersionPrefix() const
{
    return "ps";
}

void D3D11PixelShader::BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot)
{
    const auto& constBuffers = GetConstantBuffers();
    if (!constBuffers.empty())
        context->PSSetConstantBuffers(startSlot, constBuffers.size(), constBuffers.data());
    context->PSSetShader(shader_, nullptr, 0);
}


/*
 * ======= Private: =======
 */

HRESULT D3D11PixelShader::CreateShader(ID3D11Device* device, ID3DBlob* buffer)
{
    return device->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), nullptr, &shader_);
}


} // /namespace Video

} // /namespace Fork



// ========================