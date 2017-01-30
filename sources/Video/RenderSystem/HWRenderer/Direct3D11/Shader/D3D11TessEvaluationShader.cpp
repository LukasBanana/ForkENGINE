/*
 * Direct3D 11 tessellation evaluation header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11TessEvaluationShader.h"
#include "../D3D11Core.h"


namespace Fork
{

namespace Video
{


D3D11TessEvaluationShader::~D3D11TessEvaluationShader()
{
    DXSafeRelease(shader_);
}

const char* D3D11TessEvaluationShader::ShaderTypeName() const
{
    return "domain";
}

const char* D3D11TessEvaluationShader::TargetVersionPrefix() const
{
    return "ds";
}

void D3D11TessEvaluationShader::BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot)
{
    const auto& constBuffers = GetConstantBuffers();
    if (!constBuffers.empty())
        context->DSSetConstantBuffers(startSlot, constBuffers.size(), constBuffers.data());
    context->DSSetShader(shader_, nullptr, 0);
}


/*
 * ======= Private: =======
 */

HRESULT D3D11TessEvaluationShader::CreateShader(ID3D11Device* device, ID3DBlob* buffer)
{
    return device->CreateDomainShader(buffer->GetBufferPointer(), buffer->GetBufferSize(), nullptr, &shader_);
}


} // /namespace Video

} // /namespace Fork



// ========================