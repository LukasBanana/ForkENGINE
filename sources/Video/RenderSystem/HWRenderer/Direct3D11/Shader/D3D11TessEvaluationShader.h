/*
 * Direct3D 11 tessellation evaluation shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_TESS_EVALUATION_SHADER_H__
#define __FORK_D3D11_TESS_EVALUATION_SHADER_H__


#include "Video/RenderSystem/Shader/TessEvaluationShader.h"
#include "D3D11ShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(D3D11TessEvaluationShader);

//! Direct3D 11 tessellation evaluation shader implementation.
class D3D11TessEvaluationShader : public TessEvaluationShader, public D3D11ShaderBase
{
    
    public:
        
        D3D11TessEvaluationShader() = default;
        ~D3D11TessEvaluationShader();

        D3D11TessEvaluationShader(const D3D11TessEvaluationShader&) = delete;
        D3D11TessEvaluationShader& operator = (const D3D11TessEvaluationShader&) = delete;

        const char* ShaderTypeName() const;
        const char* TargetVersionPrefix() const;

        void BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot = 0);

        inline ID3D11DomainShader* GetShader() const
        {
            return shader_;
        }

    private:
        
        HRESULT CreateShader(ID3D11Device* device, ID3DBlob* buffer);

        ID3D11DomainShader* shader_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================