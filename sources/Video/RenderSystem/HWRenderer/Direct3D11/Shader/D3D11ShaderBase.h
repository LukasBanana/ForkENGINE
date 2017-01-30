/*
 * Direct3D 11 shader base header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_SHADER_BASE_H__
#define __FORK_D3D11_SHADER_BASE_H__


#include "Video/RenderSystem/Shader/Shader.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/HardwareBuffer/ConstantBuffer.h"

#include <vector>
#include <d3d11.h>
#include <d3d11shader.h>


namespace Fork
{

namespace Video
{


class D3D11ConstantBuffer;

//! Direct3D 11 shader base class.
class D3D11ShaderBase
{
    
    public:
        
        virtual ~D3D11ShaderBase();

        D3D11ShaderBase(const D3D11ShaderBase&) = delete;
        D3D11ShaderBase& operator = (const D3D11ShaderBase&) = delete;

        //! Returns the shader type name ("vertex", "pixel" etc.).
        virtual const char* ShaderTypeName() const = 0;
        //! Returns the shader target version prefix ("vs", "ps" etc.).
        virtual const char* TargetVersionPrefix() const = 0;

    protected:
        
        friend class D3D11ShaderComposition;

        struct DisassemblyEvtHandler
        {
            const ShaderComposition*    shaderComposition;
            const Shader*               shader;
        };

        D3D11ShaderBase() = default;

        HRESULT Compile(
            ID3D11Device* device,
            const std::string& sourceCode,
            const std::string& entryPoint,
            const std::string& targetVersion,
            std::string& infoLog,
            UINT flags = 0,
            const DisassemblyEvtHandler* disassemblyEvtHandler = nullptr
        );

        //! Returns the respective shader target version (e.g. "ps_4_0").
        std::string TargetVersion(const Shader::Versions version) const;

        void AttachConstantBuffer(const D3D11ConstantBuffer* constBuffer);
        void DetachConstantBuffer(const D3D11ConstantBuffer* constBuffer);
        void DetachAllConstantBuffers();

        //! Creates the D3D shader object with the specified shader buffer.
        virtual HRESULT CreateShader(ID3D11Device* device, ID3DBlob* buffer) = 0;

        //! Binds the shader and all constant buffers for the respective shader stage.
        virtual void BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot = 0) = 0;

        //! Returns the constant buffer description list for this shader.
        inline const ConstantBuffer::DescriptionListType& GetConstantBufferDescList() const
        {
            return constantBufferDescList_;
        }
        //! Returns the list of all D3D hardware constant buffers.
        inline const std::vector<ID3D11Buffer*>& GetConstantBuffers() const
        {
            return constantBuffers_;
        }

    private:
        
        ConstantBuffer::DescriptionListType constantBufferDescList_;
        std::vector<ID3D11Buffer*>          constantBuffers_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================