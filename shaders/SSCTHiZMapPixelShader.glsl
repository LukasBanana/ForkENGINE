/*
 * SSCT Hi-Z Map GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

uniform sampler2D tex;

layout(std140) uniform pixelParam
{
	ivec2 offset;
};

out vec4 fragColor;

void main()
{
	/* Fetch texels from current MIP LOD */
	ivec2 coord = ivec2(gl_FragCoord.xy) * ivec2(2);
	
	vec4 texels[2];
	
	texels[0].rg = texelFetch(tex, coord                            , 0).rg;
	texels[0].ba = texelFetch(tex, coord + ivec2(offset.x,        0), 0).rg;
	texels[1].rg = texelFetch(tex, coord + ivec2(       0, offset.y), 0).rg;
	texels[1].ba = texelFetch(tex, coord + ivec2(offset.x, offset.y), 0).rg;
	
	/* Down-sample texels */
	float minZ = min(min(texels[0].r, texels[0].b), min(texels[1].r, texels[1].b));
	float maxZ = max(max(texels[0].g, texels[0].a), max(texels[1].g, texels[1].a));
	
	fragColor = vec4(minZ, maxZ, 0.0, 1.0);
}
