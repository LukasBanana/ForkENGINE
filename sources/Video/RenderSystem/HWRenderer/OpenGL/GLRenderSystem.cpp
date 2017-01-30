/*
 * OpenGL render system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLRenderSystem.h"
#include "GLRenderContext.h"
#include "GLParamMapper.h"
#include "GLCore.h"
#include "GLQuery.h"
#include "Core/CastToConcreteObject.h"

#include "HardwareBuffer/GLVertexBuffer.h"
#include "HardwareBuffer/GLIndexBuffer.h"
#include "HardwareBuffer/GLConstantBuffer.h"
#include "HardwareBuffer/GLStorageBuffer.h"

#include "RenderState/GLBlendState.h"
#include "RenderState/GLRasterizerState.h"
#include "RenderState/GLDepthStencilState.h"
#include "RenderState/GLSamplerState.h"

#include "Shader/GLVertexShader.h"
#include "Shader/GLPixelShader.h"
#include "Shader/GLGeometryShader.h"
#include "Shader/GLTessControlShader.h"
#include "Shader/GLTessEvaluationShader.h"
#include "Shader/GLComputeShader.h"
#include "Shader/GLShaderComposition.h"
#include "Shader/GLShaderProvider.h"

#include "Texture/GLTexture1D.h"
#include "Texture/GLTexture2D.h"
#include "Texture/GLTexture3D.h"
#include "Texture/GLTextureCube.h"
#include "Texture/GLClearTexture.h"
#include "Texture/GLRenderTarget.h"

#include "Core/StringModifier.h"
#include "Core/Exception/PointerConversionException.h"
#include "Core/Exception/InvalidArgumentException.h"

#include "Platform/Core/Desktop.h"
#include "IO/Core/Log.h"
#include "Video/RenderSystem/ScopedActiveRenderContext.h"


namespace Fork
{

namespace Video
{


/*
 * Internal functions
 */

#ifdef FORK_DEBUG

void APIENTRY GLRenderSystemDebugCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    IO::Log::Debug(
        "GL Callback (" + GLDebugSourceToStr(source) + ", " + GLDebugTypeToStr(type) +
        ", " + GLDebugSeverityToStr(severity) + ") -- " + ToStr(message), 10
    );
}

#endif


/*
 * GLRenderSystem class
 */

GLRenderSystem::GLRenderSystem()
{
}
GLRenderSystem::~GLRenderSystem()
{
}

RenderContextPtr GLRenderSystem::CreateRenderContext(
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame)
{
    /* Create OpenGL render context */
    return InitRenderContext(
        std::make_shared<GLRenderContext>(
            dynamic_cast<GLRenderContext*>(GetSharedRenderContext()),
            videoMode, contextDesc,
            frameTitle, parentFrame
        ),
        videoMode, contextDesc
    );
}

const Math::Matrix4f& GLRenderSystem::ProjectionMatrix(const Projectionf& projection) const
{
    return projection.GetMatrixRH();
}

void GLRenderSystem::EnableDebugDump(bool enableDebugDump)
{
    #ifdef FORK_DEBUG

    if (glDebugMessageCallback)
    {
        if (enableDebugDump)
        {
            glDebugMessageCallback(GLRenderSystemDebugCallback, nullptr);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
        else
        {
            glDebugMessageCallback(nullptr, nullptr);
            glDisable(GL_DEBUG_OUTPUT);
            glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
    }

    #endif
}

/* --- Render states --- */

RasterizerStatePtr GLRenderSystem::CreateRasterizerState(const RasterizerState::Description& desc)
{
    return std::make_shared<GLRasterizerState>(desc);
}

DepthStencilStatePtr GLRenderSystem::CreateDepthStencilState(const DepthStencilState::Description& desc)
{
    return std::make_shared<GLDepthStencilState>(desc);
}

BlendStatePtr GLRenderSystem::CreateBlendState(const BlendState::Description& desc)
{
    return std::make_shared<GLBlendState>(desc);
}

SamplerStatePtr GLRenderSystem::CreateSamplerState(const SamplerState::Description& desc)
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLSamplerState>(desc);
}

void GLRenderSystem::ChangeStateDesc(RasterizerState* state, const RasterizerState::Description& desc)
{
    auto stateGL = CAST_TO_GL_OBJECT(RasterizerState, state);
    stateGL->ChangeDesc(desc);
}

void GLRenderSystem::ChangeStateDesc(DepthStencilState* state, const DepthStencilState::Description& desc)
{
    auto stateGL = CAST_TO_GL_OBJECT(DepthStencilState, state);
    stateGL->ChangeDesc(desc);
}

void GLRenderSystem::ChangeStateDesc(BlendState* state, const BlendState::Description& desc)
{
    auto stateGL = CAST_TO_GL_OBJECT(BlendState, state);
    stateGL->ChangeDesc(desc);
}

void GLRenderSystem::ChangeStateDesc(SamplerState* state, const SamplerState::Description& desc)
{
    auto stateGL = CAST_TO_GL_OBJECT(SamplerState, state);
    stateGL->ChangeDesc(desc);
}

/* --- Hardware buffers --- */

VertexBufferPtr GLRenderSystem::CreateVertexBuffer()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLVertexBuffer>();
}

