/*
 * Direct3D 11 tessellation control header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11TessControlShader.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11TessControlShader::~D3D11TessControlShader()
{
    DXSafeRelease(shader_);
}

const char* D3D11TessControlShader::ShaderTypeName() const
{
    return "hull";
}

const char* D3D11TessControlShader::TargetVersionPrefix() const
{
    return "hs";
}

void D3D11TessControlShader::BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot)
{
    const auto& constBuffers = GetConstantBuffers();
    if (!constBuffers.empty())
        context->HSSetConstantBuffers(startSlot, constBuffers.size(), constBuffers.data());
    context->HSSetShader(shader_, nullptr, 0);
}


/*
 * ======= Private: =======
 */

HRESULT D3D11TessControlShader::CreateShader(ID3D11Device* device, ID3DBlob* buffer)
{
    return device->CreateHullShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), nullptr, &shader_);
}


} // /namespace Video

} // /namespace Fork



// ========================