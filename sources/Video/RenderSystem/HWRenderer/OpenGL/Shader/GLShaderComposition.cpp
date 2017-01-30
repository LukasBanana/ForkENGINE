/*
 * OpenGL shader composition file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLShaderComposition.h"
#include "../HardwareBuffer/GLConstantBuffer.h"
#include "../HardwareBuffer/GLStorageBuffer.h"
#include "../GLExtensions.h"
#include "../GLCore.h"
#include "Video/RenderSystem/Shader/ShaderCode.h"
#include "Core/Exception/PointerConversionException.h"
#include "Core/CastToConcreteObject.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Video
{


GLShaderComposition::GLShaderComposition() :
    programID_{ 0 }
{
    programID_ = glCreateProgram();
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glCreateProgram", programID_);
}
GLShaderComposition::~GLShaderComposition()
{
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glDeleteProgram", programID_);
    glDeleteProgram(programID_);
}

/* --- Shader attachments --- */

void GLShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to GL render system objects */
    AttachSingleShader( vertexShader_,  vertexShader,   "VertexShader" );
    AttachSingleShader( pixelShader_,   pixelShader,    "PixelShader"  );
}

void GLShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const GeometryShaderPtr& geometryShader, const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to GL render system objects */
    AttachSingleShader( vertexShader_,      vertexShader,   "VertexShader"   );
    AttachSingleShader( geometryShader_,    geometryShader, "GeometryShader" );
    AttachSingleShader( pixelShader_,       pixelShader,    "PixelShader"    );
}

void GLShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const TessControlShaderPtr& tessControlShader,
    const TessEvaluationShaderPtr& tessEvaluationShader, const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to GL render system objects */
    AttachSingleShader( vertexShader_,          vertexShader,           "VertexShader"         );
    AttachSingleShader( tessControlShader_,     tessControlShader,      "TessControlShader"    );
    AttachSingleShader( tessEvaluationShader_,  tessEvaluationShader,   "TessEvaluationShader" );
    AttachSingleShader( pixelShader_,           pixelShader,            "PixelShader"          );
}

void GLShaderComposition::AttachShader(
    const VertexShaderPtr& vertexShader, const TessControlShaderPtr& tessControlShader,
    const TessEvaluationShaderPtr& tessEvaluationShader, const GeometryShaderPtr& geometryShader,
    const PixelShaderPtr& pixelShader)
{
    DetachAllShaders();

    /* Convert shared pointers to GL render system objects */
    AttachSingleShader( vertexShader_,          vertexShader,           "VertexShader"         );
    AttachSingleShader( geometryShader_,        geometryShader,         "GeometryShader"       );
    AttachSingleShader( tessControlShader_,     tessControlShader,      "TessControlShader"    );
    AttachSingleShader( tessEvaluationShader_,  tessEvaluationShader,   "TessEvaluationShader" );
    AttachSingleShader( pixelShader_,           pixelShader,            "PixelShader"          );
}

void GLShaderComposition::AttachShader(const ComputeShaderPtr& computeShader)
{
    DetachAllShaders();

    /* Convert shared pointers to GL render system objects */
    AttachSingleShader(computeShader_, computeShader, "ComputeShader");
}

/* --- Shader access --- */

VertexShaderPtr GLShaderComposition::GetVertexShader() const
{
    return vertexShader_;
}

PixelShaderPtr GLShaderComposition::GetPixelShader() const
{
    return pixelShader_;
}

GeometryShaderPtr GLShaderComposition::GetGeometryShader() const
{
    return geometryShader_;
}

TessControlShaderPtr GLShaderComposition::GetTessControlShader() const
{
    return tessControlShader_;
}

TessEvaluationShaderPtr GLShaderComposition::GetTessEvaluationShader() const
{
    return tessEvaluationShader_;
}

ComputeShaderPtr GLShaderComposition::GetComputeShader() const
{
    return computeShader_;
}

/* --- Compilation and input layout --- */

