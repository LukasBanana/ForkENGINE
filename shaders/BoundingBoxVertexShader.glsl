/*
 * Bounding box GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

uniform sampler2D worldMatrices;

layout(std140) uniform vertexParam
{
	mat4 vpMatrix;
	vec4 color;
	ivec4 texSize;
};

in vec3 coord;

out vec4 vertexColor;

ivec2 ImageCoord(int id, int column)
{
	int index = id*4 + column;
	
	return ivec2(
		index % texSize.x,
		index / texSize.y
	);
}

void main()
{
	/* Sample world matrix from texture atlas */
	int id = gl_InstanceID;
	
	mat4 worldMatrix;
	
	worldMatrix[0] = texelFetch(worldMatrices, ImageCoord(id, 0), 0);
	worldMatrix[1] = texelFetch(worldMatrices, ImageCoord(id, 1), 0);
	worldMatrix[2] = texelFetch(worldMatrices, ImageCoord(id, 2), 0);
	worldMatrix[3] = texelFetch(worldMatrices, ImageCoord(id, 3), 0);
	
	/* Transform vertex */
	vec4 worldPos = worldMatrix * vec4(coord, 1.0);
	
    gl_Position = vpMatrix * worldPos;
	
	vertexColor = color;
}
