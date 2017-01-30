/*
 * Direct3D 11 parameter mapper file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ParamMapper.h"


namespace Fork
{

namespace Video
{

namespace D3D11ParamMapper
{


/* ----- MAPPING ----- */

DXGI_FORMAT Map(const RendererDataTypes dataType, unsigned int numComponents, bool normalized)
{
    switch (numComponents)
    {
        /* --- 1 component --- */
        case 1:
            switch (dataType)
            {
                case RendererDataTypes::Float:
                    return DXGI_FORMAT_R32_FLOAT;
                case RendererDataTypes::Byte:
                    return normalized ? DXGI_FORMAT_R8_SNORM : DXGI_FORMAT_R8_SINT;
                case RendererDataTypes::UByte:
                    return normalized ? DXGI_FORMAT_R8_UNORM : DXGI_FORMAT_R8_UINT;
                case RendererDataTypes::Short:
                    return normalized ? DXGI_FORMAT_R16_SNORM : DXGI_FORMAT_R16_SINT;
                case RendererDataTypes::UShort:
                    return normalized ? DXGI_FORMAT_R16_UNORM : DXGI_FORMAT_R16_UINT;
                case RendererDataTypes::Int:
                    return DXGI_FORMAT_R32_SINT;
                case RendererDataTypes::UInt:
                    return DXGI_FORMAT_R32_UINT;
                default:
                    break;
            }
            break;

        /* --- 2 components --- */
        case 2:
            switch (dataType)
            {
                case RendererDataTypes::Float:
                    return DXGI_FORMAT_R32G32_FLOAT;
                case RendererDataTypes::Byte:
                    return normalized ? DXGI_FORMAT_R8G8_SNORM : DXGI_FORMAT_R8G8_SINT;
                case RendererDataTypes::UByte:
                    return normalized ? DXGI_FORMAT_R8G8_UNORM : DXGI_FORMAT_R8G8_UINT;
                case RendererDataTypes::Short:
                    return normalized ? DXGI_FORMAT_R16G16_SNORM : DXGI_FORMAT_R16G16_SINT;
                case RendererDataTypes::UShort:
                    return normalized ? DXGI_FORMAT_R16G16_UNORM : DXGI_FORMAT_R16G16_UINT;
                case RendererDataTypes::Int:
                    return DXGI_FORMAT_R32G32_SINT;
                case RendererDataTypes::UInt:
                    return DXGI_FORMAT_R32G32_UINT;
                default:
                    break;
            }
            break;

        /* --- 3 components --- */
        case 3:
            switch (dataType)
            {
                case RendererDataTypes::Float:
                    return DXGI_FORMAT_R32G32B32_FLOAT;
                case RendererDataTypes::Int:
                    return DXGI_FORMAT_R32G32B32_SINT;
                case RendererDataTypes::UInt:
                    return DXGI_FORMAT_R32G32B32_UINT;
                default:
                    break;
            }
            break;

        /* --- 4 components --- */
        case 4:
            switch (dataType)
            {
                case RendererDataTypes::Float:
                    return DXGI_FORMAT_R32G32B32A32_FLOAT;
                case RendererDataTypes::Byte:
                    return normalized ? DXGI_FORMAT_R8G8B8A8_SNORM : DXGI_FORMAT_R8G8B8A8_SINT;
                case RendererDataTypes::UByte:
                    return normalized ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UINT;
                case RendererDataTypes::Short:
                    return normalized ? DXGI_FORMAT_R16G16B16A16_SNORM : DXGI_FORMAT_R16G16B16A16_SINT;
                case RendererDataTypes::UShort:
                    return normalized ? DXGI_FORMAT_R16G16B16A16_UNORM : DXGI_FORMAT_R16G16B16A16_UINT;
                case RendererDataTypes::Int:
                    return DXGI_FORMAT_R32G32B32A32_SINT;
                case RendererDataTypes::UInt:
                    return DXGI_FORMAT_R32G32B32A32_UINT;
                default:
                    break;
            }
            break;
    }
    return DXGI_FORMAT_UNKNOWN;
}

D3D11_FILTER Map(const SamplerState::Filters minFilter, const SamplerState::Filters magFilter, const SamplerState::Filters mipFilter)
{
    if (minFilter == SamplerState::Filters::Nearest)
    {
        if (magFilter == SamplerState::Filters::Nearest)
        {
            if (mipFilter == SamplerState::Filters::Nearest)
                return D3D11_FILTER_MIN_MAG_MIP_POINT;
            else
                return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        }
        else
        {
            if (mipFilter == SamplerState::Filters::Nearest)
                return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
            else
                return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
        }
    }
    else
    {
        if (magFilter == SamplerState::Filters::Nearest)
        {
            if (mipFilter == SamplerState::Filters::Nearest)
                return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
            else
                return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
        }
        else
        {
            if (mipFilter == SamplerState::Filters::Nearest)
                return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            else
                return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }
    }
    return D3D11_FILTER_MIN_MAG_MIP_POINT;
}

DXGI_FORMAT Map(const TextureFormats textureFormat)
{
    switch (textureFormat)
    {
        /* --- Base internal formats --- */
        case TextureFormats::DepthComponent:
            return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case TextureFormats::DepthStencil:
            return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case TextureFormats::R:
            return DXGI_FORMAT_R8_UNORM;
        case TextureFormats::RG:
            return DXGI_FORMAT_R8G8_UNORM;
        case TextureFormats::RGB:
        case TextureFormats::RGBA:
            return DXGI_FORMAT_R8G8B8A8_UNORM;

        /* --- Sized internal formats --- */
        case TextureFormats::R8UInt:
            return DXGI_FORMAT_R8_UNORM;
        case TextureFormats::R8SInt:
            return DXGI_FORMAT_R8_SNORM;

        case TextureFormats::R16UInt:
            return DXGI_FORMAT_R16_UINT;
        case TextureFormats::R16SInt:
            return DXGI_FORMAT_R16_SINT;
        case TextureFormats::R16Float:
            return DXGI_FORMAT_R16_FLOAT;

        case TextureFormats::R32UInt:
            return DXGI_FORMAT_R32_UINT;
        case TextureFormats::R32SInt:
            return DXGI_FORMAT_R32_SINT;
        case TextureFormats::R32Float:
            return DXGI_FORMAT_R32_FLOAT;

        case TextureFormats::RG8UInt:
            return DXGI_FORMAT_R8G8_UNORM;
        case TextureFormats::RG8SInt:
            return DXGI_FORMAT_R8G8_SNORM;

        case TextureFormats::RG16UInt:
            return DXGI_FORMAT_R16G16_UINT;
        case TextureFormats::RG16SInt:
            return DXGI_FORMAT_R16G16_SINT;
        case TextureFormats::RG16Float:
            return DXGI_FORMAT_R16G16_FLOAT;

        case TextureFormats::RG32UInt:
            return DXGI_FORMAT_R32G32_UINT;
        case TextureFormats::RG32SInt:
            return DXGI_FORMAT_R32G32_SINT;
        case TextureFormats::RG32Float:
            return DXGI_FORMAT_R32G32_FLOAT;

        case TextureFormats::RGB8UInt:
        case TextureFormats::RGBA8UInt:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
        case TextureFormats::RGB8SInt:
        case TextureFormats::RGBA8SInt:
            return DXGI_FORMAT_R8G8B8A8_SNORM;

        case TextureFormats::RGB16UInt:
        case TextureFormats::RGBA16UInt:
            return DXGI_FORMAT_R16G16B16A16_UINT;
        case TextureFormats::RGB16SInt:
        case TextureFormats::RGBA16SInt:
            return DXGI_FORMAT_R16G16B16A16_SINT;
        case TextureFormats::RGB16Float:
        case TextureFormats::RGBA16Float:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;

        case TextureFormats::RGB32UInt:
        case TextureFormats::RGBA32UInt:
            return DXGI_FORMAT_R32G32B32A32_UINT;
        case TextureFormats::RGB32SInt:
        case TextureFormats::RGBA32SInt:
            return DXGI_FORMAT_R32G32B32A32_SINT;
        case TextureFormats::RGB32Float:
        case TextureFormats::RGBA32Float:
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }
    return DXGI_FORMAT_UNKNOWN;
}

D3D_FEATURE_LEVEL Map(const D3DFeatureLevels featureLevel)
{
    switch (featureLevel)
    {
        case D3DFeatureLevels::Default:
            return D3D_FEATURE_LEVEL_11_0;
        case D3DFeatureLevels::Level_9_1:
            return D3D_FEATURE_LEVEL_9_1;
        case D3DFeatureLevels::Level_9_2:
            return D3D_FEATURE_LEVEL_9_2;
        case D3DFeatureLevels::Level_9_3:
            return D3D_FEATURE_LEVEL_9_3;
        case D3DFeatureLevels::Level_10_0:
            return D3D_FEATURE_LEVEL_10_0;
        case D3DFeatureLevels::Level_10_1:
            return D3D_FEATURE_LEVEL_10_1;
        case D3DFeatureLevels::Level_11_0:
            return D3D_FEATURE_LEVEL_11_0;
        case D3DFeatureLevels::Level_11_1:
            return D3D_FEATURE_LEVEL_11_1;
        /*case D3DFeatureLevels::Level_11_2:
            return D3D_FEATURE_LEVEL_11_2;*/
    }
    return D3D_FEATURE_LEVEL_11_0;
}

