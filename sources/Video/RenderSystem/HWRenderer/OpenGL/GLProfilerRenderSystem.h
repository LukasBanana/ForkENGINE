/*
 * OpenGL profiler render system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_PROFILER_RENDER_SYSTEM_H__
#define __FORK_GL_PROFILER_RENDER_SYSTEM_H__


#include "GLRenderSystem.h"


namespace Fork
{

namespace Video
{


//! OpenGL profiler render system.
class GLProfilerRenderSystem : public GLRenderSystem
{
    
    public:
        
        GLProfilerRenderSystem() = default;
        
        /* === Interface implementation === */

        RenderContextPtr CreateRenderContext(
            const VideoMode& videoMode, const ContextDescription& contextDesc,
            const std::wstring& frameTitle, void* parentFrame = nullptr
        );

        /* --- Hardware buffers --- */

        void WriteBuffer(
            VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        );
        void WriteBuffer(
            IndexBuffer* indexBuffer, const IndexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        );
        void WriteBuffer(
            ConstantBuffer* constBuffer, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        );

        void WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset);
        void WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset);
        void WriteSubBuffer(ConstantBuffer* constBuffer, const void* data, size_t dataSize, size_t offset);

        /* --- Textures --- */

        void WriteTexture(
            Texture1D* texture,
            const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            Texture1D* texture,
            const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );

        void WriteTexture(
            Texture2D* texture,
            const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            Texture2D* texture,
            const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );

        void WriteTexture(
            Texture3D* texture,
            const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            Texture3D* texture,
            const Texture3D::PositionType& position, const Texture3D::SizeType& size,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );

        void WriteTexture(
            TextureCube* texture,
            const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            TextureCube* texture,
            const TextureCube::PositionType& position, const TextureCube::SizeType& size,
            const Math::AxisDirections faceDirection, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================