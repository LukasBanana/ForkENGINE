/*
 * Common.Core HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#pragma once

/* --- Macros --- */

#define M_PI 3.141592654

/* --- Functions --- */

// Returns the square of x.
float Sq(float x)
{
	return x*x;
}

// Returns the squared length of vector 'v'.
float LengthSq(float2 v) { return dot(v, v); }
float LengthSq(float3 v) { return dot(v, v); }
float LengthSq(float4 v) { return dot(v, v); }

// Converts the color to a vector.
float3 ColorToVector(float3 color)
{
	return color*2.0 - 1.0;
}

// Converts the vector to a color.
float3 VectorToColor(float3 vector)
{
	return vector*0.5 + 0.5;
}

// Computes the angle of the two vectors a and b.
float VectorAngle(float3 a, float3 b)
{
	return acos(dot(a, b));
}

/*
Transforms the point from view-space into screen-space.
Projection matrix must be in left-handed coordinate system.
*/
float3 ProjectPoint(float4x4 projMatrix, float3 viewPoint)
{
	float4 projPoint = mul(projMatrix, float4(viewPoint, 1.0));
	projPoint /= projPoint.w;
	projPoint.xy = projPoint.xy * float2(0.5, -0.5) + float2(0.5, 0.5);
	return projPoint.xyz;
}

/*
Transform the point from screen-space into view-space.
Inverse-projection matrix must be in left-handed coordinate system.
*/
float3 UnprojectPoint(float4x4 invProjMatrix, float3 screenPoint)
{
	screenPoint.xy = (screenPoint.xy - float2(0.5, 0.5)) * float2(2.0, -2.0);
	float4 projPos = mul(invProjMatrix, float4(screenPoint, 1.0));
	projPos /= projPos.w;
	return projPos.xyz;
}
