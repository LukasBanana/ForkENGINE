/*
 * Custom MIP-maps GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

void main()
{
	/* Setup vertex coordinate to fullscreen triangle */
	gl_Position = vec4(
		(gl_VertexID == 2 ? 3.0 : -1.0),
		(gl_VertexID == 0 ? 3.0 : -1.0),
		-1.0,
		1.0
	);
}
