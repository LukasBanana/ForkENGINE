// GLSL Shader
// Generated from HLSL Shader
// Thu Dec 18 17:11:52 2014


#pragma once
#define M_PI 3.141592654
float Sq(float x)
{
    return x * x;
}

float LengthSq(vec2 v)
{
    return dot(v, v);
}

float LengthSq(vec3 v)
{
    return dot(v, v);
}

float LengthSq(vec4 v)
{
    return dot(v, v);
}

vec3 ColorToVector(vec3 color)
{
    return color * 2.0 - 1.0;
}

vec3 VectorToColor(vec3 vector)
{
    return vector * 0.5 + 0.5;
}

float VectorAngle(vec3 a, vec3 b)
{
    return acos(dot(a, b));
}

vec3 ProjectPoint(mat4 projMatrix, vec3 viewPoint)
{
    vec4 projPoint = ((projMatrix) * (vec4(viewPoint, 1.0)));
    projPoint /= projPoint.w;
    projPoint.xy = projPoint.xy * vec2(0.5, -0.5) + vec2(0.5, 0.5);
    return projPoint.xyz;
}

vec3 UnprojectPoint(mat4 invProjMatrix, vec3 screenPoint)
{
    screenPoint.xy = (screenPoint.xy - vec2(0.5, 0.5)) * vec2(2.0, -2.0);
    vec4 projPos = ((invProjMatrix) * (vec4(screenPoint, 1.0)));
    projPos /= projPos.w;
    return projPos.xyz;
}

