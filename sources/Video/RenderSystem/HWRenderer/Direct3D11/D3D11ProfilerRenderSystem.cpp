/*
 * Direct3D 11 profiler render system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ProfilerRenderSystem.h"
#include "D3D11ProfilerRenderContext.h"


namespace Fork
{

namespace Video
{


RenderContextPtr D3D11ProfilerRenderSystem::CreateRenderContext(
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame)
{
    /* Create Direct3D 11 render context */
    return InitRenderContext(
        std::make_shared<D3D11ProfilerRenderContext>(
            profilerModel_,
            this, context_,
            videoMode, contextDesc,
            frameTitle, parentFrame
        ),
        videoMode, contextDesc
    );
}

/* --- Hardware buffers --- */

void D3D11ProfilerRenderSystem::WriteBuffer(
    VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data,
    size_t dataSize, const HardwareBufferUsage usage)
{
    profilerModel_.RecordBufferCreation();
    D3D11RenderSystem::WriteBuffer(vertexBuffer, format, data, dataSize, usage);
}

void D3D11ProfilerRenderSystem::WriteBuffer(
    IndexBuffer* indexBuffer, const IndexFormat& format, const void* data,
    size_t dataSize, const HardwareBufferUsage usage)
{
    profilerModel_.RecordBufferCreation();
    D3D11RenderSystem::WriteBuffer(indexBuffer, format, data, dataSize, usage);
}

void D3D11ProfilerRenderSystem::WriteBuffer(
    ConstantBuffer* constBuffer, const void* data,
    size_t dataSize, const HardwareBufferUsage usage)
{
    profilerModel_.RecordBufferCreation();
    D3D11RenderSystem::WriteBuffer(constBuffer, data, dataSize, usage);
}

void D3D11ProfilerRenderSystem::WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset)
{
    profilerModel_.RecordBufferUpdate();
    profilerModel_.RecordBufferBinding();
    D3D11RenderSystem::WriteSubBuffer(vertexBuffer, data, dataSize, offset);
}

void D3D11ProfilerRenderSystem::WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset)
{
    profilerModel_.RecordBufferUpdate();
    profilerModel_.RecordBufferBinding();
    D3D11RenderSystem::WriteSubBuffer(indexBuffer, data, dataSize, offset);
}

void D3D11ProfilerRenderSystem::WriteSubBuffer(ConstantBuffer* constBuffer, const void* data, size_t dataSize, size_t offset)
{
    profilerModel_.RecordBufferUpdate();
    profilerModel_.RecordBufferBinding();
    D3D11RenderSystem::WriteSubBuffer(constBuffer, data, dataSize, offset);
}

/* --- Textures --- */

void D3D11ProfilerRenderSystem::WriteTexture(
    Texture1D* texture,
    const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    D3D11RenderSystem::WriteTexture(texture, textureFormat, textureSize, arraySize, imageFormat, imageDataType, imageData);
}

void D3D11ProfilerRenderSystem::WriteSubTexture(
    Texture1D* texture,
    const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    D3D11RenderSystem::WriteSubTexture(texture, position, size, arrayIndex, imageFormat, imageDataType, imageData);
}

void D3D11ProfilerRenderSystem::WriteTexture(
    Texture2D* texture,
    const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    D3D11RenderSystem::WriteTexture(texture, textureFormat, textureSize, arraySize, imageFormat, imageDataType, imageData);
}

void D3D11ProfilerRenderSystem::WriteSubTexture(
    Texture2D* texture,
    const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    D3D11RenderSystem::WriteSubTexture(texture, position, size, arrayIndex, imageFormat, imageDataType, imageData);
}

void D3D11ProfilerRenderSystem::WriteTexture(
    Texture3D* texture,
    const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    D3D11RenderSystem::WriteTexture(texture, textureFormat, textureSize, imageFormat, imageDataType, imageData);
}

void D3D11ProfilerRenderSystem::WriteSubTexture(
    Texture3D* texture,
    const Texture3D::PositionType& position, const Texture3D::SizeType& size,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    D3D11RenderSystem::WriteSubTexture(texture, position, size, imageFormat, imageDataType, imageData);
}

void D3D11ProfilerRenderSystem::WriteTexture(
    TextureCube* texture,
    const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureCreation();
    D3D11RenderSystem::WriteTexture(texture, textureFormat, textureSize, arraySize, imageFormat, imageDataType, imageData);
}

void D3D11ProfilerRenderSystem::WriteSubTexture(
    TextureCube* texture,
    const TextureCube::PositionType& position, const TextureCube::SizeType& size,
    const Math::AxisDirections faceDirection, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    profilerModel_.RecordTextureUpdate();
    D3D11RenderSystem::WriteSubTexture(texture, position, size, faceDirection, arrayIndex, imageFormat, imageDataType, imageData);
}


} // /namespace Video

} // /namespace Fork



// ========================