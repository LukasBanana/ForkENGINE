/*
 * Simple 3D mesh GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

layout(std140) uniform vertexParam
{
	mat4 wvpMatrix;
	mat4 worldMatrix;
	vec3 viewPosition;
};

in vec3 coord;
in vec3 normal;
in vec2 texCoord;

out vec3 vertexNormal;
out vec2 vertexTexCoord;
out vec3 vertexWorldPos;
out vec3 viewRay;

void main()
{
    gl_Position		= wvpMatrix * vec4(coord, 1.0);
	vertexNormal	= normalize(mat3(worldMatrix) * normal);
	vertexTexCoord	= texCoord;
	vertexWorldPos	= (worldMatrix * vec4(coord, 1.0)).xyz;
	viewRay			= viewPosition - vertexWorldPos;
}
