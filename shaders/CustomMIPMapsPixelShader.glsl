/*
 * Custom MIP-maps GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

uniform sampler2DArray tex;

layout(std140) uniform configParam
{
	int layer;
};

out vec4 fragColor;

void main()
{
	/* Fetch texels from current MIP LOD */
	ivec2 coord = ivec2(gl_FragCoord.xy) * ivec2(2);
	
	vec4 texel0 = texelFetch(tex, ivec3(coord, layer), 0);
	vec4 texel1 = texelFetch(tex, ivec3(coord + ivec2(1, 0), layer), 0);
	vec4 texel2 = texelFetch(tex, ivec3(coord + ivec2(0, 1), layer), 0);
	vec4 texel3 = texelFetch(tex, ivec3(coord + ivec2(1, 1), layer), 0);
	
	/* Down-sample texels */
	fragColor =
		texel0 * vec4(0.25) +
		texel1 * vec4(0.25) +
		texel2 * vec4(0.25) +
		texel3 * vec4(0.25);
}
