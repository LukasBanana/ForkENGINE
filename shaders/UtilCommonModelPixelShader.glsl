/*
 * Utils common model GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#define LIGHT_MIN	0.35
#define LIGHT_RANGE	(1.0 - LIGHT_MIN)

in vec3 vertexNormal;
in vec4 vertexColor;
in vec3 lightDir;

out vec4 fragColor;

void main()
{
	vec3 normal = normalize(vertexNormal);
	
	float NdotL = LIGHT_MIN + max(0.0, dot(normal, normalize(lightDir))) * LIGHT_RANGE;
	
	fragColor = vertexColor * vec4(vec3(NdotL), 1.0);
}
