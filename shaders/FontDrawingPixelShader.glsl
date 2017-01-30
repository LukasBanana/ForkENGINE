/*
 * Font drawing GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

uniform sampler2D tex;

in vec2 vertexTexCoord;
in vec4 vertexColor;

out vec4 fragColor;

void main()
{
	float col = texture2D(tex, vertexTexCoord).r;
	fragColor = vertexColor * vec4(col);
}
