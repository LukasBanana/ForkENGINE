/*
 * Image array drawing GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

uniform sampler2DArray texArray;

in vec4 vertexColor;
in vec3 vertexTexCoord;

out vec4 fragColor;

void main()
{
	fragColor = vertexColor * texture(texArray, vertexTexCoord);
}
