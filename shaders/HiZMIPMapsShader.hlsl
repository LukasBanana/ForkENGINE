/*
 * Hi-Z MIP-maps HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

float4 VertexMain(uint vertexID : SV_VertexID) : SV_Position
{
	/* Setup vertex coordinate to fullscreen triangle */
	return float4(
		(vertexID == 2 ?  3.0 : -1.0),
		(vertexID == 0 ? -3.0 :  1.0),
		1.0,
		1.0
	);
}

/* ----- Pixel Shader --- */

#define SAMPLE_MODE_MIN 0
#define SAMPLE_MODE_MAX 1

Texture2D tex;

cbuffer configParam : register(b0)
{
	int sampleMode;
};

float4 PixelMain(float4 fragCoord : SV_Position) : SV_Target0
{
	/* Fetch texels from current MIP LOD */
	int3 coord = int3((int2)fragCoord.xy * 2, 0);
	
	float4 texels;
	
	texels.x = tex.Load(coord).r;
	texels.y = tex.Load(coord + int3(1, 0, 0)).r;
	texels.z = tex.Load(coord + int3(0, 1, 0)).r;
	texels.w = tex.Load(coord + int3(1, 1, 0)).r;
	
	/* Down-sample texels */
	float sample = 0.0;
	
	if (sampleMode == SAMPLE_MODE_MIN)
		sample = min(min(texels.x, texels.y), min(texels.z, texels.w));
	else
		sample = max(max(texels.x, texels.y), max(texels.z, texels.w));
	
	return float4((float3)sample, 1.0);
}
