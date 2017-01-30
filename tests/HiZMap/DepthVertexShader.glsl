
#version 330

in vec3 coord;

out vec3 worldPos;
out vec3 cameraPos;

layout(std140) uniform vertexParam
{
    mat4 wvpMatrix;
	mat4 worldMatrix;
	vec4 cameraPosition;
};

void main()
{
	vec4 pos = vec4(coord, 1.0);
    gl_Position = wvpMatrix * pos;
	worldPos = (worldMatrix * pos).xyz;
	cameraPos = cameraPosition.xyz;
}
