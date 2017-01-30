/*
 * Direct3D 11 compute shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_COMPUTE_SHADER_H__
#define __FORK_D3D11_COMPUTE_SHADER_H__


#include "Video/RenderSystem/Shader/ComputeShader.h"
#include "D3D11ShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(D3D11ComputeShader);

//! Direct3D 11 compute shader implementation.
class D3D11ComputeShader : public ComputeShader, public D3D11ShaderBase
{
    
    public:
        
        D3D11ComputeShader() = default;
        ~D3D11ComputeShader();

        D3D11ComputeShader(const D3D11ComputeShader&) = delete;
        D3D11ComputeShader& operator = (const D3D11ComputeShader&) = delete;

        const char* ShaderTypeName() const;
        const char* TargetVersionPrefix() const;

        void BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot = 0);

        inline ID3D11ComputeShader* GetShader() const
        {
            return shader_;
        }

    private:
        
        HRESULT CreateShader(ID3D11Device* device, ID3DBlob* buffer);

        ID3D11ComputeShader* shader_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================