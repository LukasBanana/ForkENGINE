/*
 * Drawing GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#define MAX_NUM_VERTICES 32

layout(std140) uniform vertexParam
{
    mat4 wvpMatrix;
	vec4 coords[MAX_NUM_VERTICES];
	vec4 colors[MAX_NUM_VERTICES];
	vec4 texCoords[MAX_NUM_VERTICES];
};

out vec4 vertexColor;
out vec4 vertexTexCoord;

void main()
{
    gl_Position		= wvpMatrix * coords[gl_VertexID];
	vertexColor		= colors[gl_VertexID];
	vertexTexCoord	= texCoords[gl_VertexID];
}
