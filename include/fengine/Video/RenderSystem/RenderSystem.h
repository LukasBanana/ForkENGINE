/*
 * Render system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_SYSTEM_H__
#define __FORK_RENDER_SYSTEM_H__


#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/HardwareBuffer/VertexBuffer.h"
#include "Video/RenderSystem/HardwareBuffer/IndexBuffer.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/RenderState/GeometryPrimitives.h"
#include "Video/RenderSystem/RendererProfilerModel.h"
#include "Video/RenderSystem/Query.h"

#include "Video/RenderSystem/Texture/Texture1D.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/Texture3D.h"
#include "Video/RenderSystem/Texture/TextureCube.h"
#include "Video/RenderSystem/Texture/TextureManager.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "Video/RenderSystem/Shader/ShaderManager.h"

#include "Video/RenderSystem/RenderState/BlendState.h"
#include "Video/RenderSystem/RenderState/RasterizerState.h"
#include "Video/RenderSystem/RenderState/DepthStencilState.h"
#include "Video/RenderSystem/RenderState/SamplerState.h"
#include "Video/RenderSystem/ScopedActiveRenderContext.h"

#include "Video/BufferFormat/HardwareBufferUsage.h"
#include "Video/Image/Image.h"
#include "Video/Image/ImageAttributes.h"
#include "Video/Core/Viewport.h"
#include "Video/Core/Scissor.h"
#include "Video/Core/Projection.h"

#include "Math/Core/Matrix4.h"
#include "Math/Common/AxisDirections.h"

#include "Platform/Core/DynamicLibrary.h"
#include "Core/Export.h"
#include "Core/Exception/InvalidArgumentException.h"

#include <memory>
#include <vector>
#include <type_traits>


namespace Fork
{

namespace Video
{


class ShaderProvider;

DECL_SHR_PTR(RenderSystem);

//! Render system interface.
class FORK_EXPORT RenderSystem
{
    
    public:
        
        /* === Static functions === */

        /**
        Creates a render system from the specified library.
        \param[in] library Specifies the library from which the render system is to be created (e.g. "ForkRendererGL" or "ForkRendererD3D11").
        \param[in] createProfilerSystem Specifies whether a profiler- or a standard render system is to be created. By default false.
        \throws NullPointerException If 'library' is null.
        \throws RenderSystemException If an error occured during creation.
        \see Platform::DynamicLibrary
        */
        static RenderSystemPtr Create(const Platform::DynamicLibraryPtr& library, bool createProfilerSystem = false);

        //! Returns the active render system.
        static RenderSystem* Active();
        /**
        Returns the active render system.
        \param[in] isRequired Specifies whether the render system is required or not.
        \return Raw-pointer to the active render system or null if no render system has been created yet.
        \throws RenderSystemException If 'isRequired' is true but the return value is null.
        */
        static RenderSystem* Active(bool isRequired);

        /* === Interface === */

        virtual ~RenderSystem();

        /**
        Creats a new render context.
        \param[in] videoMode Specifies the initial video mode (resolution, color bit-depth and fullscreen mode).
        \param[in] contextDesc Specifies the context description (v-sync-, anti-aliasing-, frame- and renderer-profile settings).
        \param[in] frameTitle Specifies the frame (or rather window) title.
        \param[in] parentFrame Optional raw pointer to an OS dependent parent frame. Under MS/Windows this must be a 'HWND*'.
        \code
        // Win32 example:
        HWND parentFrame = CreateWindow(...);
        renderSystem->CreateRenderContext(..., (void*)&parentFrame);
        \endcode
        \throws FrameCreationException If the context frame creation failed.
        \throws RenderContextException If the hardware render context creation failed.
        \see VideoMode
        \see ContextDescription
        \see RenderContext
        */
        virtual RenderContextPtr CreateRenderContext(
            const VideoMode& videoMode, const ContextDescription& contextDesc,
            const std::wstring& frameTitle, void* parentFrame = nullptr
        ) = 0;

        //! Releases the specified render context and returns true if the render context has been found and released.
        virtual bool ReleaseRenderContext(const RenderContextPtr& renderContext);

        /**
        Temporarily activates the shared render context for the current scope.
        \remarks This is used to temporarily activate the render context which shares all resources (particular for OpenGL).
        \see GetSharedRenderContext
        \see ScopedActiveRenderContext
        */
        ScopedActiveRenderContext ActivateSharedContext();

        /**
        Resets the profiler model data.
        \see GetProfilerModel::Reset
        */
        void ResetProfiler();

        /**
        Enables or disables the debug dump.
        If enabled, all renderer and driver implementation dependent debug output will be printed to the log output.
        \remarks The initial state for this option can be set with the 'RendererProfileDescription::enableDebugDump' render context flag.
        \see RendererProfileDescription::enableDebugDump
        */
        virtual void EnableDebugDump(bool enableDebugDump);

        /**
        Returns the matrix for the specified projection.
        Depending on the render system, this will be a left-handed (Direct3D) or right-handed (OpenGL) coordinate system).
        By default the left-handed coordinate system is used.
        */
        virtual const Math::Matrix4f& ProjectionMatrix(const Projectionf& projection) const;

        /* --- Render states --- */

        /**
        Creates a new rasterizer state.
        \see ChangeState(const RasterizerState*)
        \see GetDefaultRasterizerState
        */
        virtual RasterizerStatePtr CreateRasterizerState(const RasterizerState::Description& desc) = 0;
        /**
        Creates a new depth-stencil state.
        \see ChangeState(const DepthStencilState*)
        \see GetDefaultDepthStencilState
        */
        virtual DepthStencilStatePtr CreateDepthStencilState(const DepthStencilState::Description& desc) = 0;
        /**
        Creates a new blend state.
        \see ChangeState(const BlendState*)
        \see GetDefaultBlendState
        */
        virtual BlendStatePtr CreateBlendState(const BlendState::Description& desc) = 0;
        /**
        Creates a new sampler state.
        \remarks This is a render state which must be bound and unbound
        to be contrary to the rasterizer-, depth-stencil- and blend states.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see RenderContext::BindSamplerState(const SamplerState*)
        \see RenderContext::UnbindSamplerState(const SamplerState*)
        */
        virtual SamplerStatePtr CreateSamplerState(const SamplerState::Description& desc) = 0;

        //! Changes the description of the specified rasterizer state.
        virtual void ChangeStateDesc(RasterizerState* state, const RasterizerState::Description& desc) = 0;
        //! Changes the description of the specified depth-stencil state.
        virtual void ChangeStateDesc(DepthStencilState* state, const DepthStencilState::Description& desc) = 0;
        //! Changes the description of the specified blend state.
        virtual void ChangeStateDesc(BlendState* state, const BlendState::Description& desc) = 0;
        //! Changes the description of the specified sampler state.
        virtual void ChangeStateDesc(SamplerState* state, const SamplerState::Description& desc) = 0;

        /* --- Hardware buffers --- */

        /**
        Creates a new vertex buffer.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see VertexBuffer
        */
        virtual VertexBufferPtr CreateVertexBuffer() = 0;
        /**
        Creates a new index buffer.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see IndexBuffer
        */
        virtual IndexBufferPtr CreateIndexBuffer() = 0;
        /**
        Creates a new constant buffer (also called "Uniform Buffer").
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \throws ConstantBufferException If an error occured during constant buffer creation.
        \see ActivateSharedContext
        \see ConstantBuffer
        */
        virtual ConstantBufferPtr CreateConstantBuffer(const ConstantBuffer::Description& desc) = 0;
        /**
        Creates a new storage buffer (also called "Shader Resource").
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \throws StorageBufferException If an error occured during constant buffer creation.
        \see ActivateSharedContext
        \see StorageBuffer
        */
        virtual StorageBufferPtr CreateStorageBuffer(const StorageBuffer::Description& desc) = 0;

        /**
        Writes the specified data into the specified hardware vertex buffer.
        \return True on success, otherwise an error message is printed into the log output.
        \throws PointerConversionException If a vertex buffer, created from another render system, is passed.
        */
        virtual void WriteBuffer(
            VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) = 0;
        //! \see WriteBuffer(VertexBuffer*, const VertexFormat&, const void*, size_t, const HardwareBufferUsage)
        virtual void WriteBuffer(
            IndexBuffer* indexBuffer, const IndexFormat& format, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) = 0;
        //! Writes the specified data into the specified hardware constant buffer.
        virtual void WriteBuffer(
            ConstantBuffer* constantBuffer, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) = 0;
        //! Writes the specified data into the specified hardware storage buffer.
        virtual void WriteBuffer(
            StorageBuffer* storageBuffer, const void* data,
            size_t dataSize, const HardwareBufferUsage usage = HardwareBufferUsage::Static
        ) = 0;

        /**
        Writes the specified data partially into the specified hardware vertex buffer.
        \return True on success, otherwise an error message is printed into the log output.
        \throws PointerConversionException If a vertex buffer, created from another render system, is passed.
        */
        virtual void WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset = 0) = 0;
        //! \see WriteSubBuffer(VertexBuffer*, const void*, size_t, size_t)
        virtual void WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset = 0) = 0;
        //! Writes the specified data partially into the specified hardware constant buffer.
        virtual void WriteSubBuffer(ConstantBuffer* constantBuffer, const void* data, size_t dataSize, size_t offset = 0) = 0;

        /**
        Maps the specified storage buffer to CPU accessible memory.
        \param[in,out] storageBuffer Specifies the storage buffer whoes memory is to be read and/or written.
        \param[in] cpuAccess Specifies the CPU access type. By default HardwareBufferCPUAccess::ReadWrite
        \see UnmapBuffer(StorageBuffer*)
        \see HardwareBufferCPUAccess
        */
        virtual void* MapBuffer(StorageBuffer* storageBuffer, const HardwareBufferCPUAccess cpuAccess = HardwareBufferCPUAccess::ReadWrite) = 0;
        //! Unmaps the previously mapped storage buffer.
        virtual void UnmapBuffer(StorageBuffer* storageBuffer) = 0;

        /* --- Textures --- */

        /**
        Creates a 1-dimensional texture.
        \remarks To use the texture in a shader, it must be initializes with 'WriteTexture'.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see Texture1D
        */
        virtual Texture1DPtr CreateTexture1D() = 0;
        /**
        Creates a 2-dimensional texture.
        \remarks To use the texture in a shader, it must be initializes with 'WriteTexture'.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see Texture2D
        */
        virtual Texture2DPtr CreateTexture2D() = 0;
        /**
        Creates a 3-dimensional texture.
        \remarks To use the texture in a shader, it must be initializes with 'WriteTexture'.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see Texture3D
        */
        virtual Texture3DPtr CreateTexture3D() = 0;
        /**
        Creates a cube texture.
        \remarks To use the texture in a shader, it must be initialized with 'WriteTexture'.
        \remarks A cube texture has 6 cube map faces: +X, -X, +Y, -Y, +Z and -Z.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see TextureCube
        */
        virtual TextureCubePtr CreateTextureCube() = 0;

        /**
        Creates a 1-dimensional texture and initializes with the specified settings.
        \see CreateTexture1D()
        \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int)
        */
        virtual Texture1DPtr CreateTexture1D(
            const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize = 0
        );
        /**
        Creates a 2-dimensional texture and initializes with the specified settings.
        \see CreateTexture2D()
        \see WriteTexture(Texture2D*, const TextureFormats, const Texture2D::SizeType&, unsigned int)
        */
        virtual Texture2DPtr CreateTexture2D(
            const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize = 0
        );
        /**
        Creates a 3-dimensional texture and initializes with the specified settings.
        \see CreateTexture3D()
        \see WriteTexture(Texture3D*, const TextureFormats, const Texture3D::SizeType&)
        */
        virtual Texture3DPtr CreateTexture3D(
            const TextureFormats textureFormat, const Texture3D::SizeType& textureSize
        );
        /**
        Creates a cube texture and initializes with the specified settings.
        \see CreateTextureCube()
        \see WriteTexture(TextureCube*, const TextureFormats, const TextureCube::SizeType&, unsigned int)
        */
        virtual TextureCubePtr CreateTextureCube(
            const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize = 0
        );

        /**
        Write the data (with empty image) of the specified 1D texture.
        \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        */
        virtual void WriteTexture(
            Texture1D* texture,
            const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize = 0
        );
        //! Creates the texture data from the specified image.
        virtual void WriteTexture(Texture1D* texture, const ImageUByte& image);
        /**
        Writes the data of the specified 1D texture.
        \param[in] texture Raw-pointer to the texture which is to be written.
        \param[in] textureFormat Specifies the internal hardware texture format.
        \param[in] textureSize Specifies the 1-dimensional size.
        \param[in] arraySize Specifies the texture array size. An array size of 0 or 1 will be ignored.
        \param[in] imageFormat Specifies the image color format.
        \param[in] imageDataType Specifies the image data type.
        \param[in] imageData Raw-pointer to the image data which is to be uploaded to the GPU.
        */
        virtual void WriteTexture(
            Texture1D* texture,
            const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) = 0;
        /**
        Writes the sub data of the specified 1D texture.
        \param[in] texture Raw-pointer to the texture which is to be written.
        \param[in] position Specifies the position where the sub data is to be written.
        \param[in] size Specifies the size of the sub data.
        \param[in] arrayIndex Specifies the array index where the data is to be written.
        \param[in] imageFormat Specifies the image color format.
        \param[in] imageDataType Specifies the image data type.
        \param[in] imageData Raw-pointer to the image data which is to be uploaded to the GPU.
        \param[in] mipLevel Optional MIP level. By default 0 (which is the largest MIP level).
        \note This requires that the texture buffer was already created using the respective "WriteTexture" function.
        \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        */
        virtual void WriteSubTexture(
            Texture1D* texture,
            const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) = 0;

        //! \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int)
        virtual void WriteTexture(
            Texture2D* texture,
            const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize = 0
        );
        //! Creates the texture data from the specified image.
        virtual void WriteTexture(Texture2D* texture, const ImageUByte& image);
        /**
        \note For OpenGL texture arrays no integer texture formats are supported! They will be converted automatically into floating-pointer texture formats.
        \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        */
        virtual void WriteTexture(
            Texture2D* texture,
            const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) = 0;
        //! \see WriteSubTexture(Texture1D*, const Texture1D::PositionType&, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        virtual void WriteSubTexture(
            Texture2D* texture,
            const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) = 0;

        //! \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int)
        virtual void WriteTexture(
            Texture3D* texture, const TextureFormats textureFormat, const Texture3D::SizeType& textureSize
        );
        //! Creates the texture data from the specified image.
        virtual void WriteTexture(Texture3D* texture, const ImageUByte& image);
        //! \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        virtual void WriteTexture(
            Texture3D* texture,
            const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) = 0;
        //! \see WriteSubTexture(Texture1D*, const Texture1D::PositionType&, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        virtual void WriteSubTexture(
            Texture3D* texture,
            const Texture3D::PositionType& position, const Texture3D::SizeType& size,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) = 0;

        //! \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int)
        virtual void WriteTexture(
            TextureCube* texture,
            const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize = 0
        );
        //! \see WriteTexture(Texture1D*, const TextureFormats, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        virtual void WriteTexture(
            TextureCube* texture,
            const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData
        ) = 0;
        //! \see WriteSubTexture(Texture1D*, const Texture1D::PositionType&, const Texture1D::SizeType&, unsigned int, const ImageColorFormats, const RendererDataTypes, const void*)
        virtual void WriteSubTexture(
            TextureCube* texture,
            const TextureCube::PositionType& position, const TextureCube::SizeType& size,
            const TextureCube::Faces faceDirection, unsigned int arrayIndex,
            const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
            unsigned int mipLevel = 0
        ) = 0;

        #if 0

        /**
        Reads the specified texture data into the output image buffer.
        \param[in] texture Raw pointer to the texture whose image data is to be read.
        \param[out] image Output image which will contain the image data.
        \param[in] faceDirection Specifies the face direction for a cube map texture.
        \param[in] mipLevel Specifies the MIP-map level from which the image data is to be read. By default 0.
        \return True if the texture data has been read successful.
        Otherwise an error occured which will be printed to the log output.
        */
        virtual bool ReadTexture(
            const Texture* texture, ImageUByte& image,
            const TextureCube::Faces faceDirection, unsigned int mipLevel = 0
        ) = 0;
        //! \see ReadTexture(const Texture*, ImageUByte&, const TextureCube::Faces, unsigned int)
        virtual bool ReadTexture(
            const Texture* texture, ImageFloat& image,
            const TextureCube::Faces faceDirection, unsigned int mipLevel = 0
        ) = 0;

        #endif

        //! \see ReadTexture(const Texture*, ImageUByte&, const TextureCube::Faces, unsigned int)
        virtual bool ReadTexture(const Texture* texture, ImageUByte& image, unsigned int mipLevel = 0) = 0;
        //! \see ReadTexture(const Texture*, ImageUByte&, const TextureCube::Faces, unsigned int)
        virtual bool ReadTexture(const Texture* texture, ImageFloat& image, unsigned int mipLevel = 0) = 0;

        /* --- Render targets --- */

        /**
        Creates a new render target.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        */
        virtual RenderTargetPtr CreateRenderTarget() = 0;
        /**
        Creates a new render target and attaches all textures from the specified list.
        \see RenderTarget::Attach(const std::vector<TexturePtr>&)
        \see CreateRenderTarget()
        */
        RenderTargetPtr CreateRenderTarget(const std::vector<TexturePtr>& textures);
        /**
        Creates six render targets for the specified cube map texture.
        \see TextureCube::FaceRenderTargets
        */
        TextureCube::FaceRenderTargets CreateCubeRenderTargets(const TextureCubePtr& textureCube);
        /**
        Creates six render targets for the specified cube map texture list.
        \see TextureCube::FaceRenderTargets
        */
        TextureCube::FaceRenderTargets CreateCubeRenderTargets(const std::vector<TextureCubePtr>& textureCubes);

        /* --- Shaders --- */

        /**
        Creates a new vertex shader.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see VertexShader
        */
        virtual VertexShaderPtr CreateVertexShader() = 0;
        /**
        Creates a new pixel shader.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see PixelShader
        */
        virtual PixelShaderPtr CreatePixelShader() = 0;
        /**
        Creates a new geometry shader.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see GeometryShader
        */
        virtual GeometryShaderPtr CreateGeometryShader() = 0;
        /**
        Creates a new tessellation control shader (also called "Hull Shader").
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see TessControlShader
        */
        virtual TessControlShaderPtr CreateTessControlShader() = 0;
        /**
        Creates a new tessellation evaluation shader (also called "Domain Shader").
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see TessEvaluationShader
        */
        virtual TessEvaluationShaderPtr CreateTessEvaluationShader() = 0;
        /**
        Creates a new compute shader.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see ComputeShader
        */
        virtual ComputeShaderPtr CreateComputeShader() = 0;
        /**
        Creates a new shader composition. Shader compositions store several shaders.
        A common shader composition consists of at least a vertex- and a pixel shader.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see ShaderComposition
        */
        virtual ShaderCompositionPtr CreateShaderComposition() = 0;

        /**
        Returns the constant buffer for special render states or null if this render system does not have such a constant buffer.
        \code
        struct renderStateBuffer
        {
            int isInvScreen; // 'Inverse-screen' state variable. Either 1 (true) or 0 (false).
            int __pad0[3];   // Unused to fill the 4-component vector in the constant buffer.
        };
        \endcode
        \see RenderContext::GlobalRenderState
        */
        virtual ConstantBufferPtr GetRenderStateConstBuffer() const;

        /* --- Query --- */

        /**
        Creates a new renderer query object.
        \param[in] type Specifies the query object type.
        \remarks This will temporarily activate the shared render context automatically (see 'ActivateSharedContext').
        \see ActivateSharedContext
        \see Query
        \see Query::Types
        */
        virtual QueryPtr CreateQuery(const Query::Types type) = 0;

        /* === Templates === */

        /**
        Updates the entire hardware buffer.
        \tparam Buffer Specifies the hardware buffer type. This must be VertexBuffer, IndexBuffer, ConstantBuffer or StorageBuffer.
        \tparam Container Specifies the buffer container type. This must not be a pointer type!
        \see WriteSubBuffer
        */
        template <class Buffer, class Container> void UpdateBuffer(Buffer* hardwareBuffer, const Container& buffer)
        {
            static_assert(!std::is_pointer<Container>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
            WriteSubBuffer(hardwareBuffer, &buffer, sizeof(Container), 0);
        }

        /* === Inline functions === */

        /**
        Returns a raw-pointer to the common texture manager.
        You can also created further texture managers for different purposes.
        */
        inline TextureManager* GetTextureManager() const
        {
            return textureMngr_.get();
        }
        /**
        Returns a raw-pointer to the common shader manager.
        You can also created further shader managers for different purposes.
        */
        inline ShaderManager* GetShaderManager() const
        {
            return shaderMngr_.get();
        }

        //! Returns a raw-pointer to the shader provider.
        inline ShaderProvider* GetShaderProvider() const
        {
            return shaderProvider_.get();
        }

        //! Returns the list of all render contexts.
        inline const std::vector<RenderContextPtr>& GetRenderContexts() const
        {
            return renderContexts_;
        }

        /**
        Returns the renderer profiler model data. This will only be used
        (i.e. filled with content) if a profiler render system has been created.
        */
        inline const RendererProfilerModel& GetProfilerModel() const
        {
            return profilerModel_;
        }

        //! Returns the default rasterizer state.
        inline const RasterizerState* GetDefaultRasterizerState() const
        {
            return defaultRasterizerState_.get();
        }
        //! Returns the defautl depth-stencil state.
        inline const DepthStencilState* GetDefaultDepthStencilState() const
        {
            return defaultDepthStencilState_.get();
        }
        //! Returns the default blend state.
        inline const BlendState* GetDefaultBlendState() const
        {
            return defaultBlendState_.get();
        }

        /**
        Returns a raw pointer to the empty vertex buffer.
        \remarks An empty vertex buffer can be used for vertex shaders without vertex attributes.
        \see RenderContext::BindVertexBuffer(VertexBuffer*)
        */
        inline VertexBuffer* GetEmptyVertexBuffer() const
        {
            return emptyVertexBuffer_.get();
        }

        /**
        Returns the shared render context.
        \return Raw pointer to the shared render context or null if this render system does not have a shared render context.
        \remarks A shared render context is only used by the OpenGL render system. This is the primary render context
        which shares the GL lists (or rather graphics hardware objects) with all other secondary render contexts.
        To keep your code renderer independent, you should activate this render context before creating new
        renderer objects such as sampler states, textures or any other hardware buffer.
        \see ScopedActiveRenderContext
        */
        inline RenderContext* GetSharedRenderContext() const
        {
            return sharedRenderContext_;
        }

        /* === Members === */

        /**
        Texture initial color. This will be used, when "WriteTexture" is called
        and a null pointer is passed as image data. By default { 255, 255, 255, 255 }.
        */
        ColorRGBAub textureInitColor;

    protected:
        
        /* === Functions === */

        RenderSystem();

        //! \note This pointer must never be null!
        void ChangeBufferFormat(IndexBuffer* indexBuffer, const IndexFormat& format);
        //! \note This pointer must never be null!
        void ChangeBufferFormat(VertexBuffer* vertexBuffer, const VertexFormat& format);
        //! \note This pointer must never be null!
        void ChangeTextureFormat(Texture* texture, TextureFormats format);

        /**
        Creates and initializes the default render states (rasterizer-, depth-stenicl- and blend states),
        creates an empty vertex buffer and the shader provider.
        */
        void CreateDefaultResources(const ContextDescription& contextDesc);

        //! Creates a renderer specific shader prodiver.
        virtual std::unique_ptr<ShaderProvider> CreateShaderProvider() = 0;

        void InitSharedRenderContext(RenderContext* sharedRenderContext);

        //! \throws InvalidArgumentException If 'imageDataType' is RendererDataTypes::Double.
        void ValidateImageDataType(const RendererDataTypes imageDataType);

        /* === Members === */

        std::vector<RenderContextPtr>   renderContexts_;

        RendererProfilerModel           profilerModel_;

    private:
        
        static RenderSystem*            activeRenderSystem_;

        std::unique_ptr<TextureManager> textureMngr_;
        std::unique_ptr<ShaderManager>  shaderMngr_;
        std::unique_ptr<ShaderProvider> shaderProvider_;

        RasterizerStatePtr              defaultRasterizerState_;
        DepthStencilStatePtr            defaultDepthStencilState_;
        BlendStatePtr                   defaultBlendState_;

        VertexBufferPtr                 emptyVertexBuffer_;

        /**
        Main render context to share its resources with all other render contexts.
        \note This must be stored in this base class, although some render systems don't need it.
        This is due to the release order of renderer resources.
        */
        RenderContext*                  sharedRenderContext_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================