/*
 * Image array drawing HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

#define MAX_NUM_IMAGES 1024

struct ImageEntry
{
	float4 positionSizeLayer;		// xy: Position, z: Size, w: Image Array Layer.
	float4 color;					// rgba: Color.
};

cbuffer vertexParam : register(b0)
{
	float4x4 wvpMatrix;
	float4 basePositionAndSize;	// xy: Base Position, zw: Base Size.
	float4 baseColor;
};

cbuffer imageArrayEntries : register(b1)
{
	ImageEntry entries[MAX_NUM_IMAGES];
};

struct VertexOut
{
	float4 position	: SV_Position;
	float4 color	: COLOR0;
	float3 texCoord	: TEXCOORD0;
};

VertexOut VertexMain(uint instanceID : SV_InstanceID, uint vertexID : SV_VertexID)
{
	VertexOut outp;
	
	/* Extract position, size and layer */
	ImageEntry entry = entries[instanceID];
	
	float4 posSizeLayer	= entry.positionSizeLayer;
	
	float2 position		= basePositionAndSize.xy + posSizeLayer.xy;
	float2 size			= basePositionAndSize.zw * (float2)posSizeLayer.z;
	float layer			= posSizeLayer.w;
	
	/* Compute vertex coordinate */
	float4 coord = (float4)0.0;
	float2 texCoord = (float2)0.0;
	
	switch (vertexID)
	{
		case 0:
			coord		= float4(position.x, position.y, 0.0, 1.0);
			texCoord	= float2(0.0, 0.0);
			break;
		case 1:
			coord		= float4(position.x + size.x, position.y, 0.0, 1.0);
			texCoord	= float2(1.0, 0.0);
			break;
		case 2:
			coord		= float4(position.x, position.y + size.y, 0.0, 1.0);
			texCoord	= float2(0.0, 1.0);
			break;
		case 3:
			coord		= float4(position.x + size.x, position.y + size.y, 0.0, 1.0);
			texCoord	= float2(1.0, 1.0);
			break;
	}
	
    outp.position = mul(wvpMatrix, coord);
	
	/* Compute color and texture coordinate */
	outp.color = baseColor * entry.color;
	
	outp.texCoord = float3(texCoord, layer);
	
	return outp;
}

/* ----- Pixel Shader --- */

Texture2DArray texArray : register(t0);
SamplerState samplerState : register(s0);

float4 PixelMain(VertexOut inp) : SV_Target0
{
	return inp.color * texArray.Sample(samplerState, inp.texCoord);
}
