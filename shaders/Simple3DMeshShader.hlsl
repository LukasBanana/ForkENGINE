/*
 * Simple 3D mesh HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* ----- Vertex Shader --- */

cbuffer vertexParam
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
	float3 viewRay	: TEXCOORD2;
};

VertexOut VertexMain(VertexIn inp)
{
	VertexOut outp;
	
    outp.position	= mul(wvpMatrix, float4(inp.coord, 1.0));
	outp.normal		= normalize(mul((float3x3)worldMatrix, inp.normal));
	outp.texCoord	= inp.texCoord;
	outp.worldPos	= mul(worldMatrix, float4(inp.coord, 1.0)).xyz;
	outp.viewRay	= viewPosition - outp.worldPos;
	
	return outp;
}

/* ----- Pixel Shader --- */

#define MAX_NUM_LIGHTS			8

#define LIGHT_TYPE_POINT		0
#define LIGHT_TYPE_SPOT			1
#define LIGHT_TYPE_DIRECTIONAL	2

#define LIGHT_MIN				0.2
#define LIGHT_RANGE				(1.0 - LIGHT_MIN)

struct LightSource
{
	float4 positionAndType;	// Position (xyz), type (w).
	float4 colorAndEnergy;	// Color (rgb), energy (a).
	float4 direction;		// Direction (xyz) for directional- and spot lights.
};

cbuffer materialParam
{
	float4	ambientColor;
	float4	diffuseColor;
	float4	specularColor;
	float4	emissiveColor;
	float	shininess;
	int		numTextures;
};

cbuffer lightParam
{
	LightSource	lights[MAX_NUM_LIGHTS];
	int			numLights;
};

Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float LightAttenuation(float3 lightPos, float lightEnergy, float3 worldPos)
{
	float dist = distance(lightPos, worldPos);
	
	float attn0 = 1.0;
	float attn1 = dist * (1.0 / lightEnergy);
	float attn2 = dist * attn1;
	
	return 1.0 / (attn0 + attn1 + attn2);
}

void ComputeLighting(LightSource light, float3 normal, float3 viewDir, float3 worldPos, inout float3 diffuse, inout float3 specular)
{
	/* Get light parameters */
	float3 color = light.colorAndEnergy.rgb;
	float energy = light.colorAndEnergy.a;
	
	float3 position = light.positionAndType.xyz;
	int type = (int)(light.positionAndType.w);
	
	/* Compute light intensity for respective light type */
	float intensity = 0.0;
	float3 lightDir = 0.0;
	
	switch (type)
	{
		case LIGHT_TYPE_POINT:
		{
			lightDir = position - worldPos;
			intensity = LightAttenuation(position, energy, worldPos);
		}
		break;
		
		case LIGHT_TYPE_SPOT:
		{
			lightDir = -light.direction.xyz;
			intensity = LightAttenuation(position, energy, worldPos);
		}
		break;
		
		case LIGHT_TYPE_DIRECTIONAL:
		{
			lightDir = -light.direction.xyz;
			intensity = 1.0;
		}
		break;
	}
	
	lightDir = normalize(lightDir);
	
	/* Compute diffuse term */
	float NdotL = dot(normal, lightDir);
	float diffuseTerm = LIGHT_MIN + saturate(NdotL) * LIGHT_RANGE;
	diffuse += color * (intensity * diffuseTerm);
	
	/* Compute specular term */
	float3 halfDir = normalize(lightDir + viewDir);
	float NdotH = dot(normal, halfDir);
	float specularTerm = pow(saturate(NdotH), shininess);
	specular += color * (intensity * specularTerm);
}

float4 PixelMain(VertexOut inp) : SV_Target0
{
	/* Sample texture */
	float4 color = diffuseColor;
	
	if (numTextures > 0)
		color *= tex.Sample(samplerState, inp.texCoord);
	
	/* Compute light color */
	float3 diffuse = (float3)0.0;
	float3 specular = (float3)0.0;
	float3 normal = normalize(inp.normal);
	float3 viewDir = normalize(inp.viewRay);
	
	for (int i = 0; i < numLights; ++i)
		ComputeLighting(lights[i], normal, viewDir, inp.worldPos, diffuse, specular);
	
	/* Setup final fragment color */
	return float4(color.rgb * diffuse + specularColor.rgb * specular, color.a);
	//return float4(color.rgb * saturate(ambient + diffuse) + specular + emissive, color.a);
}
