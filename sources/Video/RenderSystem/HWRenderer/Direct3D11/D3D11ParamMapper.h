/*
 * Direct3D 11 parameter mapper header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_PARAM_MAPPER_H__
#define __FORK_D3D11_PARAM_MAPPER_H__


#include "Video/Monitor/ContextDescription.h"
#include "Video/RenderSystem/Texture/TextureAttributes.h"
#include "Video/RenderSystem/RenderState/GeometryPrimitives.h"
#include "Video/BufferFormat/RendererDataTypes.h"
#include "Video/BufferFormat/HardwareBufferUsage.h"
#include "Video/RenderSystem/RenderState/CompareFunctions.h"
#include "Video/RenderSystem/RenderState/BlendState.h"
#include "Video/RenderSystem/RenderState/RasterizerState.h"
#include "Video/RenderSystem/RenderState/DepthStencilState.h"
#include "Video/RenderSystem/RenderState/SamplerState.h"
#include "Video/RenderSystem/Query.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{

namespace D3D11ParamMapper
{


/* --- Mapping --- */

DXGI_FORMAT                 Map( const RendererDataTypes dataType, unsigned int numComponents, bool normalized );
D3D11_FILTER                Map( const SamplerState::Filters minFilter, const SamplerState::Filters magFilter, const SamplerState::Filters mipFilter );

DXGI_FORMAT                 Map( const TextureFormats                textureFormat );
D3D_FEATURE_LEVEL           Map( const D3DFeatureLevels              featureLevel  );
D3D11_PRIMITIVE_TOPOLOGY    Map( const GeometryPrimitives            primitiveType );
DXGI_FORMAT                 Map( const RendererDataTypes             dataType      );
D3D11_USAGE                 Map( const HardwareBufferUsage           usage         );
D3D11_MAP                   Map( const HardwareBufferCPUAccess       cpuAccess     );
D3D11_COMPARISON_FUNC       Map( const CompareFunctions              compareFunc   );
D3D11_BLEND                 Map( const BlendState::BlendOps          blendOp       );
D3D11_STENCIL_OP            Map( const DepthStencilState::StencilOps stencilOp     );
D3D11_FILL_MODE             Map( const RasterizerState::FillModes    fillMode      );
D3D11_CULL_MODE             Map( const RasterizerState::CullModes    cullMode      );
D3D11_TEXTURE_ADDRESS_MODE  Map( const SamplerState::WrapModes       wrapMode      );
D3D11_QUERY                 Map( const Query::Types                  queryType     );

/* --- Unmapping --- */

D3DFeatureLevels Unmap( const D3D_FEATURE_LEVEL featureLevel );

/* --- Extended --- */

UINT GetCPUAccessFlagsByUsage(const D3D11_USAGE usage);


} // /namespace D3D11ParamMapper

} // /namespace Video

} // /namespace Fork


#endif



// ========================