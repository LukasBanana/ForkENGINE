/*
 * SSCT Visibility Map GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 420

layout(binding = 0) uniform sampler2D visibilityMap;
layout(binding = 1) uniform sampler2D depthMap; // post-projected depth buffer (in screen space)

layout(std140) uniform pixelParam
{
	int mipLevel;
	float nearPlane;
	float farPlane;
};

out vec4 coarseIntegration;

// Linearize z (result is in range [0 .. 1] just like the post-projected z value).
float Linearize(float z)
{
	return (2.0 * nearPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

float ReadDepth(ivec2 coord, int level)
{
	return Linearize(texelFetch(depthMap, coord, level).r);
}

void main()
{
	/* Fetch texels from previous depth map LOD */
	ivec2 coordCoarse	= ivec2(gl_FragCoord.xy);
	ivec2 coordFine		= coordCoarse * ivec2(2);
	
	int mipPrevious = mipLevel - 1;
	vec4 fineZ;
	
	fineZ.x = ReadDepth(coordFine, mipPrevious);
	fineZ.y = ReadDepth(coordFine + ivec2(1, 0), mipPrevious);
	fineZ.z = ReadDepth(coordFine + ivec2(0, 1), mipPrevious);
	fineZ.w = ReadDepth(coordFine + ivec2(1, 1), mipPrevious);
	
	/* Fetch fine visibility from previous visibility map LOD */
	vec4 visibility;
	
	visibility.x = texelFetch(visibilityMap, coordFine, 0).r;
	visibility.y = texelFetch(visibilityMap, coordFine + ivec2(1, 0), 0).r;
	visibility.z = texelFetch(visibilityMap, coordFine + ivec2(0, 1), 0).r;
	visibility.w = texelFetch(visibilityMap, coordFine + ivec2(1, 1), 0).r;
	
	/* Integrate visibility */
	float maxZ = max(max(fineZ.x, fineZ.y), max(fineZ.z, fineZ.w));
	vec4 integration = (fineZ / maxZ) * visibility;
	
	/* Compute coarse visibility (with SIMD 'dot' intrinsic) */
	float coarseVisibility = dot(vec4(0.25), integration);
	
	coarseIntegration = vec4(coarseVisibility, 0.0, 0.0, 1.0);
}
