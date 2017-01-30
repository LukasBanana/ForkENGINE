/*
 * Drawing GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

uniform sampler2D tex;

layout(std140) uniform pixelParam
{
    int texEnabled;
};

in vec4 vertexColor;
in vec4 vertexTexCoord;

out vec4 fragColor;

void main()
{
	fragColor = vertexColor;
	if (texEnabled != 0)
		fragColor *= texture2D(tex, vertexTexCoord.xy);
}
