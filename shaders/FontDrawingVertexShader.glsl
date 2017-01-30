/*
 * Font drawing GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

layout(std140) uniform vertexParam
{
    mat4 wvpMatrix;
	vec4 color;
};

in vec2 coord;
in vec2 texCoord;

out vec2 vertexTexCoord;
out vec4 vertexColor;

void main()
{
    gl_Position = wvpMatrix * vec4(coord.xy, 0.0, 1.0);
	vertexTexCoord = texCoord;
	vertexColor = color;
}
