
// CubeMap HLSL Shader
// 23/08/2014

/* ------- Vertex Shader ------- */

cbuffer vertexParam : register(b0)
{
	float4x4 wvpMatrix;
	float4x4 worldMatrix;
	float3 viewPosition;
};

struct VertexIn
{
	float3 coord	: coord;
	float3 normal	: normal;
	float2 texCoord	: texCoord;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float3 normal	: NORMAL;
	float2 texCoord	: TEXCOORD0;
	float3 worldPos	: TEXCOORD1;
};

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
	outp.position	= mul(wvpMatrix, float4(inp.coord, 1.0));
	outp.normal		= normalize(mul((float3x3)worldMatrix, inp.normal));
	outp.texCoord	= inp.texCoord;
	outp.worldPos	= mul(worldMatrix, float4(inp.coord, 1.0)).xyz;
	
	return outp;
}

/* ------- Pixel Shader ------- */

cbuffer pixelParam : register(b0)
{
	float4x4 invViewMatrix;
	float4 viewPos;
};

TextureCube tex : register(t0);
SamplerState samplerState : register(s0);

float4 PixelMain(VertexOut inp) : SV_Target0
{
	// Lighting
	float3 lightDir = -normalize(float3(1, -1, 3));
	float3 normal = normalize(inp.normal);
	
	float NdotL = dot(normal, lightDir);
	float intensity = max(0.2, NdotL);
	
	float3 lightColor = 1.0;
	
	float3 shading = lightColor * intensity;
	
	// Texturing
	float3 viewDir = normalize(inp.worldPos - viewPos.xyz);
	float3 reflected = reflect(viewDir.xyz, normal);
	
	reflected = mul(invViewMatrix, float4(reflected, 0.0)).xyz;
	
	float3 diffuse = tex.Sample(samplerState, reflected).rgb;
	
	// Final color
	return float4(diffuse * shading, 1.0);
}

