/*
 * Direct3D 11 render system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_RENDER_SYSTEM_H__
#define __FORK_D3D11_RENDER_SYSTEM_H__


#include "Video/RenderSystem/RenderSystem.h"

#include <d3d11.h>
#include <vector>


namespace Fork
{

namespace Video
{


class D3D11Texture1D;
class D3D11Texture2D;
class D3D11Texture3D;
class D3D11TextureCube;
class D3D11RenderTarget;
class D3D11RenderContext;

//! Direct3D 11 render system implementation.
class D3D11RenderSystem : public RenderSystem
{
    
    public:
        
        D3D11RenderSystem();
        virtual ~D3D11RenderSystem();

        /* === Interface implementation === */

        RenderContextPtr CreateRenderContext(
            const VideoMode& videoMode, const ContextDescription& contextDesc,
            const std::wstring& frameTitle, void* parentFrame = nullptr
        );

        /* --- Render states --- */

        RasterizerStatePtr      CreateRasterizerState   (const RasterizerState  ::Description& desc);
        DepthStencilStatePtr    CreateDepthStencilState (const DepthStencilState::Description& desc);
        BlendStatePtr           CreateBlendState        (const BlendState       ::Description& desc);
        SamplerStatePtr         CreateSamplerState      (const SamplerState     ::Description& desc);

        void ChangeStateDesc(RasterizerState*   state, const RasterizerState::Description&      desc);
        void ChangeStateDesc(DepthStencilState* state, const DepthStencilState::Description&    desc);
        void ChangeStateDesc(BlendState*        state, const BlendState::Description&           desc);
        void ChangeStateDesc(SamplerState*      state, const SamplerState::Description&         desc);

        /* --- Hardware buffers --- */

        VertexBufferPtr CreateVertexBuffer();
        IndexBufferPtr CreateIndexBuffer();
        ConstantBufferPtr CreateConstantBuffer(const ConstantBuffer::Description& desc);
        StorageBufferPtr CreateStorageBuffer(const StorageBuffer::Description& desc);

        void WriteBuffer(
            VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        );
        void WriteBuffer(
            IndexBuffer* indexBuffer, const IndexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        );
        void WriteBuffer(
            ConstantBuffer* constantBuffer, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        );
        //! \todo Not yet implemented
        void WriteBuffer(
            StorageBuffer* storageBuffer, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        );

        void WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset = 0);
        void WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset = 0);
        void WriteSubBuffer(ConstantBuffer* constantBuffer, const void* data, size_t dataSize, size_t offset = 0);

        //! \todo Not yet implemented
        void* MapBuffer(StorageBuffer* storageBuffer, const HardwareBufferCPUAccess cpuAccess = HardwareBufferCPUAccess::ReadWrite);
        //! \todo Not yet implemented
        void UnmapBuffer(StorageBuffer* storageBuffer);

        /* --- Textures --- */

        Texture1DPtr CreateTexture1D();
        Texture2DPtr CreateTexture2D();
        Texture3DPtr CreateTexture3D();
        TextureCubePtr CreateTextureCube();

        void WriteTexture(
            Texture1D* texture,
            const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            Texture1D* texture,
            const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        );

        void WriteTexture(
            Texture2D* texture,
            const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            Texture2D* texture,
            const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        );

        void WriteTexture(
            Texture3D* texture,
            const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            Texture3D* texture,
            const Texture3D::PositionType& position, const Texture3D::SizeType& size,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        );

        void WriteTexture(
            TextureCube* texture,
            const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        );
        void WriteSubTexture(
            TextureCube* texture,
            const TextureCube::PositionType& position, const TextureCube::SizeType& size,
            const TextureCube::Faces faceDirection, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        );

        //! \todo Not yet implemented!
        bool ReadTexture(const Texture* texture, ImageUByte& image, unsigned int mipLevel = 0);
        //! \todo Not yet implemented!
        bool ReadTexture(const Texture* texture, ImageFloat& image, unsigned int mipLevel = 0);

        /* --- Render targets --- */

        RenderTargetPtr CreateRenderTarget();

        /* --- Shaders --- */

        VertexShaderPtr         CreateVertexShader          ();
        PixelShaderPtr          CreatePixelShader           ();
        GeometryShaderPtr       CreateGeometryShader        ();
        TessControlShaderPtr    CreateTessControlShader     ();
        TessEvaluationShaderPtr CreateTessEvaluationShader  ();
        ComputeShaderPtr        CreateComputeShader         ();
        ShaderCompositionPtr    CreateShaderComposition     ();

        /* --- Misc --- */

        QueryPtr CreateQuery(const Query::Types type);

    protected:
        
        friend class D3D11RenderContext;
        friend class D3D11RenderTarget;

        std::shared_ptr<D3D11RenderContext> InitRenderContext(
            const std::shared_ptr<D3D11RenderContext>& renderContext,
            const VideoMode& videoMode, const ContextDescription& contextDesc
        );

        //! Creates a new swap chain with the specified configuration.
        IDXGISwapChain* CreateSwapChain(
            const VideoMode& videoMode, const ContextDescription& contextDesc, HWND windowHandle
        );
        
        //! Queries the back buffer from the specified swap chain.
        ID3D11Texture2D* QueryBackBuffer(IDXGISwapChain* swapChain);
        
        //! Creates a new render target view for the specified resource.
        ID3D11RenderTargetView* CreateRenderTargetView(ID3D11Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr);

        //! Creates a new depth stencil with the specified configuration.
        ID3D11Texture2D* CreateDepthStencil(const Math::Size2ui& resolution, unsigned int numSamples = 1);

        //! Creates a new depth stencil view for the specified depth stencil.
        ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Texture2D* depthStencil, bool hasAntiAliasing);

    private:
        
        friend class D3D11ProfilerRenderSystem;

        /* === Structures === */

        struct VideoAdapter
        {
            IDXGIAdapter*   adapter = nullptr;
            Adapter         info;
        };

        /* === Functions === */

        std::unique_ptr<ShaderProvider> CreateShaderProvider();

        //! \throws std::runtime_error On errors.
        static ID3D11Device* CreateDevice(
            const D3D_FEATURE_LEVEL* featureLevel, D3D_FEATURE_LEVEL* featureLevelOut,
            IDXGIAdapter* adapter, ID3D11DeviceContext*& context
        );
        //! \throws std::runtime_error On errors.
        static IDXGIFactory* QueryFactory(ID3D11Device* device);

        static std::vector<VideoAdapter> QueryVideoAdapters(IDXGIFactory* factory, D3D_FEATURE_LEVEL featureLevel);

        /* === Members === */

        ID3D11Device*               device_         = nullptr;
        ID3D11DeviceContext*        context_        = nullptr;

        IDXGIFactory*               factory_        = nullptr;

        D3D_FEATURE_LEVEL           featureLevel_   = D3D_FEATURE_LEVEL_11_0;
        std::vector<VideoAdapter>   videoAdapters_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================