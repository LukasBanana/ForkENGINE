/*
 * Common.Shading HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#pragma once

#include "Common.Core.hlsl"

/* --- Macros --- */

#define LIGHT_TYPE_POINT		0
#define LIGHT_TYPE_SPOT			1
#define LIGHT_TYPE_DIRECTIONAL	2

/* --- Structures --- */

struct LightSource
{
	float4 positionAndType;	// Position (xyz), type (w).
	float4 colorAndEnergy;	// Color (rgb), energy (a).
	float4 direction;		// Direction (xyz) for directional- and spot lights.
};

/* --- Functions --- */

// Shades a pixel with all specified color components.
float4 ShadePixel(float4 baseColor, float3 lightColor, float3 ambient, float3 emissive)
{
	return float4(emissive + baseColor.rgb * (lightColor + ambient), baseColor.a);
}

// Schlick's approximation of the fresnel term.
float SchlickFresnel(float f0, float LoH)
{
	return (f0 + (1.0 - f0) * pow(1.0 - LoH, 5.0));
}

/*
Schlick GGX.
@see http://graphicrants.blogspot.co.uk/2013/08/specular-brdf-reference.html 
*/
float SchlickGGX(float alpha, float NoV)
{
	float k = alpha*0.5;
	return NoV/(NoV * (1.0 - k) + k);
}

/*
Microfacet models for refraction through rough surfaces (Walter et al. aka Towbridge-Reitz.).
@see http://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.html
*/
float MicrofacetNDF(in float alpha, in float NoH)
{
	float a2 = Sq(alpha);
	float cos2 = NoH*NoH;
	return (1.0/M_PI) * Sq(alpha/(cos2 * (a2 - 1.0) + 1.0));
}

/*
Computes the light attenuation.
@param lightPos Light position (in world space).
@param worldPos Pixel position (in world space).
@param lightEnergy Light source energy (in mega watt).
*/
float LightAttenuation(float3 lightPos, float3 worldPos, float lightEnergy)
{
	float dist = distance(lightPos, worldPos);
	
	float attn0 = 1.0;
	float attn1 = dist * (1.0 / lightEnergy);
	float attn2 = dist * attn1;
	
	return 1.0 / (attn0 + attn1 + attn2);
}

/*
Computes the light contribution.
@param light Light source.
@param worldPos Pixel position (in world space).
@param lightDir Resulting light direction vector.
@param intensity Resulting light intensity factor (in the range [0.0 .. 1.0]).
*/
void LightContribution(LightSource light, float3 worldPos, out float3 lightDir, out float intensity)
{
	/* Get light parameters */
	float energy = light.colorAndEnergy.a;
	float3 lightPos = light.positionAndType.xyz;
	int lightType = (int)(light.positionAndType.w);
	
	/* Compute light intensity for respective light type */
	intensity = 0.0;
	lightDir = (float3)0.0;
	
	switch (lightType)
	{
		case LIGHT_TYPE_POINT:
		{
			lightDir = lightPos - worldPos;
			intensity = LightAttenuation(lightPos, worldPos, energy);
		}
		break;
		
		case LIGHT_TYPE_SPOT:
		{
			lightDir = -light.direction.xyz;
			intensity = LightAttenuation(lightPos, worldPos, energy);
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
}

/*
Computes the blinn-phong BRDF.
@param normal Pixel normal vector (in world space).
@param viewDir View direction vector (in world space).
@param lightDir Light direction vector (in world space).
@param specularPower Specular highlight factor (also called "shininess"). Typical value is 90.0.
@param ambientColor Base ambient color.
@param diffuseColor Base diffuse color.
@param specularColor Base specular color.
*/
float3 BlinnPhongBRDF(
	float3 normal, float3 viewDir, float3 lightDir,
	float specularPower, float3 ambientColor, float3 diffuseColor, float3 specularColor)
{
	/* Diffuse term */
	float NdotL = dot(normal, lightDir);
	float diffuseTerm = saturate(NdotL);
	
	float3 diffuse = (diffuseColor - ambientColor) * diffuseTerm;
	
	/* Specular term */
	float3 halfDir = normalize(lightDir + viewDir);
	float NdotH = dot(normal, halfDir);
	float specularTerm = pow(saturate(NdotH), specularPower);
	
	float3 specular = specularColor * specularTerm;
	
	/* Final shading color */
	return diffuse + specular;
}

/*
Computes the cook-torrance BRDF.
@param normal Pixel normal vector (in world space).
@param viewDir View direction vector (in world space).
@param lightDir Light direction vector (in world space).
@param roughness Surface roughness factor (in the range [0.0 .. 1.0]).
@param diffuseColor Base diffuse color.
@param specularColor Base specular color.
*/
float3 CookTorranceBRDF(
	float3 normal, float3 viewDir, float3 lightDir,
	float roughness, float3 diffuseColor, float3 specularColor)
{
	/* Pre computations */
	float alpha = Sq(roughness);
	
	float3 halfDir = normalize(lightDir + viewDir);
	
	float NoL = dot(normal, lightDir);
	float NoV = dot(normal, viewDir);
	float NoH = dot(normal, halfDir);
	float LoH = dot(lightDir, halfDir);
	
	/* Refractive index */
	float n = 1.5; 
	float f0 = pow((1.0 - n)/(1.0 + n), 2.0);
	
	/* Fresnel term */
	float specularFresnel = SchlickFresnel(f0, LoH);
	
	/* Geometry term */
	float geometry = SchlickGGX(alpha, NoV);
	
	/* NDF term */
	float ndf = MicrofacetNDF(alpha, NoH);
	
	/* Specular term */
	float3 specular = specularColor/M_PI * (specularFresnel*geometry*ndf) / (4.0*NoL*NoV);
	
	/* Diffuse fresnel */
	float diffuseFresnel = SchlickFresnel(f0, NoL);
	
	float3 diffuse = diffuseColor/M_PI * (1.0 - diffuseFresnel);
	
	return diffuse + specular;
}
