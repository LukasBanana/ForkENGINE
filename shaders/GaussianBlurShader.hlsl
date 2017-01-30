/*
 * Guassian blur HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

struct VertexOut
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD0;
};

VertexOut VertexMain(uint vertexID : SV_VertexID)
{
	VertexOut outp;
	
	/* Setup vertex coordinate to fullscreen triangle */
	outp.position = float4(
		(vertexID == 2 ?  3.0 : -1.0),
		(vertexID == 0 ? -3.0 :  1.0),
		1.0,
		1.0
	);
	
	outp.texCoord = outp.position.xy * 0.5 + 0.5;
	
	return outp;
}

/* ----- Pixel Shader --- */

#define MAX_NUM_SAMPLES		64
#define RENDER_PASS_HORZ	0
#define RENDER_PASS_VERT	1

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

cbuffer configParam : register(b0)
{
	// x: Offset X, y: Offset Y, z: Weight, w: Unused.
	float4 offsetsAndWeights[MAX_NUM_SAMPLES];
	int numSamples;
};

cbuffer renderPassParam : register(b1)
{
	int renderPass;
};

float4 PixelMain(VertexOut inp) : SV_Target0
{
	float4 color = (float4)0.0;
	
	if (renderPass == RENDER_PASS_VERT)
	{
		/* Vertical render pass */
		for (int i = 0; i < numSamples; ++i)
		{
			color += tex.Sample(
				samplerState,
				inp.texCoord + float2(0.0, offsetsAndWeights[i].y)
			) * offsetsAndWeights[i].z;
		}
	}
	else
	{
		/* Horizontal render pass */
		for (int i = 0; i < numSamples; ++i)
		{
			color += tex.Sample(
				samplerState,
				inp.texCoord + float2(offsetsAndWeights[i].x, 0.0)
			) * offsetsAndWeights[i].z;
		}
	}
	
	return color;
}
