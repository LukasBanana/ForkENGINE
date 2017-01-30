
// HLSL SM 5 test shader
// 21/07/2014

/* --- Vertex Shader --- */

cbuffer vertexParam : register(b0)
{
	float4 diffuseColor;
};

struct VertexOut
{
	float4 position	: SV_Position;
	float2 texCoord	: TEXCOORD0;
	float4 color	: COLOR;
};

VertexOut VertexMain(uint id : SV_VertexID)
{
	VertexOut outp;
	
	outp.position.x		= (id == 2 ?  3.0 : -1.0);
	outp.position.y		= (id == 0 ? -3.0 :  1.0);
	outp.position.zw	= (float2)1.0;
	
	outp.texCoord		= outp.position.xy * float2(0.5, -0.5) + 0.5;
	outp.color			= diffuseColor;

	return outp;
}

/* --- Pixel Shader --- */

float4 PixelMain(VertexOut inp) : SV_Target0
{
	return inp.color;
}

