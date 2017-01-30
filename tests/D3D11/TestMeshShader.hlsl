
// HLSL SM 5 test mesh shader
// 01/08/2014

/* --- Vertex Shader --- */

cbuffer vertexParam : register(b0)
{
	float4x4 wvpMatrix;
	float4x4 worldMatrix;
	float4 diffuseColor;
};

struct VertexIn
{
	float3 coord	: coord;
	float3 normal 	: normal;
	float2 texCoord	: texCoord;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float3 normal	: NORMAL;
	float2 texCoord	: TEXCOORD;
	float4 color	: COLOR;
};

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
	// Basic vertex transformation
	outp.position	= mul(wvpMatrix, float4(inp.coord, 1.0));
	outp.normal		= normalize(mul((float3x3)worldMatrix, inp.normal));
	outp.texCoord	= inp.texCoord;
	outp.color		= diffuseColor;

	return outp;
}

/* --- Pixel Shader --- */

Texture2D diffuseMap : register(t0);
SamplerState samplerState : register(s0);

float4 PixelMain(VertexOut inp) : SV_Target0
{
	float4 color = inp.color;
	
	// Texturing
	color *= diffuseMap.Sample(samplerState, inp.texCoord);
	
	// Lighting
	float3 normal = normalize(inp.normal);
	float3 lightVec = float3(0, 0, -1);
	
	float lambertFactor = dot(normal, lightVec);
	
	color.xyz *= lambertFactor;
	
	return color;
}

