/*
 * Guassian blur pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#define MAX_NUM_SAMPLES		64
#define RENDER_PASS_HORZ	0
#define RENDER_PASS_VERT	1

uniform sampler2D tex;

layout(std140) uniform configParam
{
	// x: Offset X, y: Offset Y, z: Weight, w: Unused.
	vec4 offsetsAndWeights[MAX_NUM_SAMPLES];
	int numSamples;
};

layout(std140) uniform renderPassParam
{
	int renderPass;
};

in vec2 vertexTexCoord;

out vec4 fragColor;

void main()
{
	vec4 color = vec4(0.0);
	
	if (renderPass == RENDER_PASS_VERT)
	{
		/* Vertical render pass */
		for (int i = 0; i < numSamples; ++i)
		{
			color += texture2D(
				tex, vertexTexCoord + vec2(0.0, offsetsAndWeights[i].y)
			) * vec4(offsetsAndWeights[i].z);
		}
	}
	else
	{
		/* Horizontal render pass */
		for (int i = 0; i < numSamples; ++i)
		{
			color += texture2D(
				tex, vertexTexCoord + vec2(offsetsAndWeights[i].x, 0.0)
			) * vec4(offsetsAndWeights[i].z);
		}
	}
	
	fragColor = color;
}
