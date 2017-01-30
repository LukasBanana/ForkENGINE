/*
 * Render context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_CONTEXT_H__
#define __FORK_RENDER_CONTEXT_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Platform/Core/Frame.h"
#include "Math/Core/Rect.h"
#include "Math/Core/Size3.h"

#include "Video/RenderSystem/RenderSystemFlags.h"
#include "Video/RenderSystem/Shader/Shader.h"
#include "Video/RenderSystem/Shader/ShaderCode.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "Video/RenderSystem/Query.h"
#include "Video/RenderSystem/RenderState/GeometryPrimitives.h"
#include "Video/RenderSystem/RenderState/RenderAttributes.h"

#include "Video/Monitor/Adapter.h"
#include "Video/Monitor/RendererSupport.h"
#include "Video/Monitor/RenderStateReflection.h"
#include "Video/Monitor/ContextDescription.h"
#include "Video/Monitor/RendererDetails.h"

#include "Video/Core/VideoMode.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/Core/Viewport.h"
#include "Video/Core/Scissor.h"
#include "Video/Core/Projection.h"

#include "Plugins/cpplibext/flexible_stack.hpp"

#include <memory>
#include <functional>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(RenderContext);

class RasterizerState;
class DepthStencilState;
class BlendState;
class SamplerState;

class VertexBuffer;
class IndexBuffer;

class ShaderComposition;

/**
Frame recreation callback for the render context.
The most callbacks are implemented via class inheritance,
but this is just a small exception.
\see RenderContext::SetFrameReCreationCallback
*/
typedef std::function<void (void* userData, void* parentWindow)> FrameReCreateCallback;


/**
Render context interface. A render context should be used inside the main loop as follows:
\code
while (IsGameRunning)
{
    renderContext->ClearBuffers();

    // Render scene here ...

    renderContext->Present();
}
\endcode
If you want to use several render contexts, do it as follows:
\code
while (IsGameRunning)
{
    // Begin with 1st render context
    renderContextA->ClearBuffers();

        // Render 1st scene here ...

    renderContextA->Present();

    // Begin with 2nd render context
    renderContextB->ClearBuffers();

        // Render 2nd scene here ...

    renderContextB->Present();
}
\endcode
*/
class FORK_EXPORT RenderContext
{
    
    public:
        
        virtual ~RenderContext();

        /**
        Shader callback for the custom MIP-map generation function.
        \param[in] mipSize Specifies the current MIP map size.
        \param[in] mipLevel Specifies the current MIP level which is about to be rendered.
        \see GenerateMIPMaps
        */
        typedef std::function<void (const Texture2D::SizeType& mipSize, unsigned int mipLevel)> GenerateMIPMapsShaderCallback;

        /* === Structures === */

        //! Global render state, represented with multiple members.
        struct RenderState
        {
            /**
            Current projection matrix (in projection space).
            \see SetupProjectionMatrix
            */
            Math::Matrix4f              projectionMatrix;
            /**
            Current view matrix (in view space).
            \see SetupViewMatrix
            */
            Math::Matrix4f              viewMatrix;
            /**
            Current camera matrix (in world space).
            \see SetupViewMatrix
            */
            Math::Matrix4f              cameraMatrix;
            /**
            Current world matrix (in world space).
            \see SetupWorldMatrix
            */
            Math::Matrix4f              worldMatrix;

            /**
            Current rasterizer state.
            \see ChangeState(const RasterizerState*)
            */
            const RasterizerState*      rasterizerState     = nullptr;
            /**
            Current depth-stencil state.
            \see ChangeState(const DepthStencilState*)
            */
            const DepthStencilState*    depthStencilState   = nullptr;
            /**
            Current blend state.
            \see ChangeState(const BlendState*)
            */
            const BlendState*           blendState          = nullptr;

            /**
            Actively bound shader composition.
            \see BindShaderComposition
            */
            const ShaderComposition*    shaderComposition   = nullptr;
            /**
            Actively bound render target.
            \see BindRenderTarget
            */
            RenderTarget*               renderTarget        = nullptr;

            //! Current point rasterization size. By default 1.0.
            float                       pointSize           = 1.0f;
            //! Current line rasterization size. By default 1.0.
            float                       lineSize            = 1.0f;

