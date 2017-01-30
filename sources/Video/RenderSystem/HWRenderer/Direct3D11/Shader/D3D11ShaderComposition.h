/*
 * Direct3D 11 shader composition header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_SHADER_COMPOSITION_H__
#define __FORK_D3D11_SHADER_COMPOSITION_H__


#include "Video/RenderSystem/Shader/ShaderComposition.h"

#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "D3D11GeometryShader.h"
#include "D3D11TessControlShader.h"
#include "D3D11TessEvaluationShader.h"
#include "D3D11ComputeShader.h"


namespace Fork
{

namespace Video
{


//! Direct3D 11 shader composition implementation.
class D3D11ShaderComposition : public ShaderComposition
{
    
    public:
        
        D3D11ShaderComposition(ID3D11Device* device);
        ~D3D11ShaderComposition();

        D3D11ShaderComposition(const D3D11ShaderComposition&) = delete;
        D3D11ShaderComposition& operator = (const D3D11ShaderComposition&) = delete;

        /* --- Shader attachments --- */

        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const PixelShaderPtr& pixelShader
        );

        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const GeometryShaderPtr& geometryShader,
            const PixelShaderPtr& pixelShader
        );
        
        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const TessControlShaderPtr& tessControlShader,
            const TessEvaluationShaderPtr& tessEvaluationShader,
            const PixelShaderPtr& pixelShader
        );

        void AttachShader(
            const VertexShaderPtr& vertexShader,
            const TessControlShaderPtr& tessControlShader,
            const TessEvaluationShaderPtr& tessEvaluationShader,
            const GeometryShaderPtr& geometryShader,
            const PixelShaderPtr& pixelShader
        );

        void AttachShader(const ComputeShaderPtr& computeShader);

        /* --- Shader access --- */

        VertexShaderPtr         GetVertexShader         () const;
        PixelShaderPtr          GetPixelShader          () const;
        GeometryShaderPtr       GetGeometryShader       () const;
        TessControlShaderPtr    GetTessControlShader    () const;
        TessEvaluationShaderPtr GetTessEvaluationShader () const;
        ComputeShaderPtr        GetComputeShader        () const;

        /* --- Compilation and input layout --- */

        bool Compile(const Shader::CompilationFlags::DataType flags = 0);

        bool SetupVertexFormat(const VertexFormat& format);

        /* --- Constant buffers --- */

        ConstantBuffer::DescriptionListType QueryConstantBufferDescList() const;

        void Attach(const ConstantBufferPtr& constBuffer, unsigned int slot);
        void Detach(const ConstantBufferPtr& constBuffer);
        void DetachAllConstantBuffers();

        /* --- Storage buffers --- */

        //! \todo Not yet implemented
        StorageBuffer::DescriptionListType QueryStorageBufferDescList() const;

        /* --- Renderer specific functions --- */

        void BindShader(ID3D11DeviceContext* context) const;

    private:
        
        /* === Functions === */

        void DetachAllShaders();

        void QueryConstantBufferDescList(
            D3D11ShaderBase* shader, ConstantBuffer::DescriptionListType& descList
        ) const;

        //! \throws std::runtime_error If 'shader' is null or 'shader->shaderCode' is null.
        template <class T> void CompileSingleShader(const T& shader, UINT flags, bool disassembleCode);

        template <class Base, class Derived> void AttachSingleShader(
            std::shared_ptr<Derived>& dest, const std::shared_ptr<Base>& source, const std::string& baseTypeName
        );

        /* === Members === */

        ID3D11Device*                   device_         = nullptr;

        D3D11VertexShaderPtr            vertexShader_;
        D3D11PixelShaderPtr             pixelShader_;
        D3D11GeometryShaderPtr          geometryShader_;
        D3D11TessControlShaderPtr       tessControlShader_;
        D3D11TessEvaluationShaderPtr    tessEvaluationShader_;
        D3D11ComputeShaderPtr           computeShader_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================