bool GLShaderComposition::Compile(const Shader::CompilationFlags::DataType flags)
{
    /* Compile attached shaders */
    if (!computeShader_)
    {
        CompileSingleShader(vertexShader_);
        CompileSingleShader(pixelShader_);

        if (geometryShader_)
            CompileSingleShader(geometryShader_);

        if (tessControlShader_ && tessEvaluationShader_)
        {
            CompileSingleShader(tessControlShader_);
            CompileSingleShader(tessEvaluationShader_);
        }
    }
    else
        CompileSingleShader(computeShader_);

    /* Link program */
    FLUSH_GL_ERROR;

    glLinkProgram(programID_);

    SHOW_GL_ERROR("glLinkProgram");

    /* Check for link errors */
    std::string infoLog;
    hasCompiledSuccessful = QueryInfoLog(infoLog);

    /* Print info log */
    PrintInfoLog(infoLog, !hasCompiledSuccessful);

    return hasCompiledSuccessful;
}

bool GLShaderComposition::SetupVertexFormat(const VertexFormat& format)
{
    /* Check for valid arguments */
    if (format.GetAttributes().size() > GL_MAX_VERTEX_ATTRIBS)
    {
        IO::Log::Error(
            "Maximum number of vertex attributes exceeded (" +
            ToStr(format.GetAttributes().size()) + " but maximum is " +
            ToStr(GL_MAX_VERTEX_ATTRIBS) + ")"
        );
        return false;
    }

    /* Setup all attributes */
    GLuint index = 0;

    for (const auto& attrib : format.GetAttributes())
    {
        const auto& name = attrib.name;

        /* Check for valid names */
        if (name.size() >= 3 && name.substr(0, 3) == "gl_")
        {
            IO::Log::Error("Reserved name for vertex attribute used: \"" + name + "\"");
            return false;
        }

        /* Bind vertex attribute location */
        glBindAttribLocation(programID_, index++, name.c_str());
    }

    return true;
}

/* --- Constant buffers --- */

