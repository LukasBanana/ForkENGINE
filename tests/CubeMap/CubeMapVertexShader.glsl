
// CubeMap GLSL Vertex Shader
// 23/08/2014

#version 330

layout(std140) uniform vertexParam
{
	mat4 wvpMatrix;
	mat4 worldMatrix;
	vec3 viewPosition;
};

in vec3 coord;
in vec3 normal;
in vec2 texCoord;

out vec3 vertexNormal;
out vec2 vertexTexCoord;
out vec3 worldPos;

void main()
{
	gl_Position		= wvpMatrix * vec4(coord, 1.0);
	vertexNormal	= normalize(mat3(worldMatrix) * normal);
	vertexTexCoord	= texCoord;
	worldPos		= (worldMatrix * vec4(coord, 1.0)).xyz;
}