            //! Active drawing primitive. By default GeometryPrimitives::Points.
            GeometryPrimitives          drawPrimitive       = GeometryPrimitives::Points;

            /**
            Current near plane (extracted from the projection matrix).
            \see Projection::ExtractClippingPlanesLH
            \see Projection::ExtractClippingPlanesRH
            */
            float                       nearPlane           = 1.0f;
            /**
            Current far plane (extracted from the projection matrix).
            \see Projection::ExtractClippingPlanesLH
            \see Projection::ExtractClippingPlanesRH
            */
            float                       farPlane            = 100.0f;

            /**
            Specifies whether the screen is inversed or not.
            Only true when a render target is bound in the OpenGL renderer.
            \remarks This value can not be changed manual and is mainly used internally.
            \see GetInvScreenConstantBuffer
            */
            bool                        isInvScreen         = false;

            /**
            Specifies the current viewport configuration.
            \remarks This will only be updated by the 'RenderContext::SetupViewport' function,
            but not by the 'RenderContext::SetupMultiViewport' function.
            \see RenderContext::SetupViewport
            */
            Viewport                    viewport;
            /**
            Specifies the current scissor configuration.
            \remarks This will only be updated by the 'RenderContext::SetupScissor' function,
            but not by the 'RenderContext::SetupMultiScissor' function.
            \see RenderContext::SetupScissor
            */
            Scissor                     scissor;
        };

        /* === Static functions === */

        /**
        Returns the active render context.
        \see Activate
        */
        static RenderContext* Active();

        //! Sets the frame recreation callback.
        static void SetFrameReCreationCallback(const FrameReCreateCallback& callback, void* userData);

        /* === Interface === */

        /* --- Common --- */

        /**
        Queries the renderer information (hardware and software description strings).
        \note Make sure this is the currently active render context (see RenderContext::Active).
        \see RenderContext::Active
        \see Adapter
        */
        virtual Adapter QueryAdapter() const = 0;
        /**
        Queries the low-level renderer details.
        Among others this may depend on internal states (when a render target is bound for instance).
        \see RendererDetails
        */
        virtual RendererDetails QueryRendererDetails() const = 0;
        /**
        Queries the supported features of this renderer.
        \note Make sure this is the currently active render context (see RenderContext::Active).
        \see RenderContext::Active
        \see RendererSupport
        */
        virtual RendererSupport QueryRendererSupport() const = 0;
        /**
        Queries the render state map. This can be used to debug the current render pipeline state.
        \note This should only be used for debugging purposes, because it may stall the GPU pipeline.
        \see RenderStateReflection
        */
        virtual RenderStateReflection QueryRenderStateReflection() const = 0;

        /**
        Returns the supported shading language.
        \see ShaderCode::Languages
        */
        virtual ShaderCode::Languages ShadingLanguage() const = 0;
        /**
        Returns the file extension of the supported shading language,
        e.g. "glsl" for the 'OpenGL Shading Language' or "hlsl" the 'DirectX High Level Shading Language'.
        \see ShaderCode::Languages
        */
        virtual std::string ShadingLanguageFileExt() const = 0;
        /**
        Returns a list of all supported shader models.
        \see Shader::Versions
        */
        virtual std::vector<Shader::Versions> ShaderModels() const = 0;
        /**
        Returns the highest supported shader model or 'Shader::Versions::__Unspecified__' is no shader model is supported.
        \see ShaderModels
        */
        Shader::Versions HighestShaderModel() const;

        //! Activates the render context. After this call, all rendering operations will be done with this context.
        virtual bool Activate() = 0;
        //! Deactivates the render context. Never do any rendering operations while no render context is activated!
        virtual bool Deactivate() = 0;

        //! Returns true if this is the currently activated render context.
        bool IsActivated() const;

        /**
        Returns true if this is the shared render context.
        This is always the first created context.
        \see RenderSystem::GetSharedRenderContext
        */
        bool IsSharedContext() const;

        /**
        Sets the value for clearing the color buffer. By default { 0, 0, 0, 0 }.
        \see ClearBuffers
        */
        virtual void SetupClearColor(const ColorRGBAf& color);
        /**
        Sets the value for clearing the depth buffer. By default 1.0.
        \see ClearBuffers
        */
        virtual void SetupClearDepth(float depth);
        /**
        Sets the value for clearing the stencil buffer. By default 0.
        \see ClearBuffers
        */
        virtual void SetupClearStencil(unsigned char stencil);

