/*
 * Direct3D 11 vertex shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_VERTEX_SHADER_H__
#define __FORK_D3D11_VERTEX_SHADER_H__


#include "Video/RenderSystem/Shader/VertexShader.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "D3D11ShaderBase.h"

#include <vector>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(D3D11VertexShader);

//! Direct3D 11 vertex shader implementation.
class D3D11VertexShader : public VertexShader, public D3D11ShaderBase
{
    
    public:
        
        D3D11VertexShader() = default;
        ~D3D11VertexShader();

        D3D11VertexShader(const D3D11VertexShader&) = delete;
        D3D11VertexShader& operator = (const D3D11VertexShader&) = delete;

        const char* ShaderTypeName() const;
        const char* TargetVersionPrefix() const;

        HRESULT CreateInputLayoutDesc(const VertexFormat& format);
        HRESULT CreateInputLayout();

        void BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot = 0);

        inline ID3D11VertexShader* GetShader() const
        {
            return shader_;
        }
        inline ID3D11InputLayout* GetInputLayout() const
        {
            return inputLayout_;
        }

    private:
        
        //! Creats the vertex shader and input layout.
        HRESULT CreateShader(ID3D11Device* device, ID3DBlob* buffer);

        ID3D11VertexShader*                     shader_         = nullptr;
        ID3D11InputLayout*                      inputLayout_    = nullptr;

        std::vector<D3D11_INPUT_ELEMENT_DESC>   inputLayoutDesc_;
        std::vector<std::string>                inputLayoutSemanticNames_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================