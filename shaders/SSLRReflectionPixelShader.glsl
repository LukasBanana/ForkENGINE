/*
 * SSLR Reflection GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 430

layout(origin_upper_left) in vec4 gl_FragCoord;

/* --- Macros (Internals) --- */

#define MAX_RAY_LENGTH 5.0

#define saturate(x) clamp(x, 0.0, 1.0)

/* --- Shader input --- */

layout(binding = 0) uniform sampler2D colorBuffer;
layout(binding = 1) uniform sampler2D specularBuffer;
layout(binding = 2) uniform sampler2D rayCastColorBuffer;	// Post-projected depth buffer (in screen space)
layout(binding = 3) uniform sampler2D rayCastMaskBuffer;	// Scene normal buffer (in screen space)

layout(std140) uniform pixelParam
{
	float globalReflectivity;
	float globalRoughness;
	float mipCount;
};

// Input texture coordinate from the vertex shader
in vec2 vertexTexCoord;

out vec4 fragColor;

/* --- Pixel Shader Main Function --- */

void main()
{
	/* Sample color- and specular buffers */
	vec4 color = textureLod(colorBuffer, vertexTexCoord, 0.0);
	//float specular = textureLod(specularBuffer, vertexTexCoord, 0.0).r;
	
	/* Fetch ray length factor */
	float rayLength = textureLod(rayCastColorBuffer, vertexTexCoord, 0.0).a * MAX_RAY_LENGTH;
	
	/* Determine surface roughness */
	float roughness = globalRoughness;//!TODO! -> sample specular buffer !!!
	float mipLod = (mipCount * 2.0) * roughness * rayLength;
	
	/* Apply reflection color */
	vec3 rayTraceColor = textureLod(rayCastColorBuffer, vertexTexCoord, mipLod).rgb;
	
	color.rgb += rayTraceColor * globalReflectivity;
	
	/* Set final pixel color */
	fragColor = color;
}
