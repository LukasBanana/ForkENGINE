/*
 * OpenGL profiler render system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLProfilerRenderSystem.h"
#include "GLProfilerRenderContext.h"


namespace Fork
{

namespace Video
{


RenderContextPtr GLProfilerRenderSystem::CreateRenderContext(
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame)
{
    /* Create OpenGL render context */
    return InitRenderContext(
        std::make_shared<GLProfilerRenderContext>(
            profilerModel_,
            dynamic_cast<GLRenderContext*>(GetSharedRenderContext()),
            videoMode, contextDesc,
            frameTitle, parentFrame
        ),
        videoMode, contextDesc
    );
}

/* --- Hardware buffers --- */

void GLProfilerRenderSystem::WriteBuffer(
    VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data,
    size_t dataSize, const HardwareBufferUsage usage)
{
    profilerModel_.RecordBufferCreation();
    GLRenderSystem::WriteBuffer(vertexBuffer, format, data, dataSize, usage);
}

void GLProfilerRenderSystem::WriteBuffer(
    IndexBuffer* indexBuffer, const IndexFormat& format, const void* data,
    size_t dataSize, const HardwareBufferUsage usage)
{
    profilerModel_.RecordBufferCreation();
    GLRenderSystem::WriteBuffer(indexBuffer, format, data, dataSize, usage);
}

void GLProfilerRenderSystem::WriteBuffer(
    ConstantBuffer* constBuffer, const void* data,
    size_t dataSize, const HardwareBufferUsage usage)
{
    profilerModel_.RecordBufferCreation();
    GLRenderSystem::WriteBuffer(constBuffer, data, dataSize, usage);
}

void GLProfilerRenderSystem::WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset)
{
    profilerModel_.RecordBufferUpdate();
    profilerModel_.RecordBufferBinding();
    GLRenderSystem::WriteSubBuffer(vertexBuffer, data, dataSize, offset);
}

void GLProfilerRenderSystem::WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset)
{
    profilerModel_.RecordBufferUpdate();
    profilerModel_.RecordBufferBinding();
    GLRenderSystem::WriteSubBuffer(indexBuffer, data, dataSize, offset);
}

void GLProfilerRenderSystem::WriteSubBuffer(ConstantBuffer* constBuffer, const void* data, size_t dataSize, size_t offset)
{
    profilerModel_.RecordBufferUpdate();
    profilerModel_.RecordBufferBinding();
    GLRenderSystem::WriteSubBuffer(constBuffer, data, dataSize, offset);
}

/* --- Textures --- */

void GLProfilerRenderSystem::WriteTexture(
    Texture1D* texture,
    const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    GLRenderSystem::WriteTexture(texture, textureFormat, textureSize, arraySize, imageFormat, imageDataType, imageData);
}

void GLProfilerRenderSystem::WriteSubTexture(
    Texture1D* texture,
    const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    GLRenderSystem::WriteSubTexture(texture, position, size, arrayIndex, imageFormat, imageDataType, imageData);
}

void GLProfilerRenderSystem::WriteTexture(
    Texture2D* texture,
    const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    GLRenderSystem::WriteTexture(texture, textureFormat, textureSize, arraySize, imageFormat, imageDataType, imageData);
}

void GLProfilerRenderSystem::WriteSubTexture(
    Texture2D* texture,
    const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    GLRenderSystem::WriteSubTexture(texture, position, size, arrayIndex, imageFormat, imageDataType, imageData);
}

void GLProfilerRenderSystem::WriteTexture(
    Texture3D* texture,
    const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    GLRenderSystem::WriteTexture(texture, textureFormat, textureSize, imageFormat, imageDataType, imageData);
}

void GLProfilerRenderSystem::WriteSubTexture(
    Texture3D* texture,
    const Texture3D::PositionType& position, const Texture3D::SizeType& size,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    GLRenderSystem::WriteSubTexture(texture, position, size, imageFormat, imageDataType, imageData);
}

void GLProfilerRenderSystem::WriteTexture(
    TextureCube* texture,
    const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    GLRenderSystem::WriteTexture(texture, textureFormat, textureSize, arraySize, imageFormat, imageDataType, imageData);
}

void GLProfilerRenderSystem::WriteSubTexture(
    TextureCube* texture,
    const TextureCube::PositionType& position, const TextureCube::SizeType& size,
    const Math::AxisDirections faceDirection, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    GLRenderSystem::WriteSubTexture(texture, position, size, faceDirection, arrayIndex, imageFormat, imageDataType, imageData);
}


} // /namespace Video

} // /namespace Fork



// ========================