ConstantBuffer::DescriptionListType GLShaderComposition::QueryConstantBufferDescList() const
{
    ConstantBuffer::DescriptionListType descList;

    if (!HasCompiledSuccessful())
        return descList;

    /* Query number of uniform blocks */
    GLint numUniformBlocks = 0;
    glGetProgramiv(programID_, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

    if (!numUniformBlocks)
        return descList;

    /* Query maximal uniform block name length */
    GLint maxNameLength = 0;
    glGetProgramiv(programID_, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxNameLength);

    if (!maxNameLength)
        return descList;

    std::string blockName;
    blockName.resize(maxNameLength);

    /* Iterate over all uniform blocks */
    for (GLuint i = 0; i < static_cast<GLuint>(numUniformBlocks); ++i)
    {
        ConstantBuffer::Description desc;
        desc.index = i;

        /* Query uniform block name */
        GLsizei nameLength = 0;
        glGetActiveUniformBlockName(programID_, i, maxNameLength, &nameLength, &blockName[0]);
        desc.name = std::string(blockName.c_str());

        /* Query uniform block size */
        GLint blockSize = 0;
        glGetActiveUniformBlockiv(programID_, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        desc.size = blockSize;

        /* Insert uniform block into list */
        descList[desc.name] = desc;
    }

    return descList;
}

void GLShaderComposition::Attach(const ConstantBufferPtr& constBuffer, unsigned int slot)
{
    ShaderComposition::Attach(constBuffer, slot);

    /* Get GL constant buffer */
    auto constBufferGL = CAST_TO_GL_OBJECT(ConstantBuffer, constBuffer.get());

    /* Bind uniform block to program */
    const GLuint blockIndex = slot;
    const GLuint bindIndex = blockIndex;

    glUniformBlockBinding(programID_, blockIndex, bindIndex);

    constantBufferMetaData_.push_back({ constBufferGL->GetBufferID(), bindIndex });
}

void GLShaderComposition::Detach(const ConstantBufferPtr& constBuffer)
{
    ShaderComposition::Detach(constBuffer);
    //todo...
}

void GLShaderComposition::DetachAllConstantBuffers()
{
    ShaderComposition::DetachAllConstantBuffers();
    constantBufferMetaData_.clear();
}

/* --- Storage buffers --- */

StorageBuffer::DescriptionListType GLShaderComposition::QueryStorageBufferDescList() const
{
    StorageBuffer::DescriptionListType descList;

    if (!HasCompiledSuccessful())
        return descList;

    /* Query number of shader storage blocks */
    GLint numShaderStorageBlocks = 0;
    glGetProgramInterfaceiv(programID_, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numShaderStorageBlocks);

    if (!numShaderStorageBlocks)
        return descList;

    /* Query maximal shader storage block name length */
    GLint maxNameLength = 0;
    glGetProgramInterfaceiv(programID_, GL_SHADER_STORAGE_BLOCK, GL_MAX_NAME_LENGTH, &maxNameLength);

    if (!maxNameLength)
        return descList;

    std::string blockName;
    blockName.resize(maxNameLength);

    /* Iterate over all uniform blocks */
    for (GLuint i = 0; i < static_cast<GLuint>(numShaderStorageBlocks); ++i)
    {
        StorageBuffer::Description desc;
        desc.index = i;

        /* Query shader storage block name */
        GLsizei nameLength = 0;
        glGetProgramResourceName(programID_, GL_SHADER_STORAGE_BLOCK, i, maxNameLength, &nameLength, &blockName[0]);
        desc.name = std::string(blockName.c_str());

        /* Query shader storage  block size */
        GLint blockSize = 0;
        const GLenum property = GL_BUFFER_DATA_SIZE;

        glGetProgramResourceiv(
            programID_, GL_SHADER_STORAGE_BLOCK, i,
            1, &property,
            sizeof(blockSize)/sizeof(GLint), nullptr, &blockSize
        );

        desc.size = blockSize;

        /* Insert shader storage block into list */
        descList[desc.name] = desc;
    }

    return descList;
}

void GLShaderComposition::Attach(const StorageBufferPtr& storageBuffer, unsigned int slot)
{
    ShaderComposition::Attach(storageBuffer, slot);

    /* Get GL storage buffer */
    auto storageBufferGL = CAST_TO_GL_OBJECT(StorageBuffer, storageBuffer.get());

    /* Bind shader storage block to program */
    const GLuint blockIndex = slot;
    const GLuint bindIndex = blockIndex;

    glShaderStorageBlockBinding(programID_, blockIndex, bindIndex);

    storageBufferMetaData_.push_back({ storageBufferGL->GetBufferID(), bindIndex });
}

void GLShaderComposition::Detach(const StorageBufferPtr& storageBuffer)
{
    ShaderComposition::Detach(storageBuffer);
    //todo...
}

void GLShaderComposition::DetachAllStorageBuffers()
{
    ShaderComposition::DetachAllStorageBuffers();
    storageBufferMetaData_.clear();
}


/*
 * ======= Private: =======
 */

void GLShaderComposition::DetachAllShaders()
{
    DetachSingleShader( vertexShader_         );
    DetachSingleShader( pixelShader_          );
    DetachSingleShader( geometryShader_       );
    DetachSingleShader( tessControlShader_    );
    DetachSingleShader( tessEvaluationShader_ );
    DetachSingleShader( computeShader_        );
}

bool GLShaderComposition::QueryInfoLog(std::string& infoLog)
{
    /* Query compilation status */
    GLint linkStatus = 0;
    glGetProgramiv(programID_, GL_LINK_STATUS, &linkStatus);

    /* Query info log length */
    GLint infoLogLength = 0;
    glGetProgramiv(programID_, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0)
    {
        /* Query info log output */
        GLsizei charsWritten = 0;
        infoLog.resize(infoLogLength);

        glGetProgramInfoLog(programID_, infoLogLength, &charsWritten, &infoLog[0]);

        GLAdjustString(infoLog);
    }
    else
        infoLog.clear();

    return linkStatus == GL_TRUE;
}

template <class T> bool GLShaderComposition::CompileSingleShader(const T& shader)
{
    if (!shader || !shader->sourceCode)
        return false;

    /* Compile shader */
    std::string infoLog;
    bool success = shader->Compile(shader->sourceCode->sources, infoLog);
    
    /* Print info log */
    PrintInfoLog(infoLog, !success);

    return success;
}

template <class Base, class Derived> void GLShaderComposition::AttachSingleShader(
    std::shared_ptr<Derived>& dest, const std::shared_ptr<Base>& source, const std::string& baseTypeName)
{
    /* Convert shared pointer to a GL render system object */
    dest = std::dynamic_pointer_cast<Derived>(source);
    if (!dest)
        throw PointerConversionException(__FUNCTION__, baseTypeName, "GL" + baseTypeName);

    /* Attach shader to program */
    glAttachShader(programID_, dest->GetShaderID());
}

template <class T> void GLShaderComposition::DetachSingleShader(T& shader)
{
    if (shader)
    {
        glDetachShader(programID_, shader->GetShaderID());
        shader.reset();
    }
}


} // /namespace Video

} // /namespace Fork



// ========================