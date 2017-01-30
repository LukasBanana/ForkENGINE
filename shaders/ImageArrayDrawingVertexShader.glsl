/*
 * Image array drawing GLSL vertex shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

#define MAX_NUM_IMAGES 1024//2048

struct ImageEntry
{
	vec4 positionSizeLayer;		// xy: Position, z: Size, w: Image Array Layer.
	vec4 color;					// rgba: Color.
};

layout(std140) uniform vertexParam
{
	mat4 wvpMatrix;
	vec4 basePositionAndSize;	// xy: Base Position, zw: Base Size.
	vec4 baseColor;
};

layout(std140) uniform imageArrayEntries
{
	ImageEntry entries[MAX_NUM_IMAGES];
};

out vec4 vertexColor;
out vec3 vertexTexCoord;

void main()
{
	int id = gl_InstanceID;
	
	/* Extract position, size and layer */
	ImageEntry entry = entries[id];
	
	vec4 posSizeLayer	= entry.positionSizeLayer;
	
	vec2 position		= basePositionAndSize.xy + posSizeLayer.xy;
	vec2 size			= basePositionAndSize.zw * vec2(posSizeLayer.z);
	float layer			= posSizeLayer.w;
	
	/* Compute vertex coordinate */
	vec4 coord = vec4(0.0);
	vec2 texCoord = vec2(0.0);
	
	switch (gl_VertexID)
	{
		case 0:
			coord		= vec4(position.x, position.y, 0.0, 1.0);
			texCoord	= vec2(0.0, 0.0);
			break;
		case 1:
			coord		= vec4(position.x + size.x, position.y, 0.0, 1.0);
			texCoord	= vec2(1.0, 0.0);
			break;
		case 2:
			coord		= vec4(position.x, position.y + size.y, 0.0, 1.0);
			texCoord	= vec2(0.0, 1.0);
			break;
		case 3:
			coord		= vec4(position.x + size.x, position.y + size.y, 0.0, 1.0);
			texCoord	= vec2(1.0, 1.0);
			break;
	}
	
    gl_Position = wvpMatrix * coord;
	
	/* Compute color and texture coordinate */
	vertexColor = baseColor * entry.color;
	
	vertexTexCoord = vec3(texCoord, layer);
}
