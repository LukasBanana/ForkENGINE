/*
 * Utils common model GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#define MAX_NUM_MODELS 8

layout(std140) uniform vertexParam
{
	mat4 projectionMatrix;
	mat4 viewMatrix;
	vec4 viewPosition;
	mat4 worldMatrices[MAX_NUM_MODELS];
    vec4 colors[MAX_NUM_MODELS];
    int instanceIndexOffset;
};

in vec3 coord;
in vec3 normal;

out vec3 vertexNormal;
out vec4 vertexColor;
out vec3 lightDir;

void main()
{
	int id = gl_InstanceID + instanceIndexOffset;
	
	vec4 worldPos = worldMatrices[id] * vec4(coord, 1.0);
	
	mat4 vpMatrix = projectionMatrix * viewMatrix;
    gl_Position = vpMatrix * worldPos;
	
	vertexNormal = mat3(worldMatrices[id]) * normal;
	vertexColor = colors[id];
	lightDir = viewPosition.xyz - worldPos.xyz;
}