        /**
        Clears the specified buffers.
        \see ClearBuffersFlags
        */
        virtual void ClearBuffers(const ClearBuffersFlags::DataType flags = ClearBuffersFlags::FrameBuffer) = 0;

        /**
        Presents the final image onto the context frame.
        If v-sync (vertical synchronisation) is enabled, the program waits at this point,
        until the graphics card is synchronised with the monitor.
        */
        virtual void Present() = 0;

        /**
        Changes the video mode.
        \return True if the video mode could be changed.
        Otherwise the desktop video mode could not be changed or a parent window is used.
        \note The settings 'borderless' and 'resizable' of the frame descriptions will be changed here as well!
        \remarks If you only want to resize the resolution, use 'ResizeResolution'.
        \see VideoMode
        \see ResizeResolution
        */
        virtual bool ChangeVideoMode(const VideoMode& videoMode) = 0;
        /**
        Changes the context description.
        \return True if the context could be changed. Otherwise an error occured and will be printed to the log output.
        \see ContextDescription
        \todo Not implemented yet!
        */
        virtual bool ChangeDesc(const ContextDescription& contextDesc) = 0;

        /**
        Resizes the screen resolution. This is equivalent to the following code:
        \code
        auto videoMode = GetVideoMode();
        videoMode.resolution = resolution;
        ChangeVideoMode(videoMode);
        \endcode
        \see ChangeVideoMode
        */
        bool ResizeResolution(const Math::Size2ui& resolution);
        /**
        Enables or disables fullscreen mode. This is equivalent to the following code:
        \code
        auto videoMode = GetVideoMode();
        videoMode.isFullscreen = enable;
        ChangeVideoMode(videoMode);
        \endcode
        \see ChangeVideoMode
        */
        bool EnableFullscreen(bool enable);
        /**
        Enables or disables vertical synchronisation (VSync). This is equivalent to the following code:
        \code
        auto contextDesc = GetDesc();
        contextDesc.vsyncDesc.isEnabled = enable;
        ChangeDesc(contextDesc);
        \endcode
        \see ChangeDesc
        */
        bool EnableVSync(bool enable);

        /**
        Returns the video mode resolution.
        \remarks This is equivalent to the following code:
        \code
        GetVideoMode().resolution
        \endcode
        \see GetVideoMode
        */
        inline const Math::Size2ui& Resolution() const
        {
            return GetVideoMode().resolution;
        }

        /* --- Render states --- */

        /**
        Pushes the specified render state onto a stack, so it can be restored later.
        \remarks This can be used when render states are changed temporarily and must be restored after a render operation.
        \code
        // Usage example:
        context->PushRenderAttribute(Video::RenderAttributes::Viewport); // push viewport render state attribute
        context->SetupViewport({ { 0, 0 }, { 512, 512 } }); // set temporary viewport
        // rendering here ...
        context->PopRenderAttribute(); // restore previous viewport render state
        \endcode
        \throws InvalidArgumentException If 'state' is invalid.
        \see PopRenderAttribute
        \see RenderAttributes
        */
        void PushRenderAttribute(const RenderAttributes state);
        /**
        Pushes the specified render states onto a stack, so it can be restored later.
        \see PushRenderAttribute(const RenderAttributes)
        */
        void PushRenderAttribute(const std::initializer_list<RenderAttributes>& stateList);
        /**
        Pops the render state on the top of the internal stack.
        \see PushRenderAttribute
        */
        void PopRenderAttribute();
        /**
        Pops the first 'count' render states.
        \see PushRenderAttribute
        */
        void PopRenderAttribute(size_t count);

        /**
        Sets the rasterizer state.
        \see CreateRasterizerState
        \see GetDefaultRasterizerState
        */
        virtual void ChangeState(const RasterizerState* state) = 0;
        /**
        Sets the depth-stencil state.
        \param[in] stencilRef Specifies the stencil test reference bit-mask. By default 0x00000000.
        \see CreateDepthStencilState
        \see GetDefaultDepthStencilState
        */
        virtual void ChangeState(const DepthStencilState* state, unsigned int stencilRef = 0) = 0;
        /**
        Sets the blend state.
        \see CreateBlendState
        \see GetDefaultBlendState
        */
        virtual void ChangeState(const BlendState* state) = 0;

