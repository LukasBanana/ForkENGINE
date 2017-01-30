/*
 * Bezier drawing HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

#define MAX_NUM_CURVES 32

cbuffer vertexParam : register(b0)
{
    float4x4 wvpMatrix;
	float4 ctrlPoints[MAX_NUM_CURVES*4];
	float4 colors[MAX_NUM_CURVES*2];
	int segments;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float4 color 	: COLOR0;
};

float4 InterpolateBezier(uint id, float t)
{
	float inv_t		= 1.0 - t;
	float inv_t2	= inv_t*inv_t;
	float inv_t3	= inv_t2*inv_t;
	float t2		= t*t;
	float t3		= t2*t;
	
	return
		ctrlPoints[id    ] * inv_t3 +
		ctrlPoints[id + 1] * (3.0*t*inv_t2) +
		ctrlPoints[id + 2] * (3.0*t2*inv_t) +
		ctrlPoints[id + 3] * t3;
}

VertexOut VertexMain(uint instanceID : SV_InstanceID, uint vertexID : SV_VertexID)
{
	VertexOut outp;
	
	/* Get instance ID and interpolation factor 't' */
	uint id = instanceID;
	float t = float(vertexID) / segments;
	
	/* Bezier curve interpolation */
    outp.position = mul(wvpMatrix, InterpolateBezier(id*4, t));
	
	/* Linear interpolation between two colors */
	uint colorIndex = id*2;
	outp.color = lerp(colors[colorIndex], colors[colorIndex + 1], t);
	
	return outp;
}

/* ----- Pixel Shader --- */

float4 PixelMain(VertexOut inp) : SV_Target0
{
	return inp.color;
}

