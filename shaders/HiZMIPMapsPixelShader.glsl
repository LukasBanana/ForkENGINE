/*
 * Hi-Z MIP-maps GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#define SAMPLE_MODE_MIN 0
#define SAMPLE_MODE_MAX 1

uniform sampler2D tex;

layout(std140) uniform configParam
{
	int sampleMode;
};

out vec4 fragColor;

void main()
{
	/* Fetch texels from current MIP LOD */
	ivec2 coord = ivec2(gl_FragCoord.xy) * ivec2(2);
	
	vec4 texels;
	
	texels.x = texelFetch(tex, coord, 0).r;
	texels.y = texelFetch(tex, coord + ivec2(1, 0), 0).r;
	texels.z = texelFetch(tex, coord + ivec2(0, 1), 0).r;
	texels.w = texelFetch(tex, coord + ivec2(1, 1), 0).r;
	
	/* Down-sample texels */
	float sample = 0.0;
	
	if (sampleMode == SAMPLE_MODE_MIN)
		sample = min(min(texels.x, texels.y), min(texels.z, texels.w));
	else
		sample = max(max(texels.x, texels.y), max(texels.z, texels.w));
	
	fragColor = vec4(vec3(sample), 1.0);
}
