/*
 * Util common model shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTIL_COMMON_MODEL_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_UTIL_COMMON_MODEL_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Vector4.h"
#include "Math/Core/Matrix4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "UtilCommonModel" shader constant buffer structures.
namespace UtilCommonModelShader
{

#include "Core/PackPush.h"


struct ConstBuffer
{
    static const size_t maxNumModels = 8;

    Math::Matrix4f      projectionMatrix;
    Math::Matrix4f      viewMatrix;
    Math::Point4f       viewPosition;
    Math::Matrix4f      worldMatrices   [maxNumModels];
    Video::ColorRGBAf   colors          [maxNumModels];
    int                 instanceIndexOffset = 0;
    int                 __pad0[3];
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace UtilCommonModelShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================