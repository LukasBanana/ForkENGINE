/*
 * Bezier drawing GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

in vec4 vertexColor;

out vec4 fragColor;

void main()
{
	fragColor = vertexColor;
}
