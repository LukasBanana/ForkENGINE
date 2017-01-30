/*
 * OpenGL render system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_RENDER_SYSTEM_H__
#define __FORK_GL_RENDER_SYSTEM_H__


#include "Video/RenderSystem/RenderSystem.h"
#include "GLExtensionLoader.h"

#include "GLImport.h"


namespace Fork
{

namespace Video
{


class GLTexture1D;
class GLTexture2D;
class GLTexture3D;
class GLTextureCube;
class GLRenderTarget;
class GLRenderContext;
class GLConstantBuffer;

//! OpenGL render system implementation.
class GLRenderSystem : public RenderSystem
{
    
    public:
        
        GLRenderSystem();
        virtual ~GLRenderSystem();

        /* === Interface implementation === */

        RenderContextPtr CreateRenderContext(
            const VideoMode& videoMode, const ContextDescription& contextDesc,
            const std::wstring& frameTitle, void* parentFrame = nullptr
        );

        //RenderContext* GetSharedRenderContext();

        void EnableDebugDump(bool enableDebugDump) override;

        const Math::Matrix4f& ProjectionMatrix(const Projectionf& projection) const override;

        /* --- Render states --- */

        RasterizerStatePtr      CreateRasterizerState   (const RasterizerState  ::Description& desc) override;
        DepthStencilStatePtr    CreateDepthStencilState (const DepthStencilState::Description& desc) override;
        BlendStatePtr           CreateBlendState        (const BlendState       ::Description& desc) override;
        SamplerStatePtr         CreateSamplerState      (const SamplerState     ::Description& desc) override;

        void ChangeStateDesc(RasterizerState*   state, const RasterizerState::Description&      desc) override;
        void ChangeStateDesc(DepthStencilState* state, const DepthStencilState::Description&    desc) override;
        void ChangeStateDesc(BlendState*        state, const BlendState::Description&           desc) override;
        void ChangeStateDesc(SamplerState*      state, const SamplerState::Description&         desc) override;

        /* --- Hardware buffers --- */

        VertexBufferPtr CreateVertexBuffer() override;
        IndexBufferPtr CreateIndexBuffer() override;
        ConstantBufferPtr CreateConstantBuffer(const ConstantBuffer::Description& desc) override;
        StorageBufferPtr CreateStorageBuffer(const StorageBuffer::Description& desc) override;

        void WriteBuffer(
            VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) override;
        void WriteBuffer(
            IndexBuffer* indexBuffer, const IndexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) override;
        void WriteBuffer(
            ConstantBuffer* constantBuffer, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) override;
        void WriteBuffer(
            StorageBuffer* storageBuffer, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) override;

        void WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset = 0) override;
        void WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset = 0) override;
        void WriteSubBuffer(ConstantBuffer* constantBuffer, const void* data, size_t dataSize, size_t offset = 0) override;

        void* MapBuffer(
            StorageBuffer* storageBuffer,
            const HardwareBufferCPUAccess cpuAccess = HardwareBufferCPUAccess::ReadWrite
        ) override;
        void UnmapBuffer(StorageBuffer* storageBuffer) override;

        /* --- Textures --- */

        Texture1DPtr CreateTexture1D() override;
        Texture2DPtr CreateTexture2D() override;
        Texture3DPtr CreateTexture3D() override;
        TextureCubePtr CreateTextureCube() override;

        void WriteTexture(
            Texture1D* texture,
            const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) override;
        void WriteSubTexture(
            Texture1D* texture,
            const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) override;

        void WriteTexture(
            Texture2D* texture,
            const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) override;
        void WriteSubTexture(
            Texture2D* texture,
            const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) override;

        void WriteTexture(
            Texture3D* texture,
            const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) override;
        void WriteSubTexture(
            Texture3D* texture,
            const Texture3D::PositionType& position, const Texture3D::SizeType& size,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) override;

        void WriteTexture(
            TextureCube* texture,
            const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) override;
        void WriteSubTexture(
            TextureCube* texture,
            const TextureCube::PositionType& position, const TextureCube::SizeType& size,
            const TextureCube::Faces faceDirection, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) override;

        bool ReadTexture(const Texture* texture, ImageUByte& image, unsigned int mipLevel = 0) override;
        bool ReadTexture(const Texture* texture, ImageFloat& image, unsigned int mipLevel = 0) override;

        /* --- Render targets --- */

        RenderTargetPtr CreateRenderTarget() override;

        /* --- Shaders --- */

        VertexShaderPtr         CreateVertexShader          () override;
        PixelShaderPtr          CreatePixelShader           () override;
        GeometryShaderPtr       CreateGeometryShader        () override;
        TessControlShaderPtr    CreateTessControlShader     () override;
        TessEvaluationShaderPtr CreateTessEvaluationShader  () override;
        ComputeShaderPtr        CreateComputeShader         () override;
        ShaderCompositionPtr    CreateShaderComposition     () override;

        ConstantBufferPtr GetRenderStateConstBuffer() const override;

        /* --- Misc --- */

        QueryPtr CreateQuery(const Query::Types type) override;

    protected:

        std::shared_ptr<GLRenderContext> InitRenderContext(
            const std::shared_ptr<GLRenderContext>& renderContext,
            const VideoMode& videoMode, const ContextDescription& contextDesc
        );

    private:
        
        /* === Functions === */

        void LoadGLExtensions(const RendererProfileDescription& rendererProfileDesc);
        void CreateRenderStateConstBuffer();

        std::unique_ptr<ShaderProvider> CreateShaderProvider();

        /* === Members === */

        GLExtensionLoader::ExtMapType       extMap_;

        std::shared_ptr<GLConstantBuffer>   renderStateConstBuffer_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================