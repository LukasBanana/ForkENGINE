/*
 * Common.Shadow HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#pragma once

#include "Common.Core.hlsl"

/* --- Macros --- */

#ifndef VSM_MIN_VARIANCE
#	define VSM_MIN_VARIANCE 0.0001
#endif

/* --- Functions --- */

/**
Chebyshev inequality function for VSM (variance shadow maps), from GPUGems3.
@see http://http.developer.nvidia.com/GPUGems3/gpugems3_ch08.html
*/
float ChebyshevUpperBound(float2 moments, float t)
{
	/* One-tailed inequality valid if t > moments.x */
	float p = step(t, moments.x);
	
	/* Compute variance */
	float variance = moments.y - Sq(moments.x);
	variance = max(variance, VSM_MIN_VARIANCE);
	
	/* Compute probabilistic upper bound. */
	float d = t - moments.x;
	float p_max = variance / (variance + d*d);

	return max(p, p_max);
}

// Transforms the specified values v from [mi .. ma] to [0 .. 1.0].
float LinearStep(float mi, float ma, float v)
{
	return saturate((v - mi) / (ma - mi));
}

float ReduceLightBleeding(float p_max, float Amount)
{
	/* Remove the [0 .. amount] tail and linearly rescale [amount .. 1] */
	return LinearStep(Amount, 1.0, p_max);
}

float ShadowContribution(float2 moments, float lightDistance)
{
	/* Compute the Chebyshev upper bound */
	float p_max = ChebyshevUpperBound(moments, lightDistance);
	return ReduceLightBleeding(p_max, 0.6);
}

/*
Computes the moments for the chebyshev upper bound.
This should be used inside a pixel shader which generates a shadow map.
*/
float2 ComputeMoments(float depth)
{
	float2 moments;
	
	/* First moment is the depth itself */
	moments.x = depth;
	
	/* Compute partial derivatives of depth */
	float dx = ddx(depth);
	float dy = ddy(depth);
	
	moments.y = Sq(depth) + 0.25 * (Sq(dx) + Sq(dy));
	
	return moments;
}
