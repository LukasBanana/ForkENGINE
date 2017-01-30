
// Render Target GLSL Vertex Shader
// 28/06/2014

#version 420

void main()
{
	vec4 pos;
	pos.x = ((gl_VertexID == 2) ?  3.0 : -1.0);
	pos.y = ((gl_VertexID != 0) ? -1.0 :  3.0);
	pos.zw = vec2(1.0);
	
	gl_Position = pos;
}
