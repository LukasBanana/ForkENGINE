/*
 * Custom MIP-maps HLSL shader
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

Texture2DArray tex;

cbuffer configParam : register(b0)
{
	int layer;
};

float4 PixelMain(float4 fragCoord : SV_Position) : SV_Target0
{
	/* Fetch texels from current MIP LOD */
	int4 coord = int4((int2)fragCoord.xy * 2, layer, 0);
	
	float4 texel0 = tex.Load(coord);
	float4 texel1 = tex.Load(coord + int4(1, 0, 0, 0));
	float4 texel2 = tex.Load(coord + int4(0, 1, 0, 0));
	float4 texel3 = tex.Load(coord + int4(1, 1, 0, 0));
	
	/* Down-sample texels */
	return
		texel0 * 0.25 +
		texel1 * 0.25 +
		texel2 * 0.25 +
		texel3 * 0.25;
}
