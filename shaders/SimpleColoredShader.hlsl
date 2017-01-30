/*
 * Simple colored HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

cbuffer vertexParam : register(b0)
{
	float4x4 wvpMatrix;
	float4 baseColor;
};

struct VertexIn
{
	float4 coord : coord;
	float4 color : color;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float4 color	: COLOR;
};

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
    outp.position	= mul(wvpMatrix, inp.coord);
	outp.color		= baseColor * inp.color;
	
	return outp;
}

/* ----- Pixel Shader --- */

float4 PixelMain(VertexOut inp) : SV_Target0
{
	return inp.color;
}
