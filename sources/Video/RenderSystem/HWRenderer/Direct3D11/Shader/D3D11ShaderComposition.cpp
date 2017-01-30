/*
 * Direct3D 11 shader composition file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ShaderComposition.h"
#include "D3D11ShaderBase.h"
#include "../HardwareBuffer/D3D11ConstantBuffer.h"
#include "../D3D11Core.h"
#include "Video/RenderSystem/Shader/ShaderCode.h"
#include "IO/Core/Log.h"
#include "Core/Exception/PointerConversionException.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/CastToConcreteObject.h"
#include "Core/AuxiliaryMacros.h"


namespace Fork
{

namespace Video
{


/*
 * Internal functions
 */

//! Returns the Engine-To-DirectX shader compilation flags mapping list.
static std::vector<std::pair<Shader::CompilationFlags::DataType, UINT>> HLSLCompilerFlagsMap()
{
    typedef Shader::CompilationFlags ShaderFlags;
    return
    {
        { ShaderFlags::DebugInfo,               D3D10_SHADER_DEBUG                    },
        { ShaderFlags::SkipOptimization,        D3D10_SHADER_SKIP_OPTIMIZATION        },
        { ShaderFlags::SkipValidation,          D3D10_SHADER_SKIP_VALIDATION          },
        { ShaderFlags::DebugInfo,               D3D10_SHADER_DEBUG                    },
        { ShaderFlags::WarningsAreErrors,       D3D10_SHADER_WARNINGS_ARE_ERRORS      },
        { ShaderFlags::PreferFlowControl,       D3D10_SHADER_PREFER_FLOW_CONTROL      },
        { ShaderFlags::AvoidFlowControl,        D3D10_SHADER_AVOID_FLOW_CONTROL       },
        { ShaderFlags::PackMatrixRowMajor,      D3D10_SHADER_PACK_MATRIX_ROW_MAJOR    },
        { ShaderFlags::PackMatrixColumnMajor,   D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR },
        { ShaderFlags::O0,                      D3D10_SHADER_OPTIMIZATION_LEVEL0      },
        { ShaderFlags::O1,                      D3D10_SHADER_OPTIMIZATION_LEVEL1      },
        { ShaderFlags::O2,                      D3D10_SHADER_OPTIMIZATION_LEVEL2      },
        { ShaderFlags::O3,                      D3D10_SHADER_OPTIMIZATION_LEVEL3      },
    };
}

//! Converts the specified flags into HLSL compiler flags.
static UINT HLSLCompilerFlags(const Shader::CompilationFlags::DataType flags)
{
    UINT compilerFlags = 0;

    if (flags)
    {
        for (const auto& pair : HLSLCompilerFlagsMap())
        {
            if (flags & pair.first)
                compilerFlags |= pair.second;
        }
    }

    return compilerFlags;
}


/*
 * D3D11ShaderComposition class
 */

D3D11ShaderComposition::D3D11ShaderComposition(ID3D11Device* device) :
    device_{ device }
{
    ASSERT_POINTER(device);
    device_->AddRef();
}
D3D11ShaderComposition::~D3D11ShaderComposition()
{
    device_->Release();
}

/* --- Shader attachments --- */

void D3D11ShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to D3D11 render system objects */
    AttachSingleShader( vertexShader_,  vertexShader,   "VertexShader" );
    AttachSingleShader( pixelShader_,   pixelShader,    "PixelShader"  );
}

void D3D11ShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const GeometryShaderPtr& geometryShader, const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to D3D11 render system objects */
    AttachSingleShader( vertexShader_,      vertexShader,   "VertexShader"   );
    AttachSingleShader( geometryShader_,    geometryShader, "GeometryShader" );
    AttachSingleShader( pixelShader_,       pixelShader,    "PixelShader"    );
}

void D3D11ShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const TessControlShaderPtr& tessControlShader,
    const TessEvaluationShaderPtr& tessEvaluationShader, const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to D3D11 render system objects */
    AttachSingleShader( vertexShader_,          vertexShader,           "VertexShader"         );
    AttachSingleShader( tessControlShader_,     tessControlShader,      "TessControlShader"    );
    AttachSingleShader( tessEvaluationShader_,  tessEvaluationShader,   "TessEvaluationShader" );
    AttachSingleShader( pixelShader_,           pixelShader,            "PixelShader"          );
}