D3D11_PRIMITIVE_TOPOLOGY Map(const GeometryPrimitives primitiveType)
{
    switch (primitiveType)
    {
        case GeometryPrimitives::Points:
            return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        case GeometryPrimitives::Lines:
            return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        case GeometryPrimitives::LineStrip:
            return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        case GeometryPrimitives::LinesAdjacency:
            return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
        case GeometryPrimitives::LineStripAdjacency:
            return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
        case GeometryPrimitives::Triangles:
            return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        case GeometryPrimitives::TriangleStrip:
            return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        case GeometryPrimitives::TrianglesAdjacency:
            return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
        case GeometryPrimitives::TriangleStripAdjacency:
            return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
        case GeometryPrimitives::Patches:
            return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;//!!!
        default:
            break;
    }
    return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
}

DXGI_FORMAT Map(const RendererDataTypes dataType)
{
    switch (dataType)
    {
        case RendererDataTypes::Float:
            return DXGI_FORMAT_R32_FLOAT;
        case RendererDataTypes::Byte:
            return DXGI_FORMAT_R8_SINT;
        case RendererDataTypes::UByte:
            return DXGI_FORMAT_R8_UINT;
        case RendererDataTypes::Short:
            return DXGI_FORMAT_R16_SINT;
        case RendererDataTypes::UShort:
            return DXGI_FORMAT_R16_UINT;
        case RendererDataTypes::Int:
            return DXGI_FORMAT_R32_SINT;
        case RendererDataTypes::UInt:
            return DXGI_FORMAT_R32_UINT;
        default:
            break;
    }
    return DXGI_FORMAT_UNKNOWN;
}

D3D11_USAGE Map(const HardwareBufferUsage usage)
{
    return usage == HardwareBufferUsage::Dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
}

D3D11_MAP Map(const HardwareBufferCPUAccess cpuAccess)
{
    switch (cpuAccess)
    {
        case HardwareBufferCPUAccess::ReadOnly:
            return D3D11_MAP_READ;
        case HardwareBufferCPUAccess::WriteOnly:
            return D3D11_MAP_WRITE;
        case HardwareBufferCPUAccess::ReadWrite:
            return D3D11_MAP_READ_WRITE;
    }
    return D3D11_MAP_READ;
}

D3D11_COMPARISON_FUNC Map(const CompareFunctions compareFunc)
{
    switch (compareFunc)
    {
        case CompareFunctions::Never:
            return D3D11_COMPARISON_NEVER;
        case CompareFunctions::Equal:
            return D3D11_COMPARISON_EQUAL;
        case CompareFunctions::NotEqual:
            return D3D11_COMPARISON_NOT_EQUAL;
        case CompareFunctions::Less:
            return D3D11_COMPARISON_LESS;
        case CompareFunctions::LessEqual:
            return D3D11_COMPARISON_LESS_EQUAL;
        case CompareFunctions::Greater:
            return D3D11_COMPARISON_GREATER;
        case CompareFunctions::GreaterEqual:
            return D3D11_COMPARISON_GREATER_EQUAL;
        case CompareFunctions::Always:
            return D3D11_COMPARISON_ALWAYS;
    }
    return D3D11_COMPARISON_NEVER;
}

D3D11_BLEND Map(const BlendState::BlendOps blendOp)
{
    switch (blendOp)
    {
        case BlendState::BlendOps::Zero:
            return D3D11_BLEND_ZERO;
        case BlendState::BlendOps::One:
            return D3D11_BLEND_ONE;
        case BlendState::BlendOps::SrcColor:
            return D3D11_BLEND_SRC_COLOR;
        case BlendState::BlendOps::InvSrcColor:
            return D3D11_BLEND_INV_SRC_COLOR;
        case BlendState::BlendOps::SrcAlpha:
            return D3D11_BLEND_SRC_ALPHA;
        case BlendState::BlendOps::InvSrcAlpha:
            return D3D11_BLEND_INV_SRC_ALPHA;
        case BlendState::BlendOps::DestColor:
            return D3D11_BLEND_DEST_COLOR;
        case BlendState::BlendOps::InvDestColor:
            return D3D11_BLEND_INV_DEST_COLOR;
        case BlendState::BlendOps::DestAlpha:
            return D3D11_BLEND_DEST_ALPHA;
        case BlendState::BlendOps::InvDestAlpha:
            return D3D11_BLEND_INV_DEST_ALPHA;
    }
    return D3D11_BLEND_ZERO;
}

