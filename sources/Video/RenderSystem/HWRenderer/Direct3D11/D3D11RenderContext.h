/*
 * Direct3D 11 render context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_RENDER_CONTEXT_H__
#define __FORK_D3D11_RENDER_CONTEXT_H__


#include "Video/RenderSystem/RenderContext.h"
#include "Core/Version.h"
#include "Texture/D3D11FrameBuffer.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11RenderTarget;
class D3D11RenderSystem;

//! Direct3D 11 render context implementation.
class D3D11RenderContext : public RenderContext
{
    
    public:
        
        D3D11RenderContext(
            D3D11RenderSystem* renderSystem, ID3D11DeviceContext* context,
            const VideoMode& videoMode, const ContextDescription& contextDesc,
            const std::wstring& frameTitle, void* parentFrame = nullptr
        );
        virtual ~D3D11RenderContext();

        /* === Interface implementation === */

        /* --- Common --- */

        Adapter QueryAdapter() const override;
        RendererDetails QueryRendererDetails() const override;
        RendererSupport QueryRendererSupport() const override;
        RenderStateReflection QueryRenderStateReflection() const override;

        ShaderCode::Languages ShadingLanguage() const override;
        std::string ShadingLanguageFileExt() const override;
        std::vector<Shader::Versions> ShaderModels() const override;

        bool Activate() override;
        bool Deactivate() override;

        void SetupClearColor(const ColorRGBAf& color) override;
        void SetupClearDepth(float depth) override;
        void SetupClearStencil(unsigned char stencil) override;

        void ClearBuffers(const ClearBuffersFlags::DataType flags = ClearBuffersFlags::FrameBuffer) override;

        void Present() override;

        bool ChangeVideoMode(const VideoMode& videoMode) override;
        bool ChangeDesc(const ContextDescription& contextDesc) override;

        /* --- Render states --- */

        void ChangeState(const RasterizerState*     state);
        void ChangeState(const DepthStencilState*   state, unsigned int stencilRef = 0);
        void ChangeState(const BlendState*          state);

        void SetupViewport(const Viewport& viewport);
        void SetupMultiViewport(const std::vector<Viewport>& viewports);

        void SetupScissor(const Scissor& scissor);
        void SetupMultiScissor(const std::vector<Scissor>& scissors);

        void SetupDrawMode(const GeometryPrimitives type);

        void BindSamplerState(const SamplerState* state, unsigned int layer = 0);
        void UnbindSamplerState(unsigned int layer = 0);

        /* --- Hardware buffers --- */

        void BindVertexBuffer(VertexBuffer* vertexBuffer);
        void UnbindVertexBuffer();

        void BindIndexBuffer(IndexBuffer* indexBuffer);
        void UnbindIndexBuffer();

        /* --- Textures --- */

        void BindTexture(const Texture* texture, unsigned int layer = 0);
        void UnbindTexture(const Texture* texture, unsigned int layer = 0);

        void CopyTextureImage(
            Texture2D* texture, const Math::Recti& sourceArea,
            const Math::Point2i& destOffset = {}, unsigned int mipLevel = 0
        );
        void CopyTextureImage(
            Texture2D* texture, const RenderTarget* renderTarget, const Math::Recti& sourceArea,
            const Math::Point2i& destOffset = {}, unsigned int mipLevel = 0
        );

        void GenerateMIPMaps(Texture* texture);
        //! \todo Incomplete
        void GenerateMIPMaps(
            Texture2D* destTexture, const Texture2D* sourceTexture, const ShaderComposition* shader,
            unsigned int layer = 0, const GenerateMIPMapsShaderCallback& shaderCallback = nullptr
        );

        /* --- Render targets --- */

        void BindRenderTarget(RenderTarget* renderTarget);

        /* --- Drawing --- */

        void Draw(unsigned int numVertices, unsigned int firstVertex = 0);

        void DrawIndexed(unsigned int numVertices);
        void DrawIndexed(unsigned int numVertices, int indexOffset);

        void DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances);
        void DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances, unsigned int instanceOffset);

        void DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances);
        void DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset);
        void DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset, unsigned int instanceOffset);
        
        /* --- Shaders --- */

        void BindShader(const ShaderComposition* shaderComposition);
        void UnbindShader();

        void Dispatch(const Math::Size3ui& numThreadGroups);

        /* --- Query --- */

        void BeginQuery(Query* query);
        void EndQuery(Query* query);

        unsigned int QueryResult(Query* query);

    private:
        
        /* === Functions === */

        void UpdateSyncInterval();

        void CreateContext(bool activate = true);
        void ReleaseContext();

        bool CreateSwapChain();
        bool CreateBackBufferAndRTV();
        bool CreateDepthStencilAndDSV(const Math::Size2ui& resolution);

        bool ReCreateDepthStencilAndDSV(const Math::Size2ui& resolution);
        bool ResizeBackBuffer(const Math::Size2ui& resolution);

        //! Binds the render target for the specified frame buffer.
        void BindFrameBuffer(const D3D11FrameBuffer* frameBuffer);
        //! Binds the render context's main frame buffer.
        void BindMainFrameBuffer();
        //! Binds the active frame buffer (used for context switchting).
        void BindFrameBuffer();
        //! Unbinds the active frame buffer.
        void UnbindFrameBuffer();

        //! Copies the specified 'source' resource into the 'desination' resource.
        void CopyResource(
            ID3D11Resource* destination, ID3D11Resource* source, const Math::Recti& sourceArea,
            const Math::Point2i& destOffset = {}, unsigned int mipLevel = 0
        );

        //! Returns true if the specified area fills the entire fullscreen.
        bool IsRectFullscreen(const Math::Recti& area) const;

        //! Returns the depth stencil view of the main frame buffer.
        inline ID3D11DepthStencilView*& MainDepthStencilView()
        {
            return mainFrameBuffer_.depthStencilView;
        }
        //! Returns the first render target view of the main frame buffer.
        inline ID3D11RenderTargetView*& MainRenderTargetView()
        {
            return mainFrameBuffer_.renderTargetViews.front();
        }

        /* === Members === */

        D3D11RenderSystem*      renderSystem_       = nullptr;

        ID3D11DeviceContext*    context_            = nullptr;
        IDXGISwapChain*         swapChain_          = nullptr;

        UINT                    syncInterval_       = 0;

        ID3D11Texture2D*        backBuffer_         = nullptr;
        ID3D11Texture2D*        depthStencil_       = nullptr;

        D3D11FrameBuffer        mainFrameBuffer_;
        const D3D11FrameBuffer* activeFrameBuffer_  = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================