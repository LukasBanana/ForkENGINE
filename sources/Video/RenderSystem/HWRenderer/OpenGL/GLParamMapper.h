/*
 * OpenGL parameter mapper header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_PARAM_MAPPER_H__
#define __FORK_GL_PARAM_MAPPER_H__


#include "GLImport.h"
#include "Video/BufferFormat/HardwareBufferUsage.h"
#include "Video/BufferFormat/RendererDataTypes.h"
#include "Video/RenderSystem/Shader/Shader.h"
#include "Video/RenderSystem/RenderState/GeometryPrimitives.h"
#include "Video/RenderSystem/Texture/TextureAttributes.h"
#include "Video/RenderSystem/Query.h"
#include "Video/Image/ImageAttributes.h"
#include "Video/RenderSystem/RenderState/CompareFunctions.h"
#include "Video/RenderSystem/RenderState/BlendState.h"
#include "Video/RenderSystem/RenderState/RasterizerState.h"
#include "Video/RenderSystem/RenderState/DepthStencilState.h"
#include "Video/RenderSystem/RenderState/SamplerState.h"
#include "Math/Common/AxisDirections.h"


namespace Fork
{

namespace Video
{

namespace GLParamMapper
{


GLenum Map( const SamplerState::Filters         minFilter,      const SamplerState::Filters mipFilter, bool useMIPMaps = true );

GLint  Map( const TextureFormats                textureFormat,  bool disableInteger = false );
GLenum Map( const ImageColorFormats             imageFormat,    bool isInteger      = false );
GLenum Map( const SamplerState::Filters         magFilter                                   );

GLenum Map( const HardwareBufferUsage           bufferUsage   );
GLenum Map( const HardwareBufferCPUAccess       cpuAccess     );
GLenum Map( const RendererDataTypes             dataType      );
GLenum Map( const Shader::Types                 shaderType    );
GLenum Map( const GeometryPrimitives            primitiveType );
GLenum Map( const CompareFunctions              compareFunc   );
GLenum Map( const BlendState::BlendOps          blendOp       );
GLenum Map( const DepthStencilState::StencilOps stencilOp     );
GLenum Map( const RasterizerState::FillModes    fillMode      );
GLenum Map( const RasterizerState::CullModes    cullMode      );
GLenum Map( const SamplerState::WrapModes       wrapMode      );
GLenum Map( const Math::AxisDirections          axisDir       );
GLenum Map( const Query::Types                  queryType     );


} // /namespace GLParamMapper

} // /namespace Video

} // /namespace Fork


#endif



// ========================