/*
 * Tangent space mesh GLSL vertex shader
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
in vec3 tangent;
in vec3 bitangent;

out vec3 vertexTangent;
out vec3 vertexBitangent;
out vec3 vertexNormal;
out vec2 vertexTexCoord;
out vec3 vertexWorldPos;
out vec3 viewRay;

void main()
{
    gl_Position		= wvpMatrix * vec4(coord, 1.0);
	
	mat3 normalMatrix = mat3(worldMatrix);
	vertexTangent	= normalize(normalMatrix * tangent);
	vertexBitangent	= normalize(normalMatrix * bitangent);
	vertexNormal	= normalize(normalMatrix * normal);
	
	vertexTexCoord	= texCoord;
	vertexWorldPos	= (worldMatrix * vec4(coord, 1.0)).xyz;
	viewRay			= viewPosition - vertexWorldPos;
}