        /**
        Sets the point rasterization size.
        \param[in] size Specifies the new point rasterization size. By default 1.0.
        \note This is only available for the OpenGL render system.
        */
        virtual void SetupPointSize(float size);
        /**
        Sets the line rasterization size.
        \param[in] size Specifies the new line rasterization size. By default 1.0.
        \note This is only available for the OpenGL render system.
        */
        virtual void SetupLineSize(float size);

        /**
        Sets the current viewport to the default size (the context's frame's size).
        This is equivalent to the following code:
        \code
        renderContext->SetupViewport({ {}, renderContext->GetVideoMode().resolution.Cast<int>() });
        \endcode
        \see void SetupViewport(const Viewport&)
        */
        virtual void SetupViewport();
        /**
        Sets the current viewport.
        \param[in] viewport Specifies the viewport with its position, size and min- and max depth values.
        \remarks To set the viewport to the context's frame's size use the other "SetupViewport" function.
        \see SetupViewport()
        */
        virtual void SetupViewport(const Viewport& viewport);
        //! \see SetupViewport
        virtual void SetupMultiViewport(const std::vector<Viewport>& viewports) = 0;

        /**
        Sets the current scissor rectangle. This can be used to crop the output image.
        \remarks Scissoring can be enabled with the rasterizer state.
        \see RasterizerState::Description::isScissorTestEnabled
        */
        virtual void SetupScissor(const Scissor& scissor);
        //! \see SetupScissor
        virtual void SetupMultiScissor(const std::vector<Scissor>& scissors) = 0;

        /**
        Sets the projection matrix for the current global render state.
        \note Some render systems use a left-handed- and some others use a right-handed coordinate system.
        To let the render system choose the correct projection matrix, use "SetupProjectionMatrix(const Projection<float>&)".
        \see SetupProjectionMatrix(const Projection<float>&)
        */
        virtual void SetupProjectionMatrix(const Math::Matrix4f& projectionMatrix);
        /**
        Sets the projection matrix for the current global render state.
        \param[in] projection Specifies the projection for the projection matrix.
        Since some render systems need a LH (left-handed) matrix and some others need a RH (right-handed)
        projection matrix, this is done via the "Projection" class and not with a direct 4x4 matrix.
        By default the left handed matrix is used (Projection::GetMatrixLH)
        \see Projection
        \see Projection::GetMatrixLH
        \see Projection::GetMatrixRH
        \see RenderState::projectionMatrix
        \see GetRenderState
        */
        virtual void SetupProjectionMatrix(const Projectionf& projection);
        /**
        Sets the view matrix for the current global render state.
        \param[in] viewMatrix Specifies the new view matrix. This matrix must already be the camera's inverse object matrix.
        \note Some render systems (e.g. for OpenGL) will change the input matrix to support a renderer independent coordinate space.
        This engine uses a left-handed coordinate system, but OpenGL uses a right-handed coordinate system.
        So the engine will balance this between the render systems.
        \see RenderState::viewMatrix
        \see RenderState::cameraMatrix
        \see GetRenderState
        */
        virtual void SetupViewMatrix(const Math::Matrix4f& viewMatrix);
        /**
        Sets the world matrix for the current global render state.
        \param[in] worldMatrix Specifies the world matrix (also called "object matrix").
        \see RenderState::worldMatrix
        \see GetRenderState
        */
        virtual void SetupWorldMatrix(const Math::Matrix4f& worldMatrix);

        /**
        Returns the current world-view-projection matrix. This is equivalent to the following code:
        \code
        const auto& rs = GetRenderState();
        wvpMatrix = rs.projectionMatrix * rs.viewMatrix * rs.worldMatrix;
        \endcode
        The common naming convention is "world-view-projection", although in the above example the order is inverted.
        This is due to the engine's matrix representation. Direct3D for instance uses row major matrices,
        but OpenGL (and this engine) uses column major matrices (which is typically in mathematics).
        So (among others) the order of matrix multiplications must be in the order as shown in the above example.
        */
        virtual Math::Matrix4f WVPMatrix() const;
        /**
        Returns the current worl-view matrix.
        \see WVPMatrix
        */
        virtual Math::Matrix4f WVMatrix() const;
        /**
        Returns the current view-projection matrix.
        \see WVPMatrix
        */
        virtual Math::Matrix4f VPMatrix() const;

