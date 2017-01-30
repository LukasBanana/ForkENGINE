/*
 * Utils common model HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

#define MAX_NUM_MODELS 8

cbuffer vertexParam : register(b0)
{
	float4x4 projectionMatrix;
	float4x4 viewMatrix;
	float4 viewPosition;
	float4x4 worldMatrices[MAX_NUM_MODELS];
    float4 colors[MAX_NUM_MODELS];
    int instanceIndexOffset;
};

struct VertexIn
{
	float3 coord	: coord;
	float3 normal	: normal;
	uint instanceID	: SV_InstanceID;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
	float3 lightDir	: TEXCOORD;
};

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
	uint id = inp.instanceID + (uint)instanceIndexOffset;
	
	float4 worldPos = mul(worldMatrices[id], float4(inp.coord, 1.0));
	
	float4x4 vpMatrix = mul(projectionMatrix, viewMatrix);
    outp.position = mul(vpMatrix, worldPos);
	
	outp.normal = mul((float3x3)worldMatrices[id], inp.normal);
	outp.color = colors[id];
	outp.lightDir = viewPosition.xyz - worldPos.xyz;
	
	return outp;
}

/* ----- Pixel Shader --- */

#define LIGHT_MIN	0.35
#define LIGHT_RANGE	(1.0 - LIGHT_MIN)

float4 PixelMain(VertexOut inp) : SV_Target0
{
	float3 normal = normalize(inp.normal);
	
	float NdotL = LIGHT_MIN + max(0.0, dot(normal, normalize(inp.lightDir))) * LIGHT_RANGE;
	
	return inp.color * float4((float3)NdotL, 1.0);
}
