/*
 * Bezier drawing GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#define MAX_NUM_CURVES 32

layout(std140) uniform vertexParam
{
    mat4 wvpMatrix;
	vec4 ctrlPoints[MAX_NUM_CURVES*4];
	vec4 colors[MAX_NUM_CURVES*2];
	int segments;
};

out vec4 vertexColor;

vec4 InterpolateBezier(int id, float t)
{
	float inv_t		= 1.0 - t;
	float inv_t2	= inv_t*inv_t;
	float inv_t3	= inv_t2*inv_t;
	float t2		= t*t;
	float t3		= t2*t;
	
	return
		ctrlPoints[id    ] * inv_t3 +
		ctrlPoints[id + 1] * (3.0*t*inv_t2) +
		ctrlPoints[id + 2] * (3.0*t2*inv_t) +
		ctrlPoints[id + 3] * t3;
}

void main()
{
	/* Get instance ID and interpolation factor 't' */
	int id = gl_InstanceID;
	float t = float(gl_VertexID) / segments;
	
	/* Bezier curve interpolation */
    gl_Position = wvpMatrix * InterpolateBezier(id*4, t);
	
	/* Linear interpolation between two colors */
	int colorIndex = id*2;
	vertexColor = mix(colors[colorIndex], colors[colorIndex + 1], t);
}