        /**
        Set the geometry primitive draw mode. By default GeometryPrimitives::Triangles
        \see GeometryPrimitives
        */
        virtual void SetupDrawMode(const GeometryPrimitives type) = 0;

        //! Binds the specified sampler state to the specified layer.
        virtual void BindSamplerState(const SamplerState* state, unsigned int layer = 0) = 0;
        //! Unbinds the specified sampler state to the specified layer.
        virtual void UnbindSamplerState(unsigned int layer = 0) = 0;

        /* --- Hardware buffers --- */

        //! Binds the specified vertex buffer.
        virtual void BindVertexBuffer(VertexBuffer* vertexBuffer) = 0;
        //! Unbinds the specified index buffer.
        virtual void UnbindVertexBuffer() = 0;

        //! Binds the specified index buffer.
        virtual void BindIndexBuffer(IndexBuffer* indexBuffer) = 0;
        //! Unbinds the specified index buffer.
        virtual void UnbindIndexBuffer() = 0;

        /* --- Textures --- */

        //! Binds the specified texture to the specified layer.
        virtual void BindTexture(const Texture* texture, unsigned int layer = 0) = 0;
        //! Unbinds the specified texture from the specified layer.
        virtual void UnbindTexture(const Texture* texture, unsigned int layer = 0) = 0;

        /**
        Copies a rectangular portion from the main frame buffer into the specified texture image.
        \see CopyTextureImage(Texture2D*, const RenderTarget*, const Math::Recti&, const Math::Point2i&, unsigned int)
        \note Since some renderers store image data beginning from lower-left and some others beginning from the upper-left,
        this function may adjust this flaw. This may take a lot of time because it's done on the CPU.
        To avoid this performance hit, use the other "CopyTextureImage" function to copy the image data from a render target.
        \note This should not be called while a render target is bound!
        \todo Flip image data on Y-axis.
        */
        virtual void CopyTextureImage(
            Texture2D* texture, const Math::Recti& sourceArea,
            const Math::Point2i& destOffset = {}, unsigned int mipLevel = 0
        ) = 0;
        /**
        Copies a rectangular portion from the specified render target into the specified texture image.
        \param[in,out] texture Raw pointer to the 2D destination texture whose image is to be modified.
        \param[in] renderTarget Raw pointer to the render target from which the image is to be copied.
        \param[in] sourceArea Specifies the area of the source frame buffer (in screen space).
        \param[in] destOffset Specifies an optional destination offset (in image space). By default { 0, 0 }.
        \param[in] mipLevel Specifies an optional level for the destination MIP-map.
        The source render target will always be read from the highest MIP-level.
        By default 0 which is the highest MIP level.
        \throws PointerConversionException If 'texture' is null or not a renderer specific valid 2D texture or
        'renderTarget' is null or not a renderer specific render target.
        \note This should not be called while a render target is bound!
        \see BindRenderTarget
        */
        virtual void CopyTextureImage(
            Texture2D* texture, const RenderTarget* renderTarget, const Math::Recti& sourceArea,
            const Math::Point2i& destOffset = {}, unsigned int mipLevel = 0
        ) = 0;