void D3D11ShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const TessControlShaderPtr& tessControlShader,
    const TessEvaluationShaderPtr& tessEvaluationShader, const GeometryShaderPtr& geometryShader,
    const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to D3D11 render system objects */
    AttachSingleShader( vertexShader_,          vertexShader,           "VertexShader"         );
    AttachSingleShader( geometryShader_,        geometryShader,         "GeometryShader"       );
    AttachSingleShader( tessControlShader_,     tessControlShader,      "TessControlShader"    );
    AttachSingleShader( tessEvaluationShader_,  tessEvaluationShader,   "TessEvaluationShader" );
    AttachSingleShader( pixelShader_,           pixelShader,            "PixelShader"          );
}

void D3D11ShaderComposition::AttachShader(const ComputeShaderPtr& computeShader)
{
    DetachAllShaders();

    /* Convert shared pointers to D3D11 render system objects */
    AttachSingleShader(computeShader_, computeShader, "ComputeShader");
}

/* --- Shader access --- */

VertexShaderPtr D3D11ShaderComposition::GetVertexShader() const
{
    return vertexShader_;
}

PixelShaderPtr D3D11ShaderComposition::GetPixelShader() const
{
    return pixelShader_;
}

GeometryShaderPtr D3D11ShaderComposition::GetGeometryShader() const
{
    return geometryShader_;
}

TessControlShaderPtr D3D11ShaderComposition::GetTessControlShader() const
{
    return tessControlShader_;
}

TessEvaluationShaderPtr D3D11ShaderComposition::GetTessEvaluationShader() const
{
    return tessEvaluationShader_;
}

ComputeShaderPtr D3D11ShaderComposition::GetComputeShader() const
{
    return computeShader_;
}

/* --- Compilation and input layout --- */

bool D3D11ShaderComposition::Compile(const Shader::CompilationFlags::DataType flags)
{
    hasCompiledSuccessful = true;

    try
    {
        /* Convert flags to HLSL compiler flags */
        auto compilerFlags = HLSLCompilerFlags(flags);
        bool disassembleCode = ((flags & Shader::CompilationFlags::DisassemblyOutput) != 0);

        /* Compile attached shaders */
        if (!computeShader_)
        {
            CompileSingleShader(vertexShader_, compilerFlags, disassembleCode);
            CompileSingleShader(pixelShader_, compilerFlags, disassembleCode);

            if (geometryShader_)
                CompileSingleShader(geometryShader_, compilerFlags, disassembleCode);

            if (tessControlShader_ && tessEvaluationShader_)
            {
                CompileSingleShader(tessControlShader_, compilerFlags, disassembleCode);
                CompileSingleShader(tessEvaluationShader_, compilerFlags, disassembleCode);
            }
        }
        else
            CompileSingleShader(computeShader_, compilerFlags, disassembleCode);
    }
    catch (const std::runtime_error& err)
    {
        IO::Log::Error(err.what());
    }

    return hasCompiledSuccessful;
}

bool D3D11ShaderComposition::SetupVertexFormat(const VertexFormat& format)
{
    if (vertexShader_)
        return vertexShader_->CreateInputLayoutDesc(format) == S_OK;
    return false;
}

/* --- Constant buffers --- */

ConstantBuffer::DescriptionListType D3D11ShaderComposition::QueryConstantBufferDescList() const
{
    ConstantBuffer::DescriptionListType descList;

    QueryConstantBufferDescList( vertexShader_          .get(), descList );
    QueryConstantBufferDescList( pixelShader_           .get(), descList );
    QueryConstantBufferDescList( geometryShader_        .get(), descList );
    QueryConstantBufferDescList( tessControlShader_     .get(), descList );
    QueryConstantBufferDescList( tessEvaluationShader_  .get(), descList );
    QueryConstantBufferDescList( computeShader_         .get(), descList );

    return descList;
}

void D3D11ShaderComposition::Attach(const ConstantBufferPtr& constBuffer, unsigned int slot)
{
    auto constBufferD3D = CAST_TO_D3D11_OBJECT(ConstantBuffer, constBuffer.get());

    ShaderComposition::Attach(constBuffer, slot);

    SAFE_CALL( vertexShader_         )->AttachConstantBuffer(constBufferD3D);
    SAFE_CALL( pixelShader_          )->AttachConstantBuffer(constBufferD3D);
    SAFE_CALL( geometryShader_       )->AttachConstantBuffer(constBufferD3D);
    SAFE_CALL( tessControlShader_    )->AttachConstantBuffer(constBufferD3D);
    SAFE_CALL( tessEvaluationShader_ )->AttachConstantBuffer(constBufferD3D);
    SAFE_CALL( computeShader_        )->AttachConstantBuffer(constBufferD3D);
}

