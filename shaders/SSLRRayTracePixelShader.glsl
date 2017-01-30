/*
 * SSLR Ray-Cast GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 430

layout(origin_upper_left) in vec4 gl_FragCoord;

/* --- Macros (Internals) --- */

#define EARLY_OUT_EPSILON	0.0001

#define MAX_ITERATIONS		200//100

#define MIN_STRIDE			0.001
#define MAX_STRIDE			0.1

#define saturate(x) clamp(x, 0.0, 1.0)

/* --- Shader input --- */

layout(binding = 0) uniform sampler2D colorBuffer;
layout(binding = 1) uniform sampler2D specularBuffer;
layout(binding = 2) uniform sampler2D depthBuffer;		// Post-projected depth buffer (in screen space)
layout(binding = 3) uniform sampler2D normalBuffer;		// Scene normal buffer (in screen space)

layout(std140) uniform pixelParam
{
	mat4	projectionMatrix;
	mat4	invProjectionMatrix;
	mat4	viewMatrix;
	float	nearPlane;
	float	globalReflectivity;
	float	globalRoughness;
};

// Input texture coordinate from the vertex shader
in vec2 vertexTexCoord;

layout(location = 0) out vec4 rayTraceColor;
layout(location = 1) out vec4 rayTraceMask;

/* --- Functions (Ray-Casting) --- */

/*
Transforms the point from view-space into screen-space.
Projection matrix must be in left-handed coordinate system.
*/
vec3 ProjectPoint(vec3 viewPoint)
{
	vec4 projPoint = projectionMatrix * vec4(viewPoint, 1.0);
	projPoint.xyz /= projPoint.w;
	projPoint.xy = projPoint.xy * vec2(0.5, -0.5) + vec2(0.5);
	return projPoint.xyz;
}

/*
Transform the point from screen-space into view-space.
Inverse-projection matrix must be in left-handed coordinate system.
*/
vec3 UnprojectPoint(vec3 screenPoint)
{
	screenPoint.xy = (screenPoint.xy - vec2(0.5)) * vec2(2.0, -2.0);
	vec4 projPos = invProjectionMatrix * vec4(screenPoint, 1.0);
	return projPos.xyz/projPos.w;
}

// Linear ray march procedure.
bool LinearRayTrace(inout vec3 rayPos, vec3 rayDir, float stepSize)
{
	vec3 prevPos = rayPos;
	rayDir = normalize(rayDir) * stepSize;
	
	for (uint i = 0; i < MAX_ITERATIONS; ++i)
	{
		/* Move to the next sample point */
		prevPos = rayPos;
		rayPos += rayDir;
		
		if (rayPos.x < 0.0 || rayPos.x > 1.0 || rayPos.y < 0.0 || rayPos.y > 1.0)
			return false;
		
		/* Check if the ray hit any geometry (delta < 0) */
		float depth = textureLod(depthBuffer, rayPos.xy, 0.0).r;
		float depthDelta = depth - rayPos.z;
		
		if (depthDelta < 0.0)
		{
			/*
			Move between the current and previous point and
			make a binary search, to quickly find the final hit point
			*/
			rayPos = (rayPos + prevPos) * 0.5;
			return true;
		}
	}
	
	return false;
}

/* --- Pixel Shader Main Function --- */

void main()
{
	/* Sample normal- and specular buffer */
	vec3 normal = textureLod(normalBuffer, vertexTexCoord, 0.0).rgb;
	float specular = textureLod(specularBuffer, vertexTexCoord, 0.0).r;
	
	if (dot(normal, normal) <= EARLY_OUT_EPSILON || specular <= EARLY_OUT_EPSILON)
	{
		rayTraceColor = vec4(0.0);
		rayTraceMask = vec4(0.0);
		return;
	}
	
	vec3 viewNormal = normalize(mat3(viewMatrix) * normal);
	
	/* Sample depth buffer */
	float depth = textureLod(depthBuffer, vertexTexCoord, 0.0).r;
	
	/* Determine step size depending on surface roughness */
	float roughness	= globalRoughness;//!TODO! -> sample specular buffer !!!
	float stepSize	= mix(MIN_STRIDE, MAX_STRIDE, roughness);
	
	/* Calculate view position in view space */
	vec3 screenPos	= vec3(vertexTexCoord, depth);
	vec3 viewPos	= UnprojectPoint(screenPos);
	vec3 viewDir	= normalize(viewPos);
	
	/* Calculate position and reflection ray in screen space */
	vec3 viewReflect = reflect(viewDir, viewNormal);
	
	vec3 screenReflectPos	= ProjectPoint(viewPos + viewReflect * nearPlane);
	vec3 screenReflect		= screenReflectPos - screenPos;
	
	/* Find intersection point of the reflection ray */
	if (LinearRayTrace(screenPos, screenReflect, stepSize))
	{
		/* Calculate ray trace length factor */
		float rayLength = min(distance(vertexTexCoord, screenPos.xy), 1.0);
		
		/* Write ray trace color buffer */
		rayTraceColor = vec4(textureLod(colorBuffer, screenPos.xy, 0.0).rgb, rayLength);
		rayTraceMask = vec4(1.0);
	}
	else
	{
		rayTraceColor = vec4(0.0);//textureLod(colorBuffer, vertexTexCoord, 0.0);//vec4(0.0); //!!!!!!!
		rayTraceMask = vec4(0.0);
	}
}