        /**
        Generates the MIP-map levels for the specified texture.
        \param[in,out] texture Raw pointer to the texture whose MIP-maps are to be generated.
        \remarks 'MIP' comes from the Latin "multum in parvo", meaning "a multitude in a small space".
        \throws NullPointerException If 'texture' is null.
        \throws PointerConversionException If 'texture' is not created with the renderer specific context.
        */
        virtual void GenerateMIPMaps(Texture* texture) = 0;
        /**
        Generates the MIP-map levels for the specified texture in a custom process.
        This can be used to generate MIP-maps with a different filter than the default one.
        \param[out] destTexture Raw pointer to the 2D texture whose MIP-maps are to be generated.
        \param[in] sourceTexture Raw pointer to the 2D texture which will be read to generate the destination texture.
        \param[in] shader Constant raw pointer to the shader composition which will be used to render each MIP-map level.
        This shader gets a single input texture with the base MIP-map level which is currently used in the generation process,
        i.e. no MIP-map selection is required. Commonly the shader samples four surrounding pixels and merges them to a single output pixel.
        Only three vertices will be rendered, therefore the vertex shader should produce a fullscreen triangle:
        \code
        // Example in GLSL:
        gl_Position = vec4(
            (gl_VertexID == 2 ? 3.0 : -1.0),
            (gl_VertexID == 0 ? 3.0 : -1.0),
            -1.0, // Right-handed coordiante systems have +Z with negative values
            1.0
        );
        \endcode
        \code
        // Example in HLSL:
        vertexOutput.position = float4(
            (vertexInput.vertexID == 2 ? 3.0 : -1.0),
            (vertexInput.vertexID == 0 ? 3.0 : -1.0),
            1.0, // Left-handed coordiante systems have +Z with positive values
            1.0
        );
        \endcode
        \param[in] layer Specifies an optional layer for array textures.
        If the specified texture is not an array texture or 'layer' is greater than or equal to
        the texture array size (see Texture2D::GetArraySize), then this parameter will be ignored.
        \remarks 'MIP' comes from the Latin "multum in parvo", meaning "a multitude in a small space".
        \throws NullPointerException If 'texture' or 'shader' is null.
        \throws PointerConversionException If 'texture' or 'shader' is not created with the renderer specific context.
        \see Texture2D::GetArraySize
        */
        virtual void GenerateMIPMaps(
            Texture2D* destTexture, const Texture2D* sourceTexture, const ShaderComposition* shader,
            unsigned int layer = 0, const GenerateMIPMapsShaderCallback& shaderCallback = nullptr
        ) = 0;
        /**
        Generates the MIP-map levels for the specified texture in a custom process.
        The specified texture will be used for reading and writing.
        \see GenerateMIPMaps(Texture2D*, const Texture2D*, const ShaderComposition*, unsigned int, const GenerateMIPMapsShaderCallback&)
        */
        inline void GenerateMIPMaps(
            Texture2D* texture, const ShaderComposition* shader,
            unsigned int layer = 0, const GenerateMIPMapsShaderCallback& shaderCallback = nullptr)
        {
            GenerateMIPMaps(texture, texture, shader, layer, shaderCallback);
        }

        /**
        Takes a screenshot from the current framebuffer and writes it to the specified file.
        \return True if the screenshot could be created successful.
        \remarks This is a comfort function which can be implemented manually by creating a temporary
        texture, copying the framebuffer to it (see "RenderContext::CopyTextureImage"),
        read the image data back from the GPU to the CPU (see "RenderSystem::ReadTexture")
        and write it to file (see "ImageFileHandler::WriteImage").
        \note This may be a slow, depending on the render system if the image buffer must be converted on the CPU.
        Thus this should only be used for debugging purposes. To support screenshots in a final application,
        render the final pass into an additional render target and write its image data to file.
        \see RenderContext::CopyTextureImage
        \see RenderSystem::ReadTexture
        \see ImageFileHandler::WriteImage
        */
        bool SaveScreenshot(const std::string& filename);

        /* --- Render targets --- */

        /**
        Binds the specified render target.
        \param[in] renderTarget Raw-pointer to the render target which is to be bound.
        To bind the main render target, use 'RenderContext::GetMainRenderTarget()'.
        \see RenderContext::GetMainRenderTarget
        */
        virtual void BindRenderTarget(RenderTarget* renderTarget) = 0;

        /* --- Drawing --- */

        /**
        Performs a draw call. This requires that a vertex buffer was previously bound.
        \param[in] numVertices Specifies the number of vertices which are to be drawn.
        The prviously bound vertex buffer must provide at least this number of vertices!
        \param[in] firstVertex Specifies the index of the first vertex. By default 0.
        */
        virtual void Draw(unsigned int numVertices, unsigned int firstVertex = 0) = 0;

