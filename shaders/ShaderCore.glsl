/*
 * GLSL shader core file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* === Shader information === */

#define GLSL 1


/* === Base math constants === */

#define M_PI	3.14159265359
#define M_E		2.71828182846


/* === Base data types === */

#define float2		vec2
#define float3		vec3
#define float4		vec4

#define double2		dvec2
#define double3		dvec3
#define double4		dvec4

#define bool2		bvec2
#define bool3		bvec3
#define bool4		bvec4

#define int2		ivec2
#define int3		ivec3
#define int4		ivec4

#define uint2		uvec2
#define uint3		uvec3
#define uint4		uvec4

#define half		float
#define half2		vec2
#define half3		vec3
#define half4		vec4

#define float2x2	mat2
#define float3x3	mat3
#define float4x4	mat4
#define float2x3	mat2x3
#define float2x4	mat2x4
#define float3x2	mat3x2
#define float3x4	mat3x4
#define float4x2	mat4x2
#define float4x3	mat4x3

#define double2x2	mat2
#define double3x3	mat3
#define double4x4	mat4
#define double2x3	mat2x3
#define double2x4	mat2x4
#define double3x2	mat3x2
#define double3x4	mat3x4
#define double4x2	mat4x2
#define double4x3	mat4x3


/* === Function extensions === */

#define Cast(t, v)				(t(v))

#define saturate(v)				clamp(v, 0.0, 1.0)
#define clip(v)					if ((v) < 0.0) { discard; }
#define frac(v)					fract(v)
#define lerp(x, y, s)			mix(x, y, s)

#define tex2D					texture2D
#define tex2Dgrad				texture2DGrad
#define tex2DArrayLod(s, t)		texture2DArrayLod(s, (t).xyz, (t).w)
#define tex2DArray(s, t)		texture2DArray(s, t)
#define tex2DGrad(s, t, dx, dy)	texture2DGrad(s, t, dx, dy)
#define ddx(v)					dFdx(v)
#define ddy(v)					dFdy(v)

#define Texture1D				sampler1D
#define Texture2D				sampler2D
#define Texture3D				sampler3D


/* === Texture and buffer operations === */

#define DeclStructuredBuffer(s, n, r)						\
	layout(std430, binding = r) readonly buffer Buffer##n	\
	{														\
		s n[];												\
	}

#define DeclBuffer(t, n, r)									\
	layout(std430, binding = r) readonly buffer Buffer##n	\
	{														\
		t n[];												\
	}

#define DeclRWStructuredBuffer(s, n, r)				\
	layout(std430, binding = r) buffer Buffer##n	\
	{												\
		s n[];										\
	}

#define DeclRWBuffer(t, n, r)						\
	layout(std430, binding = r) buffer Buffer##n	\
	{												\
		t n[];										\
	}

#define DeclConstBuffer(n, r)							layout(std140, binding = r) uniform n
#define RWTexture3DUInt									layout(r32ui) coherent volatile uimage3D

#define groupshared										shared

#define floatBitsToUInt(v)								floatBitsToUint(v)

#define InterlockedAdd(d, v, o)							o = atomicAdd(d, v)
#define InterlockedAnd(d, v, o)							o = atomicAnd(d, v)
#define InterlockedOr(d, v, o)							o = atomicOr(d, v)
#define InterlockedXor(d, v, o)							o = atomicXor(d, v)
#define InterlockedMin(d, v, o)							o = atomicMin(d, v)
#define InterlockedMax(d, v, o)							o = atomicMax(d, v)
#define InterlockedCompareExchange(d, v, o)				o = atomicCompSwap(d, v)
#define InterlockedExchange(d, v, o)					o = atomicExchange(d, v)

#define GroupMemoryBarrier								groupMemoryBarrier

#define InterlockedImageCompareExchange(i, p, c, v, o)	o = imageAtomicCompSwap(i, p, c, v)


/* === Extended functions === */

// --- "mul" function ---

#define _DEFINE_MUL_FUNC_(m, v)	\
	v mul(m mat, v vec)			\
	{							\
		return mat * vec;		\
	}							\
	v mul(v vec, m mat)			\
	{							\
		return vec * mat;		\
	}							\
	m mul(m a, m b)				\
	{							\
		return a * b;			\
	}

_DEFINE_MUL_FUNC_(float2x2, float2)
_DEFINE_MUL_FUNC_(float3x3, float3)
_DEFINE_MUL_FUNC_(float4x4, float4)

// --- "rcp" function ---

#undef _DEFINE_MUL_FUNC_

#define _DEFINE_RCP_FUNC_(t)									\
	t rcp(t x)													\
	{															\
		return 1.0 / x;											\
	}															\
	t##2 rcp(t##2 x)											\
	{															\
		return t##2(rcp(x.x), rcp(x.y));						\
	}															\
	t##3 rcp(t##3 x)											\
	{															\
		return t##3(rcp(x.x), rcp(x.y), rcp(x.z));				\
	}															\
	t##4 rcp(t##4 x)											\
	{															\
		return t##4(rcp(x.x), rcp(x.y), rcp(x.z), rcp(x.w));	\
	}

_DEFINE_RCP_FUNC_(float)
//_DEFINE_RCP_FUNC_(double)

#undef _DEFINE_RCP_FUNC_
