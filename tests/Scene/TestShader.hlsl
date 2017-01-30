
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
	
	return outp;
}

/* ------- Pixel Shader ------- */

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

cbuffer pixelParam : register(b0)
{
    float4 color;
	int4 flags;
};

float4 PixelMain(VertexOut inp) : SV_Target0
{
	float3 lightDir = float3(0, 0, 1);
	float NdotL = max(0.2, -dot(normalize(inp.normal), lightDir));
	
	if (flags.y == 0)
		NdotL = 1.0;
	
	float2 tc = inp.texCoord;
	float3 texSample = (float3)0.0;
	
	if (flags.x != 0)
		texSample = tex.Sample(samplerState, tc).rgb;
	else
		texSample = (float3)1.0;
	
    return color * float4((float3)NdotL * texSample, 1.0);
}