        //! \see DrawIndexed(unsigned int, unsigned int, int)
        virtual void DrawIndexed(unsigned int numVertices) = 0;
        /**
        Performs an indexed draw call. This requires that a vertex- and index buffer was previously bound.
        \param[in] numVertices Specifies the number of vertices which are to be drawn.
        This may be more vertices, than the previously bound vertex buffer provides.
        But the indices, stored in the previously bound index buffer, must not be greater
        than or equal to the number of vertices stored in the that vertex buffer!
        \param[in] indexOffset Specifies the offset which will be added to each index read from the index buffer.
        */
        virtual void DrawIndexed(unsigned int numVertices, int indexOffset) = 0;

        //! \see DrawInstanced(unsigned int, unsigned int, unsigned int, unsigned int)
        virtual void DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances) = 0;
        /**
        Performs an instanced draw call. This requires that a vertex buffer was previously bound.
        \param[in] numVertices Specifies the number of vertices which are to be drawn.
        The prviously bound vertex buffer must provide at least this number of vertices!
        \param[in] firstVertex Specifies the index of the first vertex. By default 0.
        \param[in] numInstances Specifies the number of instances which are to be drawn.
        \param[in] instanceOffset Specifies the offset which will be added to each instance index.
        This index can be used in the vertex shader.
        */
        virtual void DrawInstanced(
            unsigned int numVertices, unsigned int firstVertex,
            unsigned int numInstances, unsigned int instanceOffset
        ) = 0;

        //! \see DrawInstancedIndexed(unsigned int, unsigned int, int, unsigned int)
        virtual void DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances) = 0;
        //! \see DrawInstancedIndexed(unsigned int, unsigned int, int, unsigned int)
        virtual void DrawInstancedIndexed(
            unsigned int numVertices, unsigned int numInstances, int indexOffset
        ) = 0;
        /**
        Performs an indexed-instanced draw call. This requires that a vertex- and index buffer was previously bound.
        \param[in] numVertices Specifies the number of vertices which are to be drawn.
        This may be more vertices, than the previously bound vertex buffer provides.
        But the indices, stored in the previously bound index buffer, must not be greater
        than or equal to the number of vertices stored in the that vertex buffer!
        \param[in] numInstances Specifies the number of instances which are to be drawn.
        \param[in] indexOffset Specifies the offset which will be added to each index read from the index buffer.
        \param[in] instanceOffset Specifies the offset which will be added to each instance index.
        This index can be used in the vertex shader.
        */
        virtual void DrawInstancedIndexed(
            unsigned int numVertices, unsigned int numInstances,
            int indexOffset, unsigned int instanceOffset
        ) = 0;

        //virtual void DrawIndirect(const IndirectDrawBuffer* drawBuffer) = 0;
        //virtual void DrawIndirectIndexed(const IndirectDrawBuffer* drawBuffer) = 0;

        /* --- Shaders --- */

        virtual void BindShader(const ShaderComposition* shaderComposition) = 0;
        virtual void UnbindShader() = 0;

        /**
        Executes the previously bound compute shader with the specifed number of thread groups.
        The size of the local threads must be specified inside the compute shader.
        */
        virtual void Dispatch(const Math::Size3ui& numThreadGroups) = 0;
        //virtual void DispatchIndirect(const IndirectDispatchBuffer* dispatchBuffer) = 0;

        /* --- Query --- */

        /**
        Begins with the renderer query (e.g. query how many fragment samples pass the depth test).
        \see RenderSystem::CreateQuery
        \throws PointerConversionException (Debug Mode Only) If query is not an object created by this render system.
        \throws InvalidStateException If 'query->GetStateFalgs() != Query::StatesFlags::ReadToBegin'.
        */
        virtual void BeginQuery(Query* query) = 0;
        /**
        Ends the renderer query.
        \throws PointerConversionException (Debug Mode Only) If query is not an object created by this render system.
        \throws InvalidStateException If 'query->GetStateFalgs() != Query::StatesFlags::WaitingForEnd'.
        */
        virtual void EndQuery(Query* query) = 0;

        /**
        Returns the result of the specified renderer query.
        \param[in] query Raw-pointer to the renderer query. Previously 'Begin(query)' and a subsequent 'End(query)' must be called.
        \return If the query is from the type 'Query::Types::SamplesPassed' this will be the number of passed samples.
        If the query is from the type 'Query::Types::AnySamplesPassed' this will be 1 (for 'true') or 0 (for 'false').
        \throws PointerConversionException (Debug Mode Only) If query is not an object created by this render system.
        \throws InvalidStateException If 'query->GetStateFalgs() != Query::StatesFlags::ResultAvailable'.
        \remarks First call 'Begin(query)', then 'End(query)' and finally 'Reuslt(query)':
        \code
        auto query = renderSystem->CreateQuery(Video::Query::Types::SamplesPassed);
        //...
        renderContext->BeginQuery(query.get());
        {
            // render scene ...
        }
        renderContext->EndQuery(query.get());
        //...
        auto numSamplesPassed = renderContext->QueryResult(query.get());
        \endcode
        \see Begin(Query*)
        \see End(Query*)
        \see Query
        */
        virtual unsigned int QueryResult(Query* query) = 0;

        /* === Inline functions === */

        /**
        Returns the video mode (resolution, color bit-depth and fullscreen mode).
        \see Video::VideoMode
        */
        inline const VideoMode& GetVideoMode() const
        {
            return videoMode_;
        }

        /**
        Returns the context description (v-sync-, anti-aliasing-, frame- and renderer-profile settings).
        \see Video::ContextDescription
        */
        inline const ContextDescription& GetDesc() const
        {
            return contextDesc_;
        }

        /**
        Returns the current value to clear the color buffer.
        \see SetupClearColor
        */
        inline const ColorRGBAf& GetClearColor() const
        {
            return clearColor_;
        }
        /**
        Returns the current value to clear the depth buffer.
        \see SetupClearDepth
        */
        inline float GetClearDepth() const
        {
            return clearDepth_;
        }
        /**
        Returns the current value to clear the stencil buffer.
        \ee SetupClearStencil
        */
        inline unsigned char GetClearStencil() const
        {
            return clearStencil_;
        }

        //! Returns a raw-pointer to the frame (or rather window).
        inline Platform::Frame* GetFrame() const
        {
            return frame_.get();
        }

        /**
        Returns a raw-pointer to the main render target.
        \remarks Use this to unbind the previous render target:
        \code
        auto customRenderTarget = renderSystem->CreateRenderTarget();
        //...
        renderSystem->BindRenderTarget(customRenderTarget.get());
        {
            // Draw scene into render target ...
        }
        renderSystem->BindRenderTarget(renderContext->GetMainRenderTarget());
        \endcode
        \note This may also be a null pointer, when the render system don't need a saparate render target object.
        \see RenderSystem::CreateRenderTarget
        */
        inline RenderTarget* GetMainRenderTarget() const
        {
            return mainRenderTarget_.get();
        }

        /**
        Returns the current query state flags.
        \see StateFlags
        */
        inline Query::StateFlags::DataType GetQueryStateFlags() const
        {
            return queryStateFlags_;
        }

        /**
        Returns the current global render state.
        \see RenderState
        */
        inline const RenderState& GetRenderState() const
        {
            return globalRenderState_;
        }

    protected:
        
        /* === Functions === */

        RenderContext(const VideoMode& videoMode, const ContextDescription& contextDesc);

        //! Creates a new frame with the current context description settings.
        void CreateFrame(const std::wstring& title, void* parentFrame);

        //! Returns the frame position which is suitable for the specified video mode.
        Math::Point2i GetFramePositionForVideoMode(const Video::VideoMode& videoMode) const;

        static void SetActiveRenderContext(RenderContext* context);

        void CallFrameReCreateCallback();

        //! \note This pointer must never be null!
        void ChangeTextureMIPState(Texture* texture, bool hasMIPMaps);

        /* === Members === */

        VideoMode                   videoMode_;
        ContextDescription          contextDesc_;

        Platform::FramePtr          frame_;

        ColorRGBAf                  clearColor_;
        float                       clearDepth_         = 0.0f;
        unsigned char               clearStencil_       = 0;

        RenderTargetPtr             mainRenderTarget_;

        Query::StateFlags::DataType queryStateFlags_    = Query::StateFlags::ReadyToBegin;

        RenderState                 globalRenderState_;

    private:
        
        RenderContext(const RenderContext&) = delete;

        ext::flexible_stack<RenderAttributes> renderAttribStack_;

        static RenderContext* activeRenderContext_;

        static FrameReCreateCallback frameReCreateCallback_;
        static void* frameReCreateUserData_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================