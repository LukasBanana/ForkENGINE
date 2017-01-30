/*
 * Bounding box HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

Texture2D worldMatrices;

cbuffer vertexParam : register(b0)
{
	float4x4 vpMatrix;
	float4 color;
	int4 texSize;
};

struct VertexIn
{
	float3 coord	: coord;
	uint instanceID	: SV_InstanceID;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float4 color	: COLOR;
};

int3 ImageCoord(uint id, uint column)
{
	uint index = id*4 + column;
	
	return int3(
		(int)(index % texSize.x),
		(int)(index / texSize.y),
		0
	);
}

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
	/* Sample world matrix from texture atlas */
	uint id = inp.instanceID;
	
	float4x4 worldMatrix;
	
	worldMatrix[0] = worldMatrices.Load(ImageCoord(id, 0));
	worldMatrix[1] = worldMatrices.Load(ImageCoord(id, 1));
	worldMatrix[2] = worldMatrices.Load(ImageCoord(id, 2));
	worldMatrix[3] = worldMatrices.Load(ImageCoord(id, 3));
	
	/* Transform vertex */
	float4 worldPos = mul(float4(inp.coord, 1.0), worldMatrix);
	
    outp.position = mul(vpMatrix, worldPos);
	outp.color = color;
	
	return outp;
}

/* ----- Pixel Shader --- */

float4 PixelMain(VertexOut inp) : SV_Target0
{
	return inp.color;
}
