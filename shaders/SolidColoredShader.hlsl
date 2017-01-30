/*
 * Solid colored HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

cbuffer param : register(b0)
{
	float4x4 wvpMatrix;
	float4 solidColor;
};

float4 VertexMain(float3 coord : coord) : SV_Position
{
    return mul(wvpMatrix, float4(coord, 1.0));
}

/* ----- Pixel Shader --- */

float4 PixelMain() : SV_Target0
{
	return solidColor;
}