IndexBufferPtr GLRenderSystem::CreateIndexBuffer()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLIndexBuffer>();
}

ConstantBufferPtr GLRenderSystem::CreateConstantBuffer(const ConstantBuffer::Description& desc)
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLConstantBuffer>(desc);
}

StorageBufferPtr GLRenderSystem::CreateStorageBuffer(const StorageBuffer::Description& desc)
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLStorageBuffer>(desc);
}

void GLRenderSystem::WriteBuffer(
    VertexBuffer* vertexBuffer, const VertexFormat& format, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    /* Get GL vertex buffer */
    auto vertexBufferGL = CAST_TO_GL_OBJECT(VertexBuffer, vertexBuffer);

    /* Change buffer format and write data */
    ChangeBufferFormat(vertexBuffer, format);
    
    vertexBufferGL->Bind();
    vertexBufferGL->Write(data, dataSize, GLParamMapper::Map(usage));

    /* Setup VAO with vertex format */
    vertexBuffer->SetupFormat(format);
}

void GLRenderSystem::WriteBuffer(
    IndexBuffer* indexBuffer, const IndexFormat& format, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    /* Get GL index buffer */
    auto indexBufferGL = CAST_TO_GL_OBJECT(IndexBuffer, indexBuffer);
    
    /* Change buffer format and write data */
    ChangeBufferFormat(indexBuffer, format);

    indexBufferGL->Bind();
    indexBufferGL->Write(data, dataSize, GLParamMapper::Map(usage));
}

void GLRenderSystem::WriteBuffer(
    ConstantBuffer* constantBuffer, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    /* Get GL constant buffer and write data */
    auto constantBufferGL = CAST_TO_GL_OBJECT(ConstantBuffer, constantBuffer);
    constantBufferGL->Bind();
    constantBufferGL->Write(data, dataSize, GLParamMapper::Map(usage));
}

void GLRenderSystem::WriteBuffer(
    StorageBuffer* storageBuffer, const void* data, size_t dataSize, const HardwareBufferUsage usage)
{
    /* Get GL storage buffer and write data */
    auto storageBufferGL = CAST_TO_GL_OBJECT(StorageBuffer, storageBuffer);
    storageBufferGL->Bind();
    storageBufferGL->Write(data, dataSize, GLParamMapper::Map(usage));
}

void GLRenderSystem::WriteSubBuffer(VertexBuffer* vertexBuffer, const void* data, size_t dataSize, size_t offset)
{
    /* Get GL vertex buffer and write sub data */
    auto vertexBufferGL = CAST_TO_GL_OBJECT(VertexBuffer, vertexBuffer);
    vertexBufferGL->Bind();
    vertexBufferGL->WriteSub(data, dataSize, offset);
}

void GLRenderSystem::WriteSubBuffer(IndexBuffer* indexBuffer, const void* data, size_t dataSize, size_t offset)
{
    /* Get GL index buffer and write sub data */
    auto indexBufferGL = CAST_TO_GL_OBJECT(IndexBuffer, indexBuffer);
    indexBufferGL->Bind();
    indexBufferGL->WriteSub(data, dataSize, offset);
}

void GLRenderSystem::WriteSubBuffer(ConstantBuffer* constantBuffer, const void* data, size_t dataSize, size_t offset)
{
    /* Get GL constant buffer and write sub data */
    auto constantBufferGL = CAST_TO_GL_OBJECT(ConstantBuffer, constantBuffer);
    constantBufferGL->Bind();
    constantBufferGL->WriteSub(data, dataSize, offset);
}

void* GLRenderSystem::MapBuffer(StorageBuffer* storageBuffer, const HardwareBufferCPUAccess cpuAccess)
{
    /* Get GL storage buffer and map buffer */
    auto storageBufferGL = CAST_TO_GL_OBJECT(StorageBuffer, storageBuffer);
    storageBufferGL->Bind();
    return glMapBuffer(GLStorageBuffer::target, GLParamMapper::Map(cpuAccess));
}

void GLRenderSystem::UnmapBuffer(StorageBuffer* storageBuffer)
{
    /* Get GL storage buffer and unmap buffer */
    auto storageBufferGL = CAST_TO_GL_OBJECT(StorageBuffer, storageBuffer);
    storageBufferGL->Bind();
    glUnmapBuffer(GLStorageBuffer::target);
}

/* --- Textures --- */

Texture1DPtr GLRenderSystem::CreateTexture1D()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLTexture1D>();
}

Texture2DPtr GLRenderSystem::CreateTexture2D()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLTexture2D>();
}

Texture3DPtr GLRenderSystem::CreateTexture3D()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLTexture3D>();
}

TextureCubePtr GLRenderSystem::CreateTextureCube()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLTextureCube>();
}

static void InitGLTextureFilterAndUnbind(GLenum target)
{
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(target, 0);
}

void GLRenderSystem::WriteTexture(
    Texture1D* texture,
    const TextureFormats textureFormat, const Texture1D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    //...
}

void GLRenderSystem::WriteSubTexture(
    Texture1D* texture,
    const Texture1D::PositionType& position, const Texture1D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    //...
}

void GLRenderSystem::WriteTexture(
    Texture2D* texture,
    const TextureFormats textureFormat, const Texture2D::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    ValidateImageDataType(imageDataType);

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(Texture2D, texture);

    /* Setup texture attributes */
    textureGL->SetupSize(textureSize);
    textureGL->SetupArray(arraySize);

    ChangeTextureFormat(texture, textureFormat);

    /* Write texture data */
    const auto target = textureGL->GetTarget();

    glBindTexture(target, textureGL->GetTextureID());

    if (texture->HasArray())
    {
        FLUSH_GL_ERROR;

        /* Write image data to the entire 2D texture array */
        glTexImage3D(
            target,                                     // Texture target
            0,                                          // Only write the first MIP layer
            GLParamMapper::Map(textureFormat, true),    // Internal hardware format
            textureSize.width,                          // Texture width
            textureSize.height,                         // Texture height
            static_cast<int>(texture->GetArraySize()),  // 2D array size
            0,                                          // No border
            GLParamMapper::Map(imageFormat, false),     // Image color format
            GLParamMapper::Map(imageDataType),          // Image data type
            imageData                                   // Image buffer
        );

        SHOW_GL_ERROR("WriteTexture(2D)/glTexImage3D");
    }
    else
    {
        FLUSH_GL_ERROR;

        /* Write image data to the 2D texture */
        glTexImage2D(
            target,                                     // Texture target
            0,                                          // Only write the first MIP layer
            GLParamMapper::Map(textureFormat, true),    // Internal hardware format
            textureSize.width,                          // Texture width
            textureSize.height,                         // Texture height
            0,                                          // No border
            GLParamMapper::Map(imageFormat, false),     // Image color format
            GLParamMapper::Map(imageDataType),          // Image data type
            imageData                                   // Image buffer
        );

        SHOW_GL_ERROR("WriteTexture(2D)/glTexImage2D");
    }

    if (!imageData)
    {
        /* Initialize texture data */
        GLClearTexture2D(textureGL, textureInitColor);
    }

    /* Setup initial texture parameters */
    InitGLTextureFilterAndUnbind(target);
}

void GLRenderSystem::WriteSubTexture(
    Texture2D* texture,
    const Texture2D::PositionType& position, const Texture2D::SizeType& size, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    ValidateImageDataType(imageDataType);

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(Texture2D, texture);

    /* Write texture data */
    const auto target = textureGL->GetTarget();

    glBindTexture(target, textureGL->GetTextureID());

    if (texture->HasArray())
    {
        FLUSH_GL_ERROR;

        /* Write image data to specified 2D array layer */
        glTexSubImage3D(
            target,                                 // Texture target
            static_cast<GLint>(mipLevel),           // Write to the specified MIP layer
            position.x,                             // Offset X
            position.y,                             // Offset Y
            static_cast<GLint>(arrayIndex),         // 2D array layer
            size.width,                             // Width
            size.height,                            // Height
            1,                                      // Depth for a single layer is always 1
            GLParamMapper::Map(imageFormat, false), // Image color format
            GLParamMapper::Map(imageDataType),      // Image data type
            imageData                               // Image sub-buffer
        );

        SHOW_GL_ERROR("WriteSubTexture(2D)/glTexSubImage3D");
    }
    else
    {
        FLUSH_GL_ERROR;

        /* Write image data to sub-region in 2D texture */
        glTexSubImage2D(
            target,                                 // Texture target
            static_cast<GLint>(mipLevel),           // Write to the specified MIP layer
            position.x,                             // Offset X
            position.y,                             // Offset Y
            size.width,                             // Width
            size.height,                            // Height
            GLParamMapper::Map(imageFormat, false), // Image color format
            GLParamMapper::Map(imageDataType),      // Image data type
            imageData                               // Image sub-buffer
        );

        SHOW_GL_ERROR("WriteSubTexture(2D)/glTexSubImage2D");
    }

    glBindTexture(target, 0);
}

void GLRenderSystem::WriteTexture(
    Texture3D* texture,
    const TextureFormats textureFormat, const Texture3D::SizeType& textureSize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    //...
}

void GLRenderSystem::WriteSubTexture(
    Texture3D* texture,
    const Texture3D::PositionType& position, const Texture3D::SizeType& size,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    //...
}

void GLRenderSystem::WriteTexture(
    TextureCube* texture,
    const TextureFormats textureFormat, const TextureCube::SizeType& textureSize, unsigned int arraySize,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData)
{
    ValidateImageDataType(imageDataType);

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(TextureCube, texture);

    /* Setup texture attributes */
    textureGL->SetupSize(textureSize);
    textureGL->SetupArray(arraySize);

    ChangeTextureFormat(texture, textureFormat);

    /* Write texture data */
    const auto target = textureGL->GetTarget();

    glBindTexture(target, textureGL->GetTextureID());

    if (texture->HasArray())
    {
        FLUSH_GL_ERROR;

        /* Write image data to the entire cube texture array */
        glTexImage3D(
            target,                                         // Texture target (for cube array textures always GL_TEXTURE_CUBE_MAP_ARRAY)
            0,                                              // Only write the first MIP layer
            GLParamMapper::Map(textureFormat, true),        // Internal hardware format
            textureSize.width,                              // Cube texture width
            textureSize.height,                             // Cube texture height
            static_cast<GLint>(texture->GetArraySize())*6,  // Multiply array size by 6 since we have six cube faces
            0,                                              // No border
            GLParamMapper::Map(imageFormat, false),         // Image color format
            GLParamMapper::Map(imageDataType),              // Image data type
            imageData                                       // Image buffer
        );

        SHOW_GL_ERROR("WriteTexture(Cube)/glTexImage3D");
    }
    else
    {
        FLUSH_GL_ERROR;

        /* Get image data for the current face and sub-buffer stride */
        auto imageDataFace = reinterpret_cast<const char*>(imageData);

        const auto imageDataFaceStride =
            textureSize.Area() *
            Video::NumColorComponents(imageFormat) *
            Video::GetDataTypeSize(imageDataType);

        for (GLint face : { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z })
        {
            /* Write image data to the current cube texture face */
            glTexImage2D(
                face,                                       // Texture target (individual cube face)
                0,                                          // Only write the first MIP layer
                GLParamMapper::Map(textureFormat, true),    // Internal hardware format
                textureSize.width,                          // Cube texture width
                textureSize.height,                         // Cube texture height
                0,                                          // No border
                GLParamMapper::Map(imageFormat, false),     // Image color format
                GLParamMapper::Map(imageDataType),          // Image data type
                imageDataFace                               // Image buffer for the current cube face
            );

            /* Only move to the next image sub-buffer when it's not null */
            if (imageDataFace)
                imageDataFace += imageDataFaceStride;
        }

        SHOW_GL_ERROR("WriteTexture(Cube)/glTexImage2D");
    }

    if (!imageData)
    {
        /* Initialize texture data */
        GLClearTextureCube(textureGL, textureInitColor);
    }

    /* Setup initial texture parameters */
    InitGLTextureFilterAndUnbind(target);
}

void GLRenderSystem::WriteSubTexture(
    TextureCube* texture,
    const TextureCube::PositionType& position, const TextureCube::SizeType& size,
    const TextureCube::Faces faceDirection, unsigned int arrayIndex,
    const ImageColorFormats imageFormat, const RendererDataTypes imageDataType, const void* imageData,
    unsigned int mipLevel)
{
    ValidateImageDataType(imageDataType);

    /* Get GL texture */
    auto textureGL = CAST_TO_GL_OBJECT(TextureCube, texture);

    /* Write texture data */
    const auto target = textureGL->GetTarget();

    glBindTexture(target, textureGL->GetTextureID());

    if (texture->HasArray())
    {
        FLUSH_GL_ERROR;

        /* Get 3D texture layer index: (ArrayIndex * 6 + FaceIndex) */
        const auto texLayer = static_cast<GLint>(TextureCube::MapArrayLayer(arrayIndex, faceDirection));

        /* Write image data to specified cube array layer */
        glTexSubImage3D(
            target,                                 // Texture target
            static_cast<GLint>(mipLevel),           // Write to the specified MIP layer
            position.x,                             // Offset X
            position.y,                             // Offset Y
            texLayer,                               // Cube array layer
            size.width,                             // Width
            size.height,                            // Height
            1,                                      // Depth for a single layer is always 1
            GLParamMapper::Map(imageFormat, false), // Image color format
            GLParamMapper::Map(imageDataType),      // Image data type
            imageData                               // Image sub-buffer
        );

        SHOW_GL_ERROR("WriteSubTexture(Cube)/glTexSubImage3D");
    }
    else
    {
        FLUSH_GL_ERROR;

        /* Write image data to sub-region in cube texture */
        glTexSubImage2D(
            GLParamMapper::Map(faceDirection),      // Texture target
            static_cast<GLint>(mipLevel),           // Write to the specified MIP layer
            position.x,                             // Offset X
            position.y,                             // Offset Y
            size.width,                             // Width
            size.height,                            // Height
            GLParamMapper::Map(imageFormat, false), // Image color format
            GLParamMapper::Map(imageDataType),      // Image data type
            imageData                               // Image sub-buffer
        );

        SHOW_GL_ERROR("WriteSubTexture(Cube)/glTexSubImage2D");
    }

    glBindTexture(target, 0);
}

/*
This function returns the size of the MIP-map level of the currently bound GL texture.
This is used by the "ReadTexture" functions to determine the exact hardware texture description.
*/
static Math::Size3st QueryMIPLevelSize(GLenum target, unsigned int mipLevel)
{
    GLint mipLevelGL = static_cast<GLint>(mipLevel);
    GLint width = 0, height = 0, depth;
    
    glGetTexLevelParameteriv(target, mipLevelGL, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(target, mipLevelGL, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(target, mipLevelGL, GL_TEXTURE_DEPTH, &depth);

    return { static_cast<size_t>(width), static_cast<size_t>(height), static_cast<size_t>(depth) };
}

static const GLTextureBase* GetGLTextureBase(const Texture* texture)
{
    auto textureGL = dynamic_cast<const GLTextureBase*>(texture);
    if (!textureGL)
        throw PointerConversionException(__FUNCTION__, "Texture", "GLTextureBase");
    return textureGL;
}

bool GLRenderSystem::ReadTexture(const Texture* texture, ImageUByte& image, unsigned int mipLevel)
{
    /* Get GL texture */
    auto textureGL = GetGLTextureBase(texture);
    const auto target = textureGL->GetTarget();

    /* Bind texture */
    glBindTexture(target, textureGL->GetTextureID());
    {
        /* Resize image buffer */
        image.Resize(QueryMIPLevelSize(target, mipLevel), ImageResizeModes::None);

        /* Read the image data from texture */
        glGetTexImage(
            target,
            static_cast<GLint>(mipLevel),
            GLParamMapper::Map(image.GetFormat()),
            GL_UNSIGNED_BYTE,
            image.RawBuffer()
        );
    }
    glBindTexture(target, 0);

    return true;
}

bool GLRenderSystem::ReadTexture(const Texture* texture, ImageFloat& image, unsigned int mipLevel)
{
    /* Get GL texture */
    auto textureGL = GetGLTextureBase(texture);
    const auto target = textureGL->GetTarget();

    /* Bind texture */
    glBindTexture(target, textureGL->GetTextureID());
    {
        /* Resize image buffer */
        image.Resize(QueryMIPLevelSize(target, mipLevel), ImageResizeModes::None);

        /* Read the image data from texture */
        glGetTexImage(
            target,
            static_cast<GLint>(mipLevel),
            GLParamMapper::Map(image.GetFormat()),
            GL_FLOAT,
            image.RawBuffer()
        );
    }
    glBindTexture(target, 0);

    return true;
}

/* --- Render targets --- */

RenderTargetPtr GLRenderSystem::CreateRenderTarget()
{
    // Don' ensure shared context -> framebuffer objects are not sharable!
    return std::make_shared<GLRenderTarget>();
}

/* --- Shaders --- */

VertexShaderPtr GLRenderSystem::CreateVertexShader()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLVertexShader>();
}

PixelShaderPtr GLRenderSystem::CreatePixelShader()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLPixelShader>();
}

GeometryShaderPtr GLRenderSystem::CreateGeometryShader()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLGeometryShader>();
}

TessControlShaderPtr GLRenderSystem::CreateTessControlShader()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLTessControlShader>();
}

TessEvaluationShaderPtr GLRenderSystem::CreateTessEvaluationShader()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLTessEvaluationShader>();
}

ComputeShaderPtr GLRenderSystem::CreateComputeShader()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLComputeShader>();
}

ShaderCompositionPtr GLRenderSystem::CreateShaderComposition()
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLShaderComposition>();
}

ConstantBufferPtr GLRenderSystem::GetRenderStateConstBuffer() const
{
    return renderStateConstBuffer_;
}

/* --- Query --- */

QueryPtr GLRenderSystem::CreateQuery(const Query::Types type)
{
    auto scopedContext = ActivateSharedContext();
    return std::make_shared<GLQuery>(type);
}


/*
 * ======= Protected: =======
 */

std::shared_ptr<GLRenderContext> GLRenderSystem::InitRenderContext(
    const std::shared_ptr<GLRenderContext>& renderContext, const VideoMode& videoMode, const ContextDescription& contextDesc)
{
    /*
    Load GL extensions and create default resources only
    once when the first render context is created
    */
    if (renderContexts_.empty())
    {
        LoadGLExtensions(contextDesc.rendererProfileDesc);
        InitSharedRenderContext(renderContext.get());
        CreateDefaultResources(contextDesc);
        CreateSharedResource();
        CreateRenderStateConstBuffer();
    }

    renderContexts_.push_back(renderContext);

    /* Pass constant buffer for special render states to the context */
    renderContext->InitRenderStateConstBuffer(renderStateConstBuffer_.get());

    /* Switch to fullscreen mode the first time */
    if (videoMode.isFullscreen)
        Platform::Desktop::ChangeVideoMode(videoMode);

    /* Show context frame */
    renderContext->GetFrame()->Show();

    return renderContext;
}


/*
 * ======= Private: =======
 */

void GLRenderSystem::LoadGLExtensions(const RendererProfileDescription& rendererProfileDesc)
{
    /* Load OpenGL extensions if not already done */
    if (extMap_.empty())
    {
        const bool useGLCoreProfile =
            rendererProfileDesc.useExtProfile &&
            rendererProfileDesc.useGLCoreProfile;

        /* Query extensions and load all of them */
        extMap_ = GLExtensionLoader::QueryExtensions(useGLCoreProfile);
        GLExtensionLoader::LoadAllExtensions(extMap_);

        #ifdef FORK_DEBUG
        /* Enable debug logging if extension is supported */
        EnableDebugDump(rendererProfileDesc.enableDebugDump);
        #endif
    }
}

void GLRenderSystem::CreateRenderStateConstBuffer()
{
    /* Create render state constant buffer */
    ConstantBuffer::Description constBufferDesc;
    {
        constBufferDesc.name = "renderStateBuffer";
        constBufferDesc.size = sizeof(Math::Vector4i);
    }
    renderStateConstBuffer_ = std::make_shared<GLConstantBuffer>(constBufferDesc);

    /* Initialize constant buffer */
    renderStateConstBuffer_->Bind();
    renderStateConstBuffer_->Write(nullptr, constBufferDesc.size);
}

std::unique_ptr<ShaderProvider> GLRenderSystem::CreateShaderProvider()
{
    return std::make_unique<GLShaderProvider>();
}


} // /namespace Video

} // /namespace Fork



// ========================