void D3D11ShaderComposition::Detach(const ConstantBufferPtr& constBuffer)
{
    ShaderComposition::Detach(constBuffer);
    //...
}

void D3D11ShaderComposition::DetachAllConstantBuffers()
{
    ShaderComposition::DetachAllConstantBuffers();

    SAFE_CALL( vertexShader_         )->DetachAllConstantBuffers();
    SAFE_CALL( pixelShader_          )->DetachAllConstantBuffers();
    SAFE_CALL( geometryShader_       )->DetachAllConstantBuffers();
    SAFE_CALL( tessControlShader_    )->DetachAllConstantBuffers();
    SAFE_CALL( tessEvaluationShader_ )->DetachAllConstantBuffers();
    SAFE_CALL( computeShader_        )->DetachAllConstantBuffers();
}

/* --- Storage buffers --- */

StorageBuffer::DescriptionListType D3D11ShaderComposition::QueryStorageBufferDescList() const
{
    StorageBuffer::DescriptionListType descList;
    
    //!todo!

    return descList;
}

/* --- Renderer specific functions --- */

void D3D11ShaderComposition::BindShader(ID3D11DeviceContext* context) const
{
    if (vertexShader_ && pixelShader_)
    {
        context->IASetInputLayout(vertexShader_->GetInputLayout());

        vertexShader_   ->BindShaderAndConstantBuffers(context);
        pixelShader_    ->BindShaderAndConstantBuffers(context);

        if (geometryShader_)
            geometryShader_->BindShaderAndConstantBuffers(context);

        if (tessControlShader_ && tessEvaluationShader_)
        {
            tessControlShader_      ->BindShaderAndConstantBuffers(context);
            tessEvaluationShader_   ->BindShaderAndConstantBuffers(context);
        }
    }
    else if (computeShader_)
        computeShader_->BindShaderAndConstantBuffers(context);
}


/*
 * ======= Private: =======
 */

void D3D11ShaderComposition::DetachAllShaders()
{
    vertexShader_           .reset();
    pixelShader_            .reset();
    geometryShader_         .reset();
    tessControlShader_      .reset();
    tessEvaluationShader_   .reset();
    computeShader_          .reset();
}

void D3D11ShaderComposition::QueryConstantBufferDescList(
    D3D11ShaderBase* shader, ConstantBuffer::DescriptionListType& descList) const
{
    if (shader)
    {
        auto subDescList = shader->GetConstantBufferDescList();
        descList.insert(subDescList.begin(), subDescList.end());
    }
}

template <class T> void D3D11ShaderComposition::CompileSingleShader(
    const T& shader, UINT flags, bool disassembleCode)
{
    if (!shader || !shader->sourceCode)
        throw std::runtime_error("Missing code for " + std::string(shader->ShaderTypeName()) + " shader");

    /* Compile shader */
    D3D11ShaderBase::DisassemblyEvtHandler disassemblyEvtHandler { this, shader.get() };

    std::string infoLog;
    auto result = shader->Compile(
        device_,
        shader->sourceCode->ConvertToSingleBuffer(),
        shader->entryPoint,
        shader->TargetVersion(shader->targetVersion),
        infoLog,
        flags,
        (disassembleCode ? (&disassemblyEvtHandler) : nullptr)
    );

    /* Print info log */
    if (result != S_OK)
    {
        hasCompiledSuccessful = false;
        PrintInfoLog(infoLog, true);
    }
    else
        PrintInfoLog(infoLog, false);
}

template <class Base, class Derived> void D3D11ShaderComposition::AttachSingleShader(
    std::shared_ptr<Derived>& dest, const std::shared_ptr<Base>& source, const std::string& baseTypeName)
{
    /* Convert shared pointer to a GL render system object */
    dest = std::dynamic_pointer_cast<Derived>(source);
    if (!dest)
        throw PointerConversionException(__FUNCTION__, baseTypeName, "D3D11" + baseTypeName);
}


} // /namespace Video

} // /namespace Fork



// ========================