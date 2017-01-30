/*
 * Direct3D 11 tessellation control shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_TESS_CONTROL_SHADER_H__
#define __FORK_D3D11_TESS_CONTROL_SHADER_H__


#include "Video/RenderSystem/Shader/TessControlShader.h"
#include "D3D11ShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(D3D11TessControlShader);

//! Direct3D 11 tessellation control shader implementation.
class D3D11TessControlShader : public TessControlShader, public D3D11ShaderBase
{
    
    public:
        
        D3D11TessControlShader() = default;
        ~D3D11TessControlShader();

        D3D11TessControlShader(const D3D11TessControlShader&) = delete;
        D3D11TessControlShader& operator = (const D3D11TessControlShader&) = delete;

        const char* ShaderTypeName() const;
        const char* TargetVersionPrefix() const;

        void BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot = 0);

        inline ID3D11HullShader* GetShader() const
        {
            return shader_;
        }

    private:
        
        HRESULT CreateShader(ID3D11Device* device, ID3DBlob* buffer);

        ID3D11HullShader* shader_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================