D3D11_STENCIL_OP Map(const DepthStencilState::StencilOps stencilOp)
{
    switch (stencilOp)
    {
        case DepthStencilState::StencilOps::Keep:
            return D3D11_STENCIL_OP_KEEP;
        case DepthStencilState::StencilOps::Zero:
            return D3D11_STENCIL_OP_ZERO;
        case DepthStencilState::StencilOps::Replace:
            return D3D11_STENCIL_OP_REPLACE;
        case DepthStencilState::StencilOps::Inc:
            return D3D11_STENCIL_OP_INCR;
        case DepthStencilState::StencilOps::IncWrap:
            return D3D11_STENCIL_OP_INCR_SAT;
        case DepthStencilState::StencilOps::Dec:
            return D3D11_STENCIL_OP_DECR;
        case DepthStencilState::StencilOps::DecWrap:
            return D3D11_STENCIL_OP_DECR_SAT;
        case DepthStencilState::StencilOps::Invert:
            return D3D11_STENCIL_OP_INVERT;
    }
    return D3D11_STENCIL_OP_KEEP;
}

D3D11_FILL_MODE Map(const RasterizerState::FillModes fillMode)
{
    switch (fillMode)
    {
        case RasterizerState::FillModes::Solid:
            return D3D11_FILL_SOLID;
        case RasterizerState::FillModes::Wireframe:
        case RasterizerState::FillModes::Points:
            return D3D11_FILL_WIREFRAME;
    }
    return D3D11_FILL_SOLID;
}

D3D11_CULL_MODE Map(const RasterizerState::CullModes cullMode)
{
    switch (cullMode)
    {
        case RasterizerState::CullModes::None:
            return D3D11_CULL_NONE;
        case RasterizerState::CullModes::Front:
            return D3D11_CULL_FRONT;
        case RasterizerState::CullModes::Back:
            return D3D11_CULL_BACK;
    }
    return D3D11_CULL_NONE;
}

D3D11_TEXTURE_ADDRESS_MODE Map(const SamplerState::WrapModes wrapMode)
{
    switch (wrapMode)
    {
        case SamplerState::WrapModes::Repeat:
            return D3D11_TEXTURE_ADDRESS_WRAP;
        case SamplerState::WrapModes::Mirror:
            return D3D11_TEXTURE_ADDRESS_MIRROR;
        case SamplerState::WrapModes::Clamp:
            return D3D11_TEXTURE_ADDRESS_CLAMP;
    }
    return D3D11_TEXTURE_ADDRESS_WRAP;
}

D3D11_QUERY Map(const Query::Types queryType)
{
    switch (queryType)
    {
        case Query::Types::SamplesPassed:
            return D3D11_QUERY_OCCLUSION;
        case Query::Types::AnySamplesPassed:
            return D3D11_QUERY_OCCLUSION_PREDICATE;
        case Query::Types::PrimitivesGenerated:
            return D3D11_QUERY_SO_STATISTICS;
        case Query::Types::TimeStamp:
            return D3D11_QUERY_TIMESTAMP;
    }
    return D3D11_QUERY_EVENT;
}

/* ----- UNMAPPING ----- */

D3DFeatureLevels Unmap(const D3D_FEATURE_LEVEL featureLevel)
{
    switch (featureLevel)
    {
        case D3D_FEATURE_LEVEL_9_1:
            return D3DFeatureLevels::Level_9_1;
        case D3D_FEATURE_LEVEL_9_2:
            return D3DFeatureLevels::Level_9_2;
        case D3D_FEATURE_LEVEL_9_3:
            return D3DFeatureLevels::Level_9_3;
        case D3D_FEATURE_LEVEL_10_0:
            return D3DFeatureLevels::Level_10_0;
        case D3D_FEATURE_LEVEL_10_1:
            return D3DFeatureLevels::Level_10_1;
        case D3D_FEATURE_LEVEL_11_0:
            return D3DFeatureLevels::Level_11_0;
        case D3D_FEATURE_LEVEL_11_1:
            return D3DFeatureLevels::Level_11_1;
        /*case D3D_FEATURE_LEVEL_11_2:
            return D3DFeatureLevels::Level_11_2;*/
    }
    return D3DFeatureLevels::Level_11_0;
}

/* ----- EXTENDED ----- */

UINT GetCPUAccessFlagsByUsage(const D3D11_USAGE usage)
{
    return (usage >= D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
}


} // /namespace D3D11ParamMapper

} // /namespace Video

} // /namespace Fork



// ========================