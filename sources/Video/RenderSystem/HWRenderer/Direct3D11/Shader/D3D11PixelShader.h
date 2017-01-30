/*
 * Direct3D 11 pixel shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_PIXEL_SHADER_H__
#define __FORK_D3D11_PIXEL_SHADER_H__


#include "Video/RenderSystem/Shader/PixelShader.h"
#include "D3D11ShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(D3D11PixelShader);

//! Direct3D 11 pixel shader implementation.
class D3D11PixelShader : public PixelShader, public D3D11ShaderBase
{
    
    public:
        
        D3D11PixelShader() = default;
        ~D3D11PixelShader();

        D3D11PixelShader(const D3D11PixelShader&) = delete;
        D3D11PixelShader& operator = (const D3D11PixelShader&) = delete;

        const char* ShaderTypeName() const;
        const char* TargetVersionPrefix() const;

        void BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot = 0);

        inline ID3D11PixelShader* GetShader() const
        {
            return shader_;
        }

    private:
        
        HRESULT CreateShader(ID3D11Device* device, ID3DBlob* buffer);

        ID3D11PixelShader* shader_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================