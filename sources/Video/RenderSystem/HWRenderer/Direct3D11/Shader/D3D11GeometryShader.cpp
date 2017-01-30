/*
 * Direct3D 11 geometry shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11GeometryShader.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11GeometryShader::~D3D11GeometryShader()
{
    DXSafeRelease(shader_);
}

const char* D3D11GeometryShader::ShaderTypeName() const
{
    return "geometry";
}

const char* D3D11GeometryShader::TargetVersionPrefix() const
{
    return "gs";
}

void D3D11GeometryShader::BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot)
{
    const auto& constBuffers = GetConstantBuffers();
    if (!constBuffers.empty())
        context->GSSetConstantBuffers(startSlot, constBuffers.size(), constBuffers.data());
    context->GSSetShader(shader_, nullptr, 0);
}


/*
 * ======= Private: =======
 */

HRESULT D3D11GeometryShader::CreateShader(ID3D11Device* device, ID3DBlob* buffer)
{
    return device->CreateGeometryShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), nullptr, &shader_);
}


} // /namespace Video

} // /namespace Fork



// ========================