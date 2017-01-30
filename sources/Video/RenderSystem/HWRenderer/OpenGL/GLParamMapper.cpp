/*
 * OpenGL parameter mapper file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLParamMapper.h"


namespace Fork
{

namespace Video
{

namespace GLParamMapper
{


GLint Map(const TextureFormats textureFormat, bool disableInteger)
{
    if (disableInteger)
    {
        switch (textureFormat)
        {
            /* --- Base internal formats --- */
            case TextureFormats::DepthComponent:
                return GL_DEPTH_COMPONENT;
            case TextureFormats::DepthStencil:
                return GL_DEPTH_STENCIL;
            case TextureFormats::R:
                return GL_RED;
            case TextureFormats::RG:
                return GL_RG;
            case TextureFormats::RGB:
                return GL_RGB;
            case TextureFormats::RGBA:
                return GL_RGBA;

            /* --- Sized internal formats --- */
            case TextureFormats::R8UInt:
            case TextureFormats::R8SInt:
                return GL_R8;

            case TextureFormats::R16UInt:
            case TextureFormats::R16SInt:
                return GL_R16;
            case TextureFormats::R16Float:
                return GL_R16F;

            case TextureFormats::R32UInt:
            case TextureFormats::R32SInt:
            case TextureFormats::R32Float:
                return GL_R32F;

            case TextureFormats::RG8UInt:
            case TextureFormats::RG8SInt:
                return GL_RG8;

            case TextureFormats::RG16UInt:
            case TextureFormats::RG16SInt:
                return GL_RG16;
            case TextureFormats::RG16Float:
                return GL_RG16F;

            case TextureFormats::RG32UInt:
            case TextureFormats::RG32SInt:
            case TextureFormats::RG32Float:
                return GL_RG32F;

            case TextureFormats::RGB8UInt:
            case TextureFormats::RGB8SInt:
                return GL_RGB8;

            case TextureFormats::RGB16UInt:
            case TextureFormats::RGB16SInt:
                return GL_RGB16;
            case TextureFormats::RGB16Float:
                return GL_RGB16F;

            case TextureFormats::RGB32UInt:
            case TextureFormats::RGB32SInt:
            case TextureFormats::RGB32Float:
                return GL_RGB32F;

            case TextureFormats::RGBA8UInt:
            case TextureFormats::RGBA8SInt:
                return GL_RGBA8;

            case TextureFormats::RGBA16UInt:
            case TextureFormats::RGBA16SInt:
                return GL_RGBA16;
            case TextureFormats::RGBA16Float:
                return GL_RGBA16F;

            case TextureFormats::RGBA32UInt:
            case TextureFormats::RGBA32SInt:
            case TextureFormats::RGBA32Float:
                return GL_RGBA32F;
        }
    }
    else
    {
        switch (textureFormat)
        {
            /* --- Base internal formats --- */
            case TextureFormats::DepthComponent:
                return GL_DEPTH_COMPONENT;
            case TextureFormats::DepthStencil:
                return GL_DEPTH_STENCIL;
            case TextureFormats::R:
                return GL_RED;
            case TextureFormats::RG:
                return GL_RG;
            case TextureFormats::RGB:
                return GL_RGB;
            case TextureFormats::RGBA:
                return GL_RGBA;

            /* --- Sized internal formats --- */
            case TextureFormats::R8UInt:
                return GL_R8UI;
            case TextureFormats::R8SInt:
                return GL_R8I;

            case TextureFormats::R16UInt:
                return GL_R16UI;
            case TextureFormats::R16SInt:
                return GL_R16I;
            case TextureFormats::R16Float:
                return GL_R16F;

            case TextureFormats::R32UInt:
                return GL_R32UI;
            case TextureFormats::R32SInt:
                return GL_R32I;
            case TextureFormats::R32Float:
                return GL_R32F;

            case TextureFormats::RG8UInt:
                return GL_RG8UI;
            case TextureFormats::RG8SInt:
                return GL_RG8I;

            case TextureFormats::RG16UInt:
                return GL_RG16UI;
            case TextureFormats::RG16SInt:
                return GL_RG16I;
            case TextureFormats::RG16Float:
                return GL_RG16F;

            case TextureFormats::RG32UInt:
                return GL_RG32UI;
            case TextureFormats::RG32SInt:
                return GL_RG32I;
            case TextureFormats::RG32Float:
                return GL_RG32F;

            case TextureFormats::RGB8UInt:
                return GL_RGB8UI;
            case TextureFormats::RGB8SInt:
                return GL_RGB8I;

            case TextureFormats::RGB16UInt:
                return GL_RGB16UI;
            case TextureFormats::RGB16SInt:
                return GL_RGB16I;
            case TextureFormats::RGB16Float:
                return GL_RGB16F;

            case TextureFormats::RGB32UInt:
                return GL_RGB32UI;
            case TextureFormats::RGB32SInt:
                return GL_RGB32I;
            case TextureFormats::RGB32Float:
                return GL_RGB32F;

            case TextureFormats::RGBA8UInt:
                return GL_RGBA8UI;
            case TextureFormats::RGBA8SInt:
                return GL_RGBA8I;

            case TextureFormats::RGBA16UInt:
                return GL_RGBA16UI;
            case TextureFormats::RGBA16SInt:
                return GL_RGBA16I;
            case TextureFormats::RGBA16Float:
                return GL_RGBA16F;

            case TextureFormats::RGBA32UInt:
                return GL_RGBA32UI;
            case TextureFormats::RGBA32SInt:
                return GL_RGBA32I;
            case TextureFormats::RGBA32Float:
                return GL_RGBA32F;
        }
    }
    return 0;
}

/*
The value for 'isInteger' can be determined by the "IsIntegerType" function.
Note: The integer types currently make problems with OpenGL!
*/
GLenum Map(const ImageColorFormats imageFormat, bool isInteger)
{
    if (isInteger)
    {
        switch (imageFormat)
        {
            case ImageColorFormats::Gray:
                return GL_RED_INTEGER;
            case ImageColorFormats::GrayAlpha:
                return GL_RG_INTEGER;
            case ImageColorFormats::RGB:
                return GL_RGB_INTEGER;
            case ImageColorFormats::BGR:
                return GL_BGR_INTEGER;
            case ImageColorFormats::RGBA:
                return GL_RGBA_INTEGER;
            case ImageColorFormats::BGRA:
                return GL_BGRA_INTEGER;
            case ImageColorFormats::DepthComponent:
                return 0;
            case ImageColorFormats::DepthStencil:
                return 0;
        }
    }
    else
    {
        switch (imageFormat)
        {
            case ImageColorFormats::Gray:
                return GL_RED;
            case ImageColorFormats::GrayAlpha:
                return GL_RG;
            case ImageColorFormats::RGB:
                return GL_RGB;
            case ImageColorFormats::BGR:
                return GL_BGR;
            case ImageColorFormats::RGBA:
                return GL_RGBA;
            case ImageColorFormats::BGRA:
                return GL_BGRA;
            case ImageColorFormats::DepthComponent:
                return GL_DEPTH_COMPONENT;
            case ImageColorFormats::DepthStencil:
                return GL_DEPTH_STENCIL;
        }
    }
    return 0;
}

GLenum Map(const SamplerState::Filters magFilter)
{
    switch (magFilter)
    {
        case SamplerState::Filters::Nearest:
            return GL_NEAREST;
        case SamplerState::Filters::Linear:
            return GL_LINEAR;
    }
    return 0;
}

GLenum Map(const SamplerState::Filters minFilter, const SamplerState::Filters mipFilter, bool useMIPMaps)
{
    switch (minFilter)
    {
        case SamplerState::Filters::Nearest:
            if (useMIPMaps)
            {
                switch (mipFilter)
                {
                    case SamplerState::Filters::Nearest:
                        return GL_NEAREST_MIPMAP_NEAREST;
                    case SamplerState::Filters::Linear:
                        return GL_NEAREST_MIPMAP_LINEAR;
                }
            }
            return GL_NEAREST;

        case SamplerState::Filters::Linear:
            if (useMIPMaps)
            {
                switch (mipFilter)
                {
                    case SamplerState::Filters::Nearest:
                        return GL_LINEAR_MIPMAP_NEAREST;
                    case SamplerState::Filters::Linear:
                        return GL_LINEAR_MIPMAP_LINEAR;
                }
            }
            return GL_LINEAR;
    }
    return 0;
}

GLenum Map(const HardwareBufferUsage bufferUsage)
{
    switch (bufferUsage)
    {
        case HardwareBufferUsage::Static:
            return GL_STATIC_DRAW;
        case HardwareBufferUsage::Dynamic:
            return GL_DYNAMIC_DRAW;
    }
    return 0;
}

GLenum Map(const HardwareBufferCPUAccess cpuAccess)
{
    switch (cpuAccess)
    {
        case HardwareBufferCPUAccess::ReadOnly:
            return GL_READ_ONLY;
        case HardwareBufferCPUAccess::WriteOnly:
            return GL_WRITE_ONLY;
        case HardwareBufferCPUAccess::ReadWrite:
            return GL_READ_WRITE;
    }
    return 0;
}

GLenum Map(const RendererDataTypes dataType)
{
    switch (dataType)
    {
        case RendererDataTypes::Float:
            return GL_FLOAT;
        case RendererDataTypes::Double:
            return GL_DOUBLE;

        case RendererDataTypes::Byte:
            return GL_BYTE;
        case RendererDataTypes::UByte:
            return GL_UNSIGNED_BYTE;

        case RendererDataTypes::Short:
            return GL_SHORT;
        case RendererDataTypes::UShort:
            return GL_UNSIGNED_SHORT;

        case RendererDataTypes::Int:
            return GL_INT;
        case RendererDataTypes::UInt:
            return GL_UNSIGNED_INT;
    }
    return 0;
}

GLenum Map(const Shader::Types shaderType)
{
    switch (shaderType)
    {
        case Shader::Types::Vertex:
            return GL_VERTEX_SHADER;
        case Shader::Types::Pixel:
            return GL_FRAGMENT_SHADER;
        case Shader::Types::Geometry:
            return GL_GEOMETRY_SHADER;
        case Shader::Types::TessControl:
            return GL_TESS_CONTROL_SHADER;
        case Shader::Types::TessEvaluation:
            return GL_TESS_EVALUATION_SHADER;
        case Shader::Types::Compute:
            return GL_COMPUTE_SHADER;
    }
    return 0;
}

GLenum Map(const GeometryPrimitives primitiveType)
{
    switch (primitiveType)
    {
        case GeometryPrimitives::Points:
            return GL_POINTS;
        case GeometryPrimitives::Lines:
            return GL_LINES;
        case GeometryPrimitives::LineStrip:
            return GL_LINE_STRIP;
        case GeometryPrimitives::LineLoop:
            return GL_LINE_LOOP;
        case GeometryPrimitives::LinesAdjacency:
            return GL_LINES_ADJACENCY;
        case GeometryPrimitives::LineStripAdjacency:
            return GL_LINE_STRIP_ADJACENCY;
        case GeometryPrimitives::Triangles:
            return GL_TRIANGLES;
        case GeometryPrimitives::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case GeometryPrimitives::TriangleFan:
            return GL_TRIANGLE_FAN;
        case GeometryPrimitives::TrianglesAdjacency:
            return GL_TRIANGLES_ADJACENCY;
        case GeometryPrimitives::TriangleStripAdjacency:
            return GL_TRIANGLE_STRIP_ADJACENCY;
        case GeometryPrimitives::Patches:
            return GL_PATCHES;
    }
    return 0;
}

GLenum Map(const CompareFunctions compareFunc)
{
    switch (compareFunc)
    {
        case CompareFunctions::Never:
            return GL_NEVER;
        case CompareFunctions::Equal:
            return GL_EQUAL;
        case CompareFunctions::NotEqual:
            return GL_NOTEQUAL;
        case CompareFunctions::Less:
            return GL_LESS;
        case CompareFunctions::LessEqual:
            return GL_LEQUAL;
        case CompareFunctions::Greater:
            return GL_GREATER;
        case CompareFunctions::GreaterEqual:
            return GL_GEQUAL;
        case CompareFunctions::Always:
            return GL_ALWAYS;
    }
    return 0;
}

GLenum Map(const BlendState::BlendOps blendOp)
{
    switch (blendOp)
    {
        case BlendState::BlendOps::Zero:
            return GL_ZERO;
        case BlendState::BlendOps::One:
            return GL_ONE;
        case BlendState::BlendOps::SrcColor:
            return GL_SRC_COLOR;
        case BlendState::BlendOps::InvSrcColor:
            return GL_ONE_MINUS_SRC_COLOR;
        case BlendState::BlendOps::SrcAlpha:
            return GL_SRC_ALPHA;
        case BlendState::BlendOps::InvSrcAlpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case BlendState::BlendOps::DestColor:
            return GL_DST_COLOR;
        case BlendState::BlendOps::InvDestColor:
            return GL_ONE_MINUS_DST_COLOR;
        case BlendState::BlendOps::DestAlpha:
            return GL_DST_ALPHA;
        case BlendState::BlendOps::InvDestAlpha:
            return GL_ONE_MINUS_DST_ALPHA;
    }
    return 0;
}

GLenum Map(const DepthStencilState::StencilOps stencilOp)
{
    switch (stencilOp)
    {
        case DepthStencilState::StencilOps::Keep:
            return GL_KEEP;
        case DepthStencilState::StencilOps::Zero:
            return GL_ZERO;
        case DepthStencilState::StencilOps::Replace:
            return GL_REPLACE;
        case DepthStencilState::StencilOps::Inc:
            return GL_INCR;
        case DepthStencilState::StencilOps::IncWrap:
            return GL_INCR_WRAP;
        case DepthStencilState::StencilOps::Dec:
            return GL_DECR;
        case DepthStencilState::StencilOps::DecWrap:
            return GL_DECR_WRAP;
        case DepthStencilState::StencilOps::Invert:
            return GL_INVERT;
    }
    return 0;
}

GLenum Map(const RasterizerState::FillModes fillMode)
{
    switch (fillMode)
    {
        case RasterizerState::FillModes::Solid:
            return GL_FILL;
        case RasterizerState::FillModes::Wireframe:
            return GL_LINE;
        case RasterizerState::FillModes::Points:
            return GL_POINT;
    }
    return 0;
}

GLenum Map(const RasterizerState::CullModes cullMode)
{
    switch (cullMode)
    {
        case RasterizerState::CullModes::Front:
            return GL_FRONT;
        case RasterizerState::CullModes::Back:
            return GL_BACK;
        default:
            break;
    }
    return 0;
}

GLenum Map(const SamplerState::WrapModes wrapMode)
{
    switch (wrapMode)
    {
        case SamplerState::WrapModes::Repeat:
            return GL_REPEAT;
        case SamplerState::WrapModes::Mirror:
            return GL_MIRRORED_REPEAT;
        case SamplerState::WrapModes::Clamp:
            return GL_CLAMP_TO_EDGE;
    }
    return 0;
}

GLenum Map(const Math::AxisDirections axisDir)
{
    switch (axisDir)
    {
        case Math::AxisDirections::XPositive:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case Math::AxisDirections::XNegative:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case Math::AxisDirections::YPositive:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case Math::AxisDirections::YNegative:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        case Math::AxisDirections::ZPositive:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        case Math::AxisDirections::ZNegative:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    }
    return 0;
}

GLenum Map(const Query::Types queryType)
{
    switch (queryType)
    {
        case Query::Types::SamplesPassed:
            return GL_SAMPLES_PASSED;
        case Query::Types::AnySamplesPassed:
            return GL_ANY_SAMPLES_PASSED;
        case Query::Types::PrimitivesGenerated:
            return GL_PRIMITIVES_GENERATED;
        case Query::Types::TimeStamp:
            return GL_TIME_ELAPSED;
    }
    return 0;
}


} // /namespace GLParamMapper

} // /namespace Video

} // /namespace Fork



// ========================