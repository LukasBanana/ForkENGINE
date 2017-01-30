/*
 * Solid colored GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

layout(std140) uniform param
{
	mat4 wvpMatrix;
	vec4 solidColor;
};

in vec3 coord;

void main()
{
	gl_Position = wvpMatrix * vec4(coord, 1.0);
}
