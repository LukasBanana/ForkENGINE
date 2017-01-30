/*
 * Direct3D 11 geometry shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_GEOMETRY_SHADER_H__
#define __FORK_D3D11_GEOMETRY_SHADER_H__


#include "Video/RenderSystem/Shader/GeometryShader.h"
#include "D3D11ShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(D3D11GeometryShader);

//! Direct3D 11 geometry shader implementation.
class D3D11GeometryShader : public GeometryShader, public D3D11ShaderBase
{
    
    public:
        
        D3D11GeometryShader() = default;
        ~D3D11GeometryShader();

        D3D11GeometryShader(const D3D11GeometryShader&) = delete;
        D3D11GeometryShader& operator = (const D3D11GeometryShader&) = delete;

        const char* ShaderTypeName() const;
        const char* TargetVersionPrefix() const;

        void BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot = 0);

        inline ID3D11GeometryShader* GetShader() const
        {
            return shader_;
        }

    private:
        
        HRESULT CreateShader(ID3D11Device* device, ID3DBlob* buffer);

        ID3D11GeometryShader* shader_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================