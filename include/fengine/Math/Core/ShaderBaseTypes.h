/*
 * Shader base types header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_CORE_SHADER_BASE_TYPES_H__
#define __FORK_MATH_CORE_SHADER_BASE_TYPES_H__


#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"


namespace Fork
{


/* --- Vector types --- */

typedef Math::Vector2b bool2;
typedef Math::Vector3b bool3;
typedef Math::Vector4b bool4;

typedef Math::Vector2i int2;
typedef Math::Vector3i int3;
typedef Math::Vector4i int4;

typedef Math::Vector2ui uint2;
typedef Math::Vector3ui uint3;
typedef Math::Vector4ui uint4;

typedef Math::Vector2f float2;
typedef Math::Vector3f float3;
typedef Math::Vector4f float4;

typedef Math::Vector2d double2;
typedef Math::Vector3d double3;
typedef Math::Vector4d double4;


/* --- Matrix types --- */

typedef Math::Matrix2b bool2x2;
typedef Math::Matrix3b bool3x3;
typedef Math::Matrix4b bool4x4;

typedef Math::Matrix2i int2x2;
typedef Math::Matrix3i int3x3;
typedef Math::Matrix4i int4x4;

typedef Math::Matrix2ui uint2x2;
typedef Math::Matrix3ui uint3x3;
typedef Math::Matrix4ui uint4x4;

typedef Math::Matrix2f float2x2;
typedef Math::Matrix3f float3x3;
typedef Math::Matrix4f float4x4;

typedef Math::Matrix2d double2x2;
typedef Math::Matrix3d double3x3;
typedef Math::Matrix4d double4x4;


} // /namespace Fork


#endif



// ========================