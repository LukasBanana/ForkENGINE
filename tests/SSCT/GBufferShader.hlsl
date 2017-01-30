
/* ------- Vertex Shader ------- */

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

cbuffer vertexParam : register(b0)
{
    float4x4 wvpMatrix;
	float4x4 worldMatrix;
};

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
	outp.position	= mul(wvpMatrix, float4(inp.coord, 1.0));
	outp.normal		= mul((float3x3)worldMatrix, inp.normal);
	outp.texCoord	= inp.texCoord;
	outp.worldPos	= mul(worldMatrix, float4(inp.coord, 1.0)).xyz;
	
	return outp;
}

/* ------- Pixel Shader ------- */

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

struct PixelOut
{
	float4 color	: SV_Target0;
	float4 normal	: SV_Target1;
	float4 depth	: SV_Target2;
};

cbuffer pixelParam : register(b0)
{
    float3 cameraPos;
};

PixelOut PixelMain(VertexOut inp)
{
	// Compute shading
	float3 normal = normalize(inp.normal);
	
	float3 lightDir = normalize(float3(0.3, -0.5, 1));
	float NdotL = max(0.2, -dot(normal, lightDir));
	
	float4 shading = float4((float3)NdotL, 1.0);
	
	// Get diffuse color
	float4 diffuse = (float4)1.0;
	
	//diffuse *= tex.Sample(samplerState, inp.texCoord);
	
	// Set output values
	PixelOut outp;
	
	float depth = distance(inp.worldPos, cameraPos);
	
    outp.color = diffuse * shading;
	outp.normal = float4(normal, 1.0);
	outp.depth = float4(depth, depth, 0.0, 1.0);
	
	return outp;
}
