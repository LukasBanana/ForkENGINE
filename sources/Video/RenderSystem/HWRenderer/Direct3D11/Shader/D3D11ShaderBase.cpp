/*
 * Direct3D 11 shader base file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ShaderBase.h"
#include "../HardwareBuffer/D3D11ConstantBuffer.h"
#include "../D3D11Core.h"
#include "IO/Core/Log.h"
#include "Core/STLHelper.h"

#include <algorithm>
#include <d3dcompiler.h>


namespace Fork
{

namespace Video
{


/*
 * Internal functions
 */

static ConstantBuffer::DescriptionListType QueryConstantBufferDescList(ID3D11ShaderReflection* shaderReflector)
{
    ConstantBuffer::DescriptionListType descList;

    /* Query shader description */
    D3D11_SHADER_DESC shaderDesc;
    if (!DXAssert(shaderReflector->GetDesc(&shaderDesc), "Querying shader reflector description failed", false))
        return descList;
    
    /* Query description for each constant buffer */
    for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
    {
        /* Get current constant buffer reflector */
        auto cbufferReflector = shaderReflector->GetConstantBufferByIndex(i);
        if (!cbufferReflector)
        {
            IO::Log::Error("Getting constant buffer reflector (#" + ToStr(i) + ") failed");
            continue;
        }

        /* Query constant buffer description */
        D3D11_SHADER_BUFFER_DESC cbufferDesc;
        if (!DXAssert(cbufferReflector->GetDesc(&cbufferDesc), "Getting constant buffer description (#" + ToStr(i) + ") failed", false))
            continue;

        if (cbufferDesc.Type != D3D11_CT_CBUFFER)
            continue;

        /* Add new constant buffer to output list */
        ConstantBuffer::Description desc;
        {
            desc.name   = cbufferDesc.Name;
            desc.index  = i;
            desc.size   = cbufferDesc.Size;
        }
        descList[desc.name] = desc;
    }

    return descList;
}


/*
 * D3D11ShaderBase class
 */

D3D11ShaderBase::~D3D11ShaderBase()
{
}

HRESULT D3D11ShaderBase::Compile(
    ID3D11Device* device,
    const std::string& sourceCode,
    const std::string& entryPoint,
    const std::string& targetVersion,
    std::string& infoLog,
    UINT flags,
    const DisassemblyEvtHandler* disassemblyEvtHandler)
{
    ID3DBlob* buffer = nullptr;
    ID3DBlob* errors = nullptr;

    try
    {
        /*
        (Phase 1)
        Compile the shader:
        -> First convert the sources into a single program buffer (string)
        -> Then call "D3DCompile" to dynamically compile the shader
        */
        auto result = D3DCompile(
            sourceCode.c_str(),     // Shader source
            sourceCode.size(),      // Source length
            nullptr,                // Source name
            nullptr,                // Shader macros
            nullptr,                // Include file handler (optionally 'D3D_COMPILE_STANDARD_FILE_INCLUDE')
            entryPoint.c_str(),     // Entry point (shader's main function)
            targetVersion.c_str(),  // Target name (shader's version)
            flags,                  // Compile flags (flags1)
            0,                      // Effect flags (flags2)
            &buffer,                // Compiled output shader code
            &errors                 // Error messages
        );

        /* Check for errors */
        if (errors)
        {
            infoLog = reinterpret_cast<const char*>(errors->GetBufferPointer());
            DXSafeRelease(errors);
        }

        DXAssert(result, "Compiling " + std::string(ShaderTypeName()) + " shader failed");

        /*
        (Phase 2)
        Check if code must be disassembled for debug output:
        -> Post 'disassembly output' event to the shader composition and also pass the current shader object
        */
        if (disassemblyEvtHandler)
        {
            ID3DBlob* disassembly = nullptr;
            result = D3DDisassemble(buffer->GetBufferPointer(), buffer->GetBufferSize(), 0, nullptr, &disassembly);

            if (DXAssert(result, "Disassembling " + std::string(ShaderTypeName()) + " shader failed", false))
            {
                /* Get text from D3D buffer and post 'disassembly output' to event handler */
                const char* text = reinterpret_cast<const char*>(disassembly->GetBufferPointer());
                disassemblyEvtHandler->shaderComposition->PostDisassemblyOutput(
                    disassemblyEvtHandler->shader, text
                );
            }
        }

        /* Creates the D3D11 shader object */
        result = CreateShader(device, buffer);

        DXAssert(result, "Creating " + std::string(ShaderTypeName()) + " shader failed");

        /*
        (Phase 3)
        Query shader refelction:
        -> This is used to determine all shader information such as constant buffers
        */
        ID3D11ShaderReflection* shaderReflector = nullptr;

        result = D3DReflect(
            buffer->GetBufferPointer(),                 // Program source buffer
            buffer->GetBufferSize(),                    // Program source buffer size
            IID_ID3D11ShaderReflection,                 // Reference GUID to the ID3D11ShaderReflection interface
            reinterpret_cast<void**>(&shaderReflector)  // Output shader reflector
        );

        DXAssert(result, "Querying " + std::string(ShaderTypeName()) + " shader reflector failed");

        /* Query and store all constant buffer descriptions */
        constantBufferDescList_ = QueryConstantBufferDescList(shaderReflector);

        shaderReflector->Release();
    }
    catch (HRESULT err)
    {
        /* Return with error */
        DXSafeRelease(buffer);
        return err;
    }

    DXSafeRelease(buffer);

    return S_OK;
}

std::string D3D11ShaderBase::TargetVersion(const Shader::Versions version) const
{
    std::string targetVersion = TargetVersionPrefix();

    switch (version)
    {
        case Shader::Versions::HLSL_4_0:
            targetVersion += "_4_0";
            break;
        case Shader::Versions::HLSL_4_1:
            targetVersion += "_4_1";
            break;
        case Shader::Versions::HLSL_5_0:
            targetVersion += "_5_0";
            break;
        default:
            break;
    }

    return targetVersion;
}

void D3D11ShaderBase::AttachConstantBuffer(const D3D11ConstantBuffer* constBuffer)
{
    /* Check if specified constant buffer is used in this shader */
    const auto& cbufferDescList = GetConstantBufferDescList();

    auto it = cbufferDescList.find(constBuffer->GetDesc().name);
    if (it == cbufferDescList.end())
        return;

    /* Check if the buffer is already in the list */
    auto cbufferD3D = constBuffer->GetBuffer();
    if (std::find(constantBuffers_.begin(), constantBuffers_.end(), cbufferD3D) != constantBuffers_.end())
        return;

    /* Attach D3D hardware constant buffer to the list */
    constantBuffers_.push_back(cbufferD3D);
}

void D3D11ShaderBase::DetachConstantBuffer(const D3D11ConstantBuffer* constBuffer)
{
    RemoveFromList(constantBuffers_, constBuffer->GetBuffer());
}

void D3D11ShaderBase::DetachAllConstantBuffers()
{
    constantBuffers_.clear();
}


} // /namespace Video

} // /namespace Fork



// ========================