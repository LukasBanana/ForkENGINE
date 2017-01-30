/*
 * HLSL (SM 5) shader core file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* === Shader information === */

#define HLSL 1


/* === Base math constants === */

#define M_PI	3.14159265359
#define M_E		2.71828182846


/* === Function extensions === */

#define Cast(t, v)				((t)(v))

#define mod(a, b)				fmod(a, b)

#define floatBitsToInt(v)		asint(v)
#define floatBitsToUInt(v)		asuint(v)
#define intBitsToFloat(v)		asfloat(v)
#define uintBitsToFloat(v)		asfloat(v)


/* === Texture and buffer operations === */

#define DeclStructuredBuffer(s, n, r)	StructuredBuffer<s> n : register(t##r)
#define DeclBuffer(t, n, r)				Buffer<t> n : register(t##r)

#define DeclRWStructuredBuffer(s, n, r)	RWStructuredBuffer<s> n : register(u##r)
#define DeclRWBuffer(t, n, r)			RWBuffer<t> n : register(u##r)

#define DeclConstBuffer(n, r)			cbuffer n : register(b##r)


/* === Extended functions === */

inline void InterlockedImageCompareExchange(RWTexture3DUInt Image, int3 Coord, uint Compare, uint Value, out uint Result)
{
	InterlockedCompareExchange(Image[Coord], Compare, Value, Result);
}
