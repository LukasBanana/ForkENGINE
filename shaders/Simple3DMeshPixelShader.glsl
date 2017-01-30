/*
 * Simple 3D mesh GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 330

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

uniform sampler2D colorMap;

in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec3 vertexWorldPos;
in vec3 viewRay;

out vec4 fragColor;

void main()
{
	/* Sample texture */
	vec4 baseColor = diffuseColor;
	if (numTextures > 0)
		baseColor *= texture2D(colorMap, vertexTexCoord);
	
	/* Compute light color */
	vec3 normal			= normalize(vertexNormal);
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
		/*shadingColor += CookTorranceBRDF(
			normal, viewDir, lightDir, 0.3,//shininess,
			lightColor, specularColor.rgb
		) * intensity * 2.0;*/
	}
	
	/* Setup final fragment color */
	fragColor = ShadePixel(baseColor, shadingColor, ambientColor.rgb, emissiveColor.rgb);
}
