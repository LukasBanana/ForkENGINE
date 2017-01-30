/*
 * Font drawing HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

cbuffer vertexParam : register(b0)
{
    float4x4 wvpMatrix;
	float4 color;
};

struct VertexIn
{
	float2 coord	: coord;
	float2 texCoord	: texCoord;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float2 texCoord	: TEXCOORD0;
	float4 color	: COLOR0;
};

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
    outp.position	= mul(wvpMatrix, float4(inp.coord.xy, 0.0, 1.0));
	outp.texCoord	= inp.texCoord;
	outp.color		= color;
	
	return outp;
}

/* ----- Pixel Shader --- */

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float4 PixelMain(VertexOut inp) : SV_Target0
{
	float col = tex.Sample(samplerState, inp.texCoord).r;
	return inp.color * (float4)col;
}

