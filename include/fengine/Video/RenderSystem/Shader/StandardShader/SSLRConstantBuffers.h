/*
 * SSLR shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SSLR_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_SSLR_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Size2.h"
#include "Math/Core/Matrix4.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "SSLR" shader constant buffer structures.
namespace SSLRShader
{

#include "Core/PackPush.h"


struct RayTracePixelConstBuffer
{
    Math::Matrix4f  projectionMatrix;
    Math::Matrix4f  invProjectionMatrix;
    Math::Matrix4f  viewMatrix;
    float           nearPlane;
    float           globalReflectivity;
    float           globalRoughness;
    float           __pad0;
}
PACK_STRUCT;

struct ReflectionPixelConstBuffer
{
    float globalReflectivity;
    float globalRoughness;
    float mipCount;
    float __pad0;
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace SSCTShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================