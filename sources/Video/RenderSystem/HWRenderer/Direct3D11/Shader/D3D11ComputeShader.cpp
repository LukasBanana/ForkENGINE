/*
 * Direct3D 11 compute shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ComputeShader.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11ComputeShader::~D3D11ComputeShader()
{
    DXSafeRelease(shader_);
}

const char* D3D11ComputeShader::ShaderTypeName() const
{
    return "compute";
}

const char* D3D11ComputeShader::TargetVersionPrefix() const
{
    return "cs";
}

void D3D11ComputeShader::BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot)
{
    const auto& constBuffers = GetConstantBuffers();
    if (!constBuffers.empty())
        context->CSSetConstantBuffers(startSlot, constBuffers.size(), constBuffers.data());
    context->CSSetShader(shader_, nullptr, 0);
}


/*
 * ======= Private: =======
 */

HRESULT D3D11ComputeShader::CreateShader(ID3D11Device* device, ID3DBlob* buffer)
{
    return device->CreateComputeShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), nullptr, &shader_);
}


} // /namespace Video

} // /namespace Fork



// ========================