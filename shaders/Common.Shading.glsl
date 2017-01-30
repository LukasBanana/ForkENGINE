// GLSL Shader
// Generated from HLSL Shader
// Mon Nov 24 22:34:13 2014


#pragma once
#include "Common.Core.glsl"
#define LIGHT_TYPE_POINT		0
#define LIGHT_TYPE_SPOT			1
#define LIGHT_TYPE_DIRECTIONAL	2
struct LightSource
{
    vec4 positionAndType;
    vec4 colorAndEnergy;
    vec4 direction;
};

vec4 ShadePixel(vec4 baseColor, vec3 lightColor, vec3 ambient, vec3 emissive)
{
    return vec4(emissive + baseColor.rgb * (lightColor + ambient), baseColor.a);
}

float SchlickFresnel(float f0, float LoH)
{
    return (f0 + (1.0 - f0) * pow(1.0 - LoH, 5.0));
}

float SchlickGGX(float alpha, float NoV)
{
    float k = alpha * 0.5;
    return NoV / (NoV * (1.0 - k) + k);
}

float MicrofacetNDF(in float alpha, in float NoH)
{
    float a2 = Sq(alpha);
    float cos2 = NoH * NoH;
    return (1.0 / M_PI) * Sq(alpha / (cos2 * (a2 - 1.0) + 1.0));
}

float LightAttenuation(vec3 lightPos, vec3 worldPos, float lightEnergy)
{
    float dist = distance(lightPos, worldPos);
    float attn0 = 1.0;
    float attn1 = dist * (1.0 / lightEnergy);
    float attn2 = dist * attn1;
    return 1.0 / (attn0 + attn1 + attn2);
}

void LightContribution(LightSource light, vec3 worldPos, out vec3 lightDir, out float intensity)
{
    float energy = light.colorAndEnergy.a;
    vec3 lightPos = light.positionAndType.xyz;
    int lightType = int((light.positionAndType.w));
    intensity = 0.0;
    lightDir = vec3(0.0);
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

vec3 BlinnPhongBRDF(vec3 normal, vec3 viewDir, vec3 lightDir, float specularPower, vec3 ambientColor, vec3 diffuseColor, vec3 specularColor)
{
    float NdotL = dot(normal, lightDir);
    float diffuseTerm = clamp(NdotL, 0.0, 1.0);
    vec3 diffuse = (diffuseColor - ambientColor) * diffuseTerm;
    vec3 halfDir = normalize(lightDir + viewDir);
    float NdotH = dot(normal, halfDir);
    float specularTerm = pow(clamp(NdotH, 0.0, 1.0), specularPower);
    vec3 specular = specularColor * specularTerm;
    return diffuse + specular;
}

vec3 CookTorranceBRDF(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness, vec3 diffuseColor, vec3 specularColor)
{
    float alpha = Sq(roughness);
    vec3 halfDir = normalize(lightDir + viewDir);
    float NoL = dot(normal, lightDir);
    float NoV = dot(normal, viewDir);
    float NoH = dot(normal, halfDir);
    float LoH = dot(lightDir, halfDir);
    float n = 1.5;
    float f0 = pow((1.0 - n) / (1.0 + n), 2.0);
    float specularFresnel = SchlickFresnel(f0, LoH);
    float geometry = SchlickGGX(alpha, NoV);
    float ndf = MicrofacetNDF(alpha, NoH);
    vec3 specular = specularColor / M_PI * (specularFresnel * geometry * ndf) / (4.0 * NoL * NoV);
    float diffuseFresnel = SchlickFresnel(f0, NoL);
    vec3 diffuse = diffuseColor / M_PI * (1.0 - diffuseFresnel);
    return diffuse + specular;
}

