/*
 * Tangent space mesh GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330
#extension GL_ARB_shading_language_420pack : enable

#include "Common.Shading.glsl"

#define MAX_NUM_LIGHTS 8

layout(std140) uniform materialParam
{
	vec4	ambientColor;
	vec4	diffuseColor;
	vec4	specularColor;
	vec4	emissiveColor;
	float	shininess;
	int		numTextures;
};

layout(std140) uniform lightParam
{
	LightSource	lights[MAX_NUM_LIGHTS];
	int			numLights;
};

layout(binding = 0) uniform sampler2D colorMap;
layout(binding = 1) uniform sampler2D normalMap;

in vec3 vertexTangent;
in vec3 vertexBitangent;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec3 vertexWorldPos;
in vec3 viewRay;

out vec4 fragColor;

void main()
{
	/* Sample texture */
	vec4 baseColor = diffuseColor;
	if (numTextures >= 1)
		baseColor *= texture2D(colorMap, vertexTexCoord);
	
	/* Sample normal vector from texture space */
	vec3 normal = vec3(0.0, 0.0, 1.0);
	
	if (numTextures >= 2)
	{
		normal = normalize(ColorToVector(texture2D(normalMap, vertexTexCoord).rgb));
		
		/* Transform normal into tangent space */
		mat3 tangentSpace = mat3(
			normalize(vertexTangent),
			normalize(vertexBitangent),
			normalize(vertexNormal)
		);
		
		normal = tangentSpace * normal;
	}
	else
		normal = normalize(vertexNormal);
	
	/* Compute light color */
	vec3 viewDir		= normalize(viewRay);
	vec3 shadingColor	= vec3(0.0);
	
	for (int i = 0; i < numLights; ++i)
	{
		/* Light contribution */
		vec3 lightColor = lights[i].colorAndEnergy.rgb;
		vec3 lightDir;
		float intensity;
		
		LightContribution(lights[i], vertexWorldPos, lightDir, intensity);
		
		/* Compute BRDF */
		shadingColor += BlinnPhongBRDF(
			normal, viewDir, lightDir, shininess,
			ambientColor.rgb, lightColor, specularColor.rgb
		) * intensity;
	}
	
	/* Setup final fragment color */
	fragColor = ShadePixel(baseColor, shadingColor, ambientColor.rgb, emissiveColor.rgb);
}
