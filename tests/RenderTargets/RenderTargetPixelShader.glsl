
// Render Target GLSL Vertex Shader
// 28/06/2014

#version 420

layout(location = 0) out vec4 diffuse1;
layout(location = 1) out vec4 diffuse2;

void main()
{
	diffuse1 = vec4(1.0, 0.0, 0.0, 1.0);
	diffuse2 = vec4(0.0, 1.0, 0.0, 1.0);
}
