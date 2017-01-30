/*
 * Direct3D 11 vertex shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11VertexShader.h"
#include "../D3D11Core.h"
#include "../D3D11ParamMapper.h"
#include "IO/Core/Log.h"

#include <map>


namespace Fork
{

namespace Video
{


D3D11VertexShader::~D3D11VertexShader()
{
    DXSafeRelease( inputLayout_ );
    DXSafeRelease( shader_      );
}

const char* D3D11VertexShader::ShaderTypeName() const
{
    return "vertex";
}

const char* D3D11VertexShader::TargetVersionPrefix() const
{
    return "vs";
}

typedef std::map<std::string, UINT> SemanticNameIndexMap;

/*
Internal function to manage the semantic index
for the specified input element name.
*/
static UINT SemanticNameIndex(const std::string& semanticName, SemanticNameIndexMap& semanticNameIndices)
{
    return semanticNameIndices[semanticName]++;
}

/*
Internal function to setup input element description
for the specified vertex format attribute.
*/
static void SetupInputElementDesc(
    D3D11_INPUT_ELEMENT_DESC& elementDesc, std::string& semanticName,
    const VertexFormat::Attribute& attrib, SemanticNameIndexMap& semanticNameIndices)
{
    /* Store semantic name to have a consistent string memory */
    semanticName = attrib.name;

    /* Initialize input element description */
    elementDesc.SemanticName            = semanticName.c_str();
    elementDesc.SemanticIndex           = SemanticNameIndex(semanticName, semanticNameIndices);
    elementDesc.Format                  = D3D11ParamMapper::Map(attrib.GetDataType(), attrib.GetNumComponents(), false);
    elementDesc.InputSlot               = 0;
    elementDesc.AlignedByteOffset       = attrib.GetOffset();
    elementDesc.InputSlotClass          = D3D11_INPUT_PER_VERTEX_DATA;
    elementDesc.InstanceDataStepRate    = 0;

    if (elementDesc.Format == DXGI_FORMAT_UNKNOWN)
        throw std::runtime_error("Invalid vertex attribute format \"" + semanticName + "\" for D3D11 input layout");
}

HRESULT D3D11VertexShader::CreateInputLayoutDesc(const VertexFormat& format)
{
    /* Setup input element description for each format attribute */
    const auto& attributes = format.GetAttributes();

    const auto numAttribs = attributes.size();
    inputLayoutDesc_.resize(numAttribs);
    inputLayoutSemanticNames_.resize(numAttribs);

    SemanticNameIndexMap semanticNameIndices;

    try
    {
        for (size_t i = 0; i < numAttribs; ++i)
        {
            SetupInputElementDesc(
                inputLayoutDesc_[i], inputLayoutSemanticNames_[i],
                attributes[i], semanticNameIndices
            );
        }
    }
    catch (const std::runtime_error& err)
    {
        IO::Log::Error(err.what());
        return E_INVALIDARG;
    }

    return S_OK;
}

void D3D11VertexShader::BindShaderAndConstantBuffers(ID3D11DeviceContext* context, UINT startSlot)
{
    const auto& constBuffers = GetConstantBuffers();
    if (!constBuffers.empty())
        context->VSSetConstantBuffers(startSlot, constBuffers.size(), constBuffers.data());
    context->VSSetShader(shader_, nullptr, 0);
}


/*
 * ======= Private: =======
 */

HRESULT D3D11VertexShader::CreateShader(ID3D11Device* device, ID3DBlob* buffer)
{
    /* Create D3D vertex shader */
    auto result = device->CreateVertexShader(
        buffer->GetBufferPointer(),
        buffer->GetBufferSize(),
        nullptr,
        &shader_
    );

    if (!inputLayoutDesc_.empty())
    {
        /* Create D3D input layout */
        auto resultLayout = device->CreateInputLayout(
            inputLayoutDesc_.data(),
            inputLayoutDesc_.size(),
            buffer->GetBufferPointer(),
            buffer->GetBufferSize(),
            &inputLayout_
        );

        DXAssert(resultLayout, "Creating D3D11 input layout failed", false);
    }

    return result;
}


} // /namespace Video

} // /namespace Fork



// ========================