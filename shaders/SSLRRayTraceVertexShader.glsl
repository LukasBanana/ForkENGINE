/*
 * SSLR Ray-Cast GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 420

out vec2 vertexTexCoord;

void main()
{
	/* Setup vertex coordinate for fullscreen triangle programmatically */
	vec4 coord;
	
	coord.x = (gl_VertexID == 2 ? 3.0 : -1.0);
	coord.y = (gl_VertexID == 0 ? 3.0 : -1.0);
	coord.z = -1.0;
	coord.w = 1.0;
	
	gl_Position = coord;
	
	/* Setup vertex texture coordinate from vertex coordinate */
	vertexTexCoord = coord.xy * vec2(0.5) + vec2(0.5);
}
