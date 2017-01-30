/*
 * SSCT shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SSCT_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_SSCT_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Size2.h"
#include "Math/Core/Matrix4.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "SSCT" shader constant buffer structures.
namespace SSCTShader
{

#include "Core/PackPush.h"


struct VisibilityMapPixelConstBuffer
{
    int     mipLevel;
    float   nearPlane;
    float   farPlane;
    int     __pad0;
}
PACK_STRUCT;

struct HiZMapPixelConstBuffer
{
    Math::Vector2i  offset;
    int             __pad0[2];
}
PACK_STRUCT;

struct ReflectionPixelConstBuffer
{
    Math::Matrix4f  projectionMatrix;
    Math::Matrix4f  invProjectionMatrix;
    Math::Matrix4f  viewMatrix;
    int             mipCount;
    int             __pad0;
    Math::Size2f    resolution;
    float           nearPlane;
    float           farPlane;
    float           globalReflectivity;
    float           globalRoughness;
    #ifdef _DEBUG_SSCT_//!!!
    Math::Vector4f  _deb_mousepos;
    Math::Vector4i  _deb_options;
    #endif
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace SSCTShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================