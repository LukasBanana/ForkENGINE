/*
 * Drawing HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

#define MAX_NUM_VERTICES 32

cbuffer vertexParam : register(b0)
{
    float4x4 wvpMatrix;
	float4 coords[MAX_NUM_VERTICES];
	float4 colors[MAX_NUM_VERTICES];
	float4 texCoords[MAX_NUM_VERTICES];
};

struct VertexOut
{
	float4 position	: SV_Position;
	float4 color	: COLOR0;
	float4 texCoord	: TEXCOORD0;
};

VertexOut VertexMain(uint id : SV_VertexID)
{
	VertexOut outp;
	
    outp.position	= mul(wvpMatrix, coords[id]);
	outp.color		= colors[id];
	outp.texCoord	= texCoords[id];
	
	return outp;
}

/* ----- Pixel Shader --- */

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

cbuffer pixelParam : register(b0)
{
    int texEnabled;
};

float4 PixelMain(VertexOut inp) : SV_Target0
{
	float4 fragColor = inp.color;
	if (texEnabled != 0)
		fragColor *= tex.Sample(samplerState, inp.texCoord.xy);
	return fragColor;
}
