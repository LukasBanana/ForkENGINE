// GLSL Shader
// Generated from HLSL Shader
// Mon Nov 24 22:34:13 2014


#pragma once
#include "Common.Core.glsl"
#ifndef VSM_MIN_VARIANCE
#	define VSM_MIN_VARIANCE 1.0
#endif
float ChebyshevUpperBound(vec2 moments, float t)
{
    float p = step(t, moments.x);
    float variance = moments.y - Sq(moments.x);
    variance = max(variance, VSM_MIN_VARIANCE);
    float d = t - moments.x;
    float p_max = variance / (variance + d * d);
    return max(p, p_max);
}

float LinearStep(float mi, float ma, float v)
{
    return clamp((v - mi) / (ma - mi), 0.0, 1.0);
}

float ReduceLightBleeding(float p_max, float Amount)
{
    return LinearStep(Amount, 1.0, p_max);
}

float ShadowContribution(vec2 moments, float lightDistance)
{
    float p_max = ChebyshevUpperBound(moments, lightDistance);
    return ReduceLightBleeding(p_max, 0.6);
}

vec2 ComputeMoments(float depth)
{
    vec2 moments;
    moments.x = depth;
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    moments.y = Sq(depth) + 0.25 * (Sq(dx) + Sq(dy));
    return moments;
}

