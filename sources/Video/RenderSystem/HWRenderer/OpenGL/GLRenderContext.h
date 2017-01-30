/*
 * OpenGL render context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_RENDER_CONTEXT_H__
#define __FORK_GL_RENDER_CONTEXT_H__


#include "Video/RenderSystem/RenderContext.h"
#include "Core/Version.h"
#include "GLImport.h"


namespace Fork
{

namespace Video
{


class GLRenderTarget;
class GLVertexBuffer;
class GLConstantBuffer;
class GLStateManager;

//! OpenGL render context implementation.
class GLRenderContext : public RenderContext
{
    
    public:
        
        /* === Functions === */

        GLRenderContext(
            GLRenderContext* sharedRenderContext,
            const VideoMode& videoMode, const ContextDescription& contextDesc,
            const std::wstring& frameTitle, void* parentFrame = nullptr
        );
        virtual ~GLRenderContext();

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

        void SetupPointSize(float size);
        void SetupLineSize(float size);

        void SetupViewport(const Viewport& viewport);
        //! \todo Not yet implemented.
        void SetupMultiViewport(const std::vector<Viewport>& viewports);

        void SetupScissor(const Scissor& scissor);
        void SetupMultiScissor(const std::vector<Scissor>& scissors);

        void SetupProjectionMatrix(const Math::Matrix4f& projectionMatrix);
        void SetupProjectionMatrix(const Projectionf& projection);
        void SetupViewMatrix(const Math::Matrix4f& viewMatrix);
        
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

    protected:
        
        friend class GLRenderSystem;

        void InitRenderStateConstBuffer(GLConstantBuffer* constBuffer);

    private:
        
        /* === Structures === */

        struct GLVersion
        {
            GLVersion() = default;
            GLVersion(int ma, int mi);

            int major = 0;
            int minor = 0;
        };

        struct GLRenderState
        {
            GLRenderTarget* renderTarget    = nullptr;          //!< Actively GL render target.
            GLVertexBuffer* vertexBuffer    = nullptr;          //!< Actively bound GL vertex buffer.
            GLenum          drawMode        = GL_TRIANGLES;
            GLenum          indexBufferType = GL_UNSIGNED_INT;
            bool            isFrontCCW      = false;
        };

        /* === Functions === */

        void CreateContext(GLRenderContext* sharedRenderContext);
        void DeleteContext();

        #if defined(FORK_WINDOWS_PLATFORM)

        void DeleteGLContext(HGLRC& renderContext);

        HGLRC CreateGLContext(bool useExtProfile, GLRenderContext* sharedRenderContextGL = nullptr);
        HGLRC CreateStdContextProfile();
        HGLRC CreateExtContextProfile(HGLRC sharedGLRC = nullptr);

        void SetupDeviceContextAndPixelFormat();

        void SelectPixelFormat();
        bool SetupAntiAliasing();
        void CopyPixelFormat(GLRenderContext* sourceContext);

        void ReCreateFrame();

        //! Calles "ReCreateFrame" and "SetupDeviceContextAndPixelFormat".
        void ReCreateFrameAndUpdatePixelFormat();

        #elif defined(FORK_POSIX_PLATFORM)

        GLXContext CreateContext(bool useExtProfile);

        #endif

        bool SetupVSyncInterval();

        void QueryStateManager(GLRenderContext* sharedRenderContext = nullptr);
        void SetupInitialConfig();
        void ClearMSPixelFormats();

        void QueryGLVersion();

        void InverseScreen(bool enable);
        void SetupFrontFace(bool isFrontCCW);

        void UpdateViewport();
        void UpdateScissor();

        //! Equivalent to 'GetVideoMode().resolution.height'.
        inline unsigned int ScreenHeight() const
        {
            return GetVideoMode().resolution.height;
        }

        /* === Members === */

        /* --- Main context data --- */

        #if defined(FORK_WINDOWS_PLATFORM)
        
        static const size_t numMaxMSPixelFormats = 8; //!RENAME to "maxNumMSPixelFormats"

        int     pixelFormat_        = 0;
        int     multiSamplePixelFormats_[numMaxMSPixelFormats];
        size_t  numMSPixelFormats_  = 0;

        HDC     deviceContext_      = 0;
        HGLRC   renderContext_      = 0;
        
        #elif defined(FORK_POSIX_PLATFORM)

        GLXContext renderContext_;

        #endif

        /* --- Active states --- */

        GLVersion                       versionGL_;

        GLConstantBuffer*               renderStateConstBuffer_ = nullptr; //!< Reference to the render state constant buffer.

        GLRenderState                   globalGLRenderState_;
        std::shared_ptr<GLStateManager> stateMngr_;

        //! Specifies whether this context uses a shared GL render context (true) or has its own hardware context (false).
        bool                            hasSharedContext_       = false;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================