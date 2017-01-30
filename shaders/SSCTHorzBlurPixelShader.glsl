/*
 * SSCT Horizontal Blur GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#if 0
#define WEIGHT_0 0.002
#define WEIGHT_1 0.028
#define WEIGHT_2 0.233
#define WEIGHT_3 0.474
#else
#define WEIGHT_0 0.106
#define WEIGHT_1 0.140
#define WEIGHT_2 0.166
#define WEIGHT_3 0.176
#endif

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
	vec4 color = vec4(0.0);
	ivec2 coord = ivec2(gl_FragCoord.xy) * ivec2(2);
	
	color += texelFetch(tex, coord + ivec2( 3, 0), 0) * WEIGHT_0;
	color += texelFetch(tex, coord + ivec2( 2, 0), 0) * WEIGHT_1;
	color += texelFetch(tex, coord + ivec2( 1, 0), 0) * WEIGHT_2;
	color += texelFetch(tex, coord + ivec2( 0, 0), 0) * WEIGHT_3;
	color += texelFetch(tex, coord + ivec2(-1, 0), 0) * WEIGHT_2;
	color += texelFetch(tex, coord + ivec2(-2, 0), 0) * WEIGHT_1;
	color += texelFetch(tex, coord + ivec2(-3, 0), 0) * WEIGHT_0;
	
	fragColor = color;
	fragColor.a = 1.0;
}
