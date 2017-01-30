/*
 * Simple colored GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

layout(std140) uniform vertexParam
{
	mat4 wvpMatrix;
	vec4 baseColor;
};

in vec4 coord;
in vec4 color;

out vec4 vertexColor;

void main()
{
    gl_Position = wvpMatrix * coord;
	vertexColor = baseColor * color;
}
