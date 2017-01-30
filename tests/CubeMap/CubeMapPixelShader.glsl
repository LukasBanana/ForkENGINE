
// CubeMap GLSL Vertex Shader
// 23/08/2014

#version 330

layout(std140) uniform pixelParam
{
	mat4 invViewMatrix;
	vec4 viewPos;
};

uniform samplerCube tex;

in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec3 worldPos;

out vec4 fragColor;

void main()
{
	// Lighting
	vec3 lightDir = -normalize(vec3(1, -1, 3));
	vec3 normal = normalize(vertexNormal);
	
	float NdotL = dot(normal, lightDir);
	float intensity = max(0.2, NdotL);
	
	vec3 lightColor = vec3(1.0);
	
	vec3 shading = lightColor * intensity;
	
	// Texturing
	vec3 viewDir = normalize(worldPos - viewPos.xyz);
	vec3 reflected = reflect(viewDir.xyz, normal);
	
	reflected = (invViewMatrix * vec4(reflected, 0.0)).xyz;
	
	vec3 diffuse = texture(tex, reflected).rgb;
	
	// Final color
	fragColor = vec4(diffuse * shading, 1.0);
}
