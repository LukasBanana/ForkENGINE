/*
 * Shader composition file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Core/Exception/NullPointerException.h"
#include "Video/RenderSystem/HardwareBuffer/ConstantBufferNotFoundException.h"
#include "Video/RenderSystem/HardwareBuffer/StorageBufferNotFoundException.h"
#include "IO/Core/Log.h"
#include "../RenderSysCtx.h"

#include <algorithm>


namespace Fork
{

namespace Video
{


ShaderComposition::ShaderComposition()
{
}
ShaderComposition::~ShaderComposition()
{
}

bool ShaderComposition::Compile(const IO::CommandLine& commandLine)
{
    Shader::CompilationFlags::DataType flags = 0;

    auto flagsMap = Shader::CompilationFlagsMaps();

    /* Parse command line arguments */
    for (const auto& arg : commandLine.GetArguments())
    {
        auto it = flagsMap.find(arg);
        if (it != flagsMap.end())
            flags |= it->second;
    }

    return Compile(flags);
}

/* --- Constant buffers --- */

ConstantBuffer::DescriptionPtr ShaderComposition::FindConstantBufferDesc(const std::string& name) const
{
    auto descList = QueryConstantBufferDescList();

    auto it = descList.find(name);
    if (it != descList.end())
        return std::make_shared<ConstantBuffer::Description>(it->second);

    return nullptr;
}

void ShaderComposition::Attach(const ConstantBufferPtr& constBuffer, unsigned int slot)
{
    ASSERT_POINTER(constBuffer);
    constantBuffers.push_back(constBuffer);
}

void ShaderComposition::Attach(const ConstantBufferPtr& constBuffer, const std::string& name)
{
    auto constBufferDesc = FindConstantBufferDesc(name);
    if (constBufferDesc)
        Attach(constBuffer, constBufferDesc->index);
}

void ShaderComposition::Attach(const ConstantBufferPtr& constBuffer)
{
    ASSERT_POINTER(constBuffer);
    Attach(constBuffer, constBuffer->GetDesc().index);
}

void ShaderComposition::Detach(const ConstantBufferPtr& constBuffer)
{
    ASSERT_POINTER(constBuffer);
    RemoveFromList(constantBuffers, constBuffer);
}

void ShaderComposition::DetachAllConstantBuffers()
{
    constantBuffers.clear();
}

ConstantBufferPtr ShaderComposition::CreateAndAttachConstantBuffer(
    const std::string& name, const HardwareBufferUsage usage, bool isOptional)
{
    return CreateAndAttachConstantBuffer(name, "", usage, isOptional);
}

ConstantBufferPtr ShaderComposition::CreateAndAttachConstantBuffer(
    const std::string& name, const std::string& shaderName, const HardwareBufferUsage usage, bool isOptional)
{
    /* Try to find the specified constant buffer by its name */
    auto bufferDesc = FindConstantBufferDesc(name);

    if (!bufferDesc)
    {
        if (isOptional)
            return nullptr;

        /* Error: constant buffer not found */
        std::string err = "Constant buffer \"" + name + "\" not found";

        if (!shaderName.empty())
            err += " (" + shaderName + ")";

        throw ConstantBufferNotFoundException(err);
    }

    /* Create constant buffer */
    auto constBuffer = RenderSys()->CreateConstantBuffer(*bufferDesc);

    /* Allocate the storage for this constant buffer and bind it to this shader composition */
    RenderSys()->WriteBuffer(constBuffer.get(), nullptr, bufferDesc->size, usage);
    Attach(constBuffer);

    return constBuffer;
}

/* --- Storage buffers --- */

StorageBuffer::DescriptionPtr ShaderComposition::FindStorageBufferDesc(const std::string& name) const
{
    auto descList = QueryStorageBufferDescList();

    auto it = descList.find(name);
    if (it != descList.end())
        return std::make_shared<StorageBuffer::Description>(it->second);

    return nullptr;
}

void ShaderComposition::Attach(const StorageBufferPtr& storageBuffer, unsigned int slot)
{
    ASSERT_POINTER(storageBuffer);
    storageBuffers.push_back(storageBuffer);
}

void ShaderComposition::Attach(const StorageBufferPtr& storageBuffer, const std::string& name)
{
    auto constBufferDesc = FindStorageBufferDesc(name);
    if (constBufferDesc)
        Attach(storageBuffer, constBufferDesc->index);
}

void ShaderComposition::Attach(const StorageBufferPtr& storageBuffer)
{
    ASSERT_POINTER(storageBuffer);
    Attach(storageBuffer, storageBuffer->GetDesc().index);
}

void ShaderComposition::Detach(const StorageBufferPtr& storageBuffer)
{
    ASSERT_POINTER(storageBuffer);
    RemoveFromList(storageBuffers, storageBuffer);
}

void ShaderComposition::DetachAllStorageBuffers()
{
    storageBuffers.clear();
}

StorageBufferPtr ShaderComposition::CreateAndAttachStorageBuffer(
    const std::string& name, const HardwareBufferUsage usage, bool isOptional)
{
    return CreateAndAttachStorageBuffer(name, "", usage, isOptional);
}

StorageBufferPtr ShaderComposition::CreateAndAttachStorageBuffer(
    const std::string& name, const std::string& shaderName, const HardwareBufferUsage usage, bool isOptional)
{
    /* Try to find the specified storage buffer by its name */
    auto bufferDesc = FindStorageBufferDesc(name);

    if (!bufferDesc)
    {
        if (isOptional)
            return nullptr;

        /* Error: constant buffer not found */
        std::string err = "Storage buffer \"" + name + "\" not found";

        if (!shaderName.empty())
            err += " (" + shaderName + ")";

        throw StorageBufferNotFoundException(err);
    }

    /* Create constant buffer */
    auto storageBuffer = RenderSys()->CreateStorageBuffer(*bufferDesc);

    /* Allocate the storage for this storage buffer and bind it to this shader composition */
    RenderSys()->WriteBuffer(storageBuffer.get(), nullptr, bufferDesc->size, usage);
    Attach(storageBuffer);

    return storageBuffer;
}

/* --- Event handler handling --- */

void ShaderComposition::PostUpdateConstantBuffer(RenderContext* renderContext) const
{
    if (eventHandler_ && renderContext)
        eventHandler_->OnUpdateConstantBuffers(this, renderContext);
}

void ShaderComposition::PostDisassemblyOutput(const Shader* shader, const std::string& disassemblyOutput) const
{
    if (eventHandler_ && shader)
        eventHandler_->OnDisassemblyOutput(this, shader, disassemblyOutput);
}


/*
 * ======= Protected: =======
 */

void ShaderComposition::PrintInfoLog(const std::string& infoLog, bool isError)
{
    if (!infoLog.empty())
    {
        using namespace Platform::ConsoleManip;

        if (isError)
            IO::Log::Message(infoLog, Colors::Red | Colors::Intens);
        #ifdef FORK_DEBUG
        else
            IO::Log::Message(infoLog, Colors::Yellow | Colors::Intens);
        #endif
    }
}


} // /namespace Video

} // /namespace Fork



// ========================