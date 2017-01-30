
#version 330

in vec3 worldPos;
in vec3 cameraPos;

out vec4 depth;

void main()
{
	depth = vec4(vec3(distance(cameraPos, worldPos)), 1.0);
}
