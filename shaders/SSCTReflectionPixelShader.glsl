/*
 * SSCT Reflection GLSL pixel shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#version 430

layout(origin_upper_left) in vec4 gl_FragCoord;

/* --- Macros (Debugging and configuration) --- */

//#define _DEBUG_SHOW_REFLECTIONS_ONLY_

//#define _DEBUG_SSCT_
#ifdef _DEBUG_SSCT_
#	define _DEBUG_RAY_MARCH_
#	define _DEBUG_TRACE_METHOD_NONE 0
#	define _DEBUG_TRACE_METHOD_HIZ 1
#	define _DEBUG_TRACE_METHOD_BS 2
#endif

#define ENABLE_FALLBACK_LINEAR_SEARCH					// Enable fallback method with linear-search for Hi-Z ray tracing flaw
#define ENABLE_FADING									// Enable fading of reflections
//#define SPECULAR_POWER_TO_CONE_ANGLE					// Computes the cone angle by the specular power for optimal specular lobe coverage
//#define ENABLE_FALLBACK_ENVMAP							// Enable fallback method with environment map
//#define ENABLE_ENVMAP_FADING							// Enable fading of environment map (if disabled, it will already be accumulated to the reflections)

/* --- Macros (Internals) --- */

#define EARLY_OUT_EPSILON		0.0001

#define INVALID_HIT_POINT		vec3(-1.0)

#define HIZ_START_LEVEL			2.0
#define HIZ_STOP_LEVEL			0.0//2.0
#define HIZ_MAX_LEVEL			float(mipCount)
#define HIZ_CROSS_EPSILON		(vec2(0.5)/resolution)
#define HIZ_MAX_ITERATIONS		64						// Maximal number of iterations for Hi-Z ray-marching
#define HIZ_VIEWDIR_EPSILON		0.00001

#define LRM_MAX_ITERATIONS		100						// Maximal number of iterations for linear ray-marching
#define BS_MAX_ITERATIONS		30						// Maximal number of iterations for bineary search
#define BS_DELTA_EPSILON		0.0002

#define MIN_SPECULAR_POWER		0.1
#define MAX_SPECULAR_POWER		1024.0

#define FADE_BORDER_START		0.8
#define FADE_BORDER_END			1.0
#define FADE_MIRROR_FACTOR		10.0

#define ENVMAP_INTENSITY		0.5

#define SIGN(x) ((x) >= 0.0 ? 1.0 : -1.0)

#define saturate(x) clamp(x, 0.0, 1.0)

/* --- Shader input --- */

layout(binding = 0) uniform sampler2D colorBuffer;
layout(binding = 1) uniform sampler2D hiZBuffer;		// Post-projected Hi-Z depth buffer (in screen space)
layout(binding = 2) uniform sampler2D visibilityBuffer;
layout(binding = 3) uniform sampler2D normalBuffer;		// Scene normal buffer (in screen space)
layout(binding = 4) uniform sampler2D specularBuffer;

#ifdef ENABLE_FALLBACK_ENVMAP
layout(binding = 5) uniform samplerCube envColorBuffer;	// Environment color buffer
#endif

layout(std140) uniform pixelParam
{
	mat4	projectionMatrix;
	mat4	invProjectionMatrix;
	mat4	viewMatrix;
	int		mipCount;
	int		_pad0;
	vec2	resolution;
	float	nearPlane;
	float	farPlane;
	float	globalReflectivity;
	float	globalRoughness;
	#ifdef _DEBUG_SSCT_
	vec4	_deb_mousepos;
	ivec4	_deb_options;
	#endif
};

#ifdef _DEBUG_RAY_MARCH_

layout(std430) buffer _DebugOutput_
{
	vec4	originalRay;
	vec4	reflectionPos;
	vec4	reflectionRay;
	vec4	rayPoints[64];
	vec4	firstRayPos;
	uint	iterations;
	float	cellBoundaryFactor;
	int		earlyExit;
	int		cellsCrossed;
}
_deboutp;

#endif

// Input texture coordinate from the vertex shader
in vec2 vertexTexCoord;

// Final pixel color for this post-processing effect
out vec4 fragColor;

/* --- Functions (Ray-Casting) --- */

// Linearize z (result is in range [0 .. 1] just like the post-projected z value).
float Linearize(float z)
{
	return (2.0 * nearPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

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

vec3 IntersectDepthPlane(vec3 rayOrigin, vec3 rayDir, float t)
{
	return rayOrigin + rayDir * t;
}

vec2 GetCellCount(vec2 size, float level)
{
	return floor(size / (level > 0.0 ? exp2(level) : 1.0));
}

vec2 GetCell(vec2 ray, vec2 cellCount)
{
	return floor(ray * cellCount);
}

/*
This function computes the new ray position where
the ray intersects with the specified cell's boundary.
*/
vec3 IntersectCellBoundary(vec3 rayOrigin, vec3 rayDir, vec2 cellIndex, vec2 cellCount, vec2 crossStep, vec2 crossOffset)
{
	/*
	Determine in which adjacent cell the ray resides.
	The cell index always refers to the left-top corner of a cell.
	So add "crossStep" whose components are either 1.0
	(for positive direction) or 0.0 (for negative direction),
	and then add "crossOffset" so that we are in the center of that cell.
	*/
	vec2 cell = cellIndex + crossStep;
	cell /= cellCount;
	cell += crossOffset;
	
	/* Now compute interpolation factor "t" with this cell position and the ray origin */
	vec2 delta = cell - rayOrigin.xy;
	delta /= rayDir.xy;
	
	float t = min(delta.x, delta.y);
	
	/* Return final ray position */
	return IntersectDepthPlane(rayOrigin, rayDir, t);
}

vec2 GetDepthPlanes(vec2 pos, float level)
{
	/* Texture lookup with <linear-clamp> sampler */
	return textureLod(hiZBuffer, pos, level).rg;
}

vec2 GetDepthPlanesFromCell(float level, vec2 cellIndex, vec2 cellCount, vec2 crossStep, vec2 crossOffset)
{
	#if 1
	vec2 cell = cellIndex + crossStep;
	cell /= cellCount;
	cell += crossOffset;
	#else
	vec2 cell = cellIndex + vec2(0.5);
	cell /= cellCount;
	#endif
	
	return GetDepthPlanes(cell, level);
}

bool CrossedCellBoundary(vec2 cellIndexA, vec2 cellIndexB)
{
	return cellIndexA.x != cellIndexB.x || cellIndexA.y != cellIndexB.y;
}

#if 1//!!!
bool LinearRayTrace(inout vec3 rayPos, vec3 rayDir);
#endif

/*
Hi-Z ray tracing function.
rayOrigin: Ray start position (in screen space).
rayDir: Ray reflection vector (in screen space).
*/
vec3 HiZRayTrace(
	vec3 rayOrigin, vec3 rayDir
	#ifdef _DEBUG_RAY_MARCH_
	,vec2 _Deb_PixelPos, inout int _Deb_ReachedPixelIterator, bool _Deb_WriteDebugOutput, out int _Deb_TraceMethod
	#endif
	)
{
	const vec2 hiZSize = resolution;
	
	#ifdef _DEBUG_RAY_MARCH_
	_Deb_TraceMethod = _DEBUG_TRACE_METHOD_NONE;
	if (_Deb_WriteDebugOutput)
		_deboutp.originalRay = vec4(rayDir, 1.0);
	#endif
	
	/* Check if ray points towards the camera */
	if (rayDir.z <= HIZ_VIEWDIR_EPSILON)
	{
		#ifdef ENABLE_FALLBACK_LINEAR_SEARCH
		if (LinearRayTrace(rayOrigin, rayDir))
		{
			#ifdef _DEBUG_RAY_MARCH_
			_Deb_TraceMethod = _DEBUG_TRACE_METHOD_BS;
			#endif
			return rayOrigin;
		}
		#endif
		return INVALID_HIT_POINT;
	}
	
	/*
	Get the cell cross direction and a small offset
	to enter the next cell when doing cell crossing
	*/
	vec2 crossStep = vec2(SIGN(rayDir.x), SIGN(rayDir.y));
	vec2 crossOffset = crossStep * HIZ_CROSS_EPSILON;
	crossStep = saturate(crossStep);
	
	/* Set hit point to the original screen coordinate and depth */
	vec3 rayPos = rayOrigin;
	
	/* Scale vector such that z is 1.0 (maximum depth) */
	rayDir = rayDir / rayDir.z;
	
	/* Set starting point to the point where z equals 0.0 (minimum depth) */
	rayOrigin = IntersectDepthPlane(rayOrigin, rayDir, -rayOrigin.z);
	
	/* Cross to next cell so that we don't get a self-intersection immediately */
	float level = HIZ_START_LEVEL;
	
	vec2 firstCellCount = GetCellCount(hiZSize, level);
	#if 1
	vec2 rayCell = GetCell(rayPos.xy, firstCellCount);
	rayPos = IntersectCellBoundary(rayOrigin, rayDir, rayCell, firstCellCount, crossStep, crossOffset);//*16.0);
	#else
	vec2 rayCell = GetCell(rayPos.xy, hiZSize);
	rayPos = IntersectCellBoundary(rayOrigin, rayDir, rayCell, hiZSize, crossStep, crossOffset);//crossOffset);
	#endif
	
	#ifdef _DEBUG_RAY_MARCH_
	
	int numCellsCrossed = 0;
	
	if (_Deb_WriteDebugOutput)
	{
		vec2 cell = rayCell + crossStep;
		cell /= firstCellCount;
		cell += crossOffset;
		
		vec2 delta = cell - rayOrigin.xy;
		delta /= rayDir.xy;
		
		float t = min(delta.x, delta.y);
		
		_deboutp.cellBoundaryFactor = t;
		_deboutp.earlyExit = 0;
		_deboutp.firstRayPos = vec4(rayPos, 1.0);
	}
	
	#endif
	
	/* Main tracing iteration loop */
	uint i = 0;
	
	for (/*uint i = 0*/; level >= HIZ_STOP_LEVEL && i < HIZ_MAX_ITERATIONS; ++i)
	{
		/* Get cell number of our current ray */
		const vec2 cellCount = GetCellCount(hiZSize, level);
		const vec2 oldCellIndex = GetCell(rayPos.xy, cellCount);
		
		/* Get minimum depth plane in which the current ray resides */
		#if 1
		vec2 zMinMax = GetDepthPlanes(rayPos.xy, level);
		#else
		vec2 zMinMax = GetDepthPlanesFromCell(level, oldCellIndex, cellCount, crossStep, crossOffset);
		#endif
		
		#ifdef _DEBUG_RAY_MARCH_
		
		if (_Deb_WriteDebugOutput)
			_deboutp.rayPoints[i] = vec4(rayPos, 1.0);
		else
		{
			// Visualize ray position in current iteration
			if (_deb_options.x != 0)
			{
				// Visualize MIP LODs
				vec2 _pointCell = GetCell(_Deb_PixelPos, cellCount);
				if (oldCellIndex.x == _pointCell.x && oldCellIndex.y == _pointCell.y)
				{
					_Deb_ReachedPixelIterator = int(i);
					rayPos.z = level;
					break;
				}
			}
			else
			{
				// Visualize fixed size boxes
				float _boxSize = 4.0;
				vec2 _pointCell = GetCell(_Deb_PixelPos, resolution);
				vec2 _pixelCell = GetCell(rayPos.xy, resolution);
				if (abs(_pixelCell.x - _pointCell.x) < _boxSize && abs(_pixelCell.y - _pointCell.y) < _boxSize)
				{
					_Deb_ReachedPixelIterator = int(i);
					rayPos.z = level;
					break;
				}
			}
		}
		
		#endif
		
		/* Intersect only if ray depth is between minimum and maximum depth planes */
		#if 0
		vec3 tmpRay = IntersectDepthPlane(rayOrigin, rayDir, clamp(rayPos.z, zMinMax.r, zMinMax.g)); // MIN/MAX
		#else
		vec3 tmpRay = IntersectDepthPlane(rayOrigin, rayDir, max(rayPos.z, zMinMax.r)); // MIN
		#endif
		
		/* Get new cell number */
		const vec2 newCellIndex = GetCell(tmpRay.xy, cellCount);
		
		/* If the new cell number is different from the old cell number, we know we crossed a cell */
		if (CrossedCellBoundary(oldCellIndex, newCellIndex))// || tmpRay.z > zMinMax.g + 0.001)
		{
			/*
			Intersect the boundary of that cell instead,
			and go up a level for taking a larger step next iteration
			*/
			tmpRay = IntersectCellBoundary(rayOrigin, rayDir, oldCellIndex, cellCount, crossStep, crossOffset);
			level = min(HIZ_MAX_LEVEL, level + 2.0);
			
			#ifdef _DEBUG_RAY_MARCH_
			if (_Deb_WriteDebugOutput)
				++numCellsCrossed;
			#endif
		}
		
		rayPos = tmpRay;
		
		#if 1
		if (rayPos.x < 0.0 || rayPos.x > 1.0 || rayPos.y < 0.0 || rayPos.y > 1.0)
		{
			rayPos = INVALID_HIT_POINT;
			#ifdef _DEBUG_RAY_MARCH_
			if (_Deb_WriteDebugOutput)
				_deboutp.earlyExit = 1;
			#endif
			break;
		}
		#endif
		
		/* Go down a level in the Hi-Z */
		level -= 1.0;
		
		#if 0//!!!
		/* Reject ray intersections from hidden geometry */
		if (level < HIZ_STOP_LEVEL && Linearize(zMinMax.r) - Linearize(rayPos.z) > 0.002)
		{
			rayPos = INVALID_HIT_POINT;
			break;
		}
		#endif
	}
	
	#ifdef _DEBUG_RAY_MARCH_
	if (_Deb_WriteDebugOutput)
	{
		_deboutp.reflectionPos = vec4(rayOrigin, 1.0);
		_deboutp.reflectionRay = vec4(rayDir, 1.0);
		_deboutp.iterations = i;
		_deboutp.cellsCrossed = numCellsCrossed;
	};
	if (rayPos.x > 0.0 && rayPos.x < 1.0 && rayPos.y > 0.0 && rayPos.y < 1.0)
		_Deb_TraceMethod = _DEBUG_TRACE_METHOD_HIZ;
	#endif
	
	return rayPos;
}

// Binary search for the alternative ray tracing function.
void LinearRayTraceBinarySearch(inout vec3 rayPos, vec3 rayDir)
{
	for (uint i = 0; i < BS_MAX_ITERATIONS; ++i)
	{
		/* Check if we are out of screen */
		if (rayPos.x < 0.0 || rayPos.x > 1.0 || rayPos.y < 0.0 || rayPos.y > 1.0)
			break;
		
		/* Check if we found our final hit point */
		float depth = textureLod(hiZBuffer, rayPos.xy, 0.0).r;
		float depthDelta = depth - rayPos.z;
		
		if (abs(depthDelta) < BS_DELTA_EPSILON)
			break;
		
		/*
		Move ray forwards if we are in front of geometry and
		move backwards, if we are behind geometry
		*/
		if (depthDelta > 0.0)
			rayPos += rayDir;
		else
			rayPos -= rayDir;
		
		rayDir *= 0.5;
	}
}

// Alternative ray tracing function.
bool LinearRayTrace(inout vec3 rayPos, vec3 rayDir)
{
	vec3 prevPos = rayPos;
	rayDir = normalize(rayDir);
	float stepSize = 0.001;
	int numScreenEdgeHits = 0;
	
	for (uint i = 0; i < LRM_MAX_ITERATIONS; ++i)
	{
		/* Move to the next sample point */
		prevPos = rayPos;
		rayPos += rayDir * stepSize;
		
		if (rayPos.x < 0.0 || rayPos.x > 1.0 || rayPos.y < 0.0 || rayPos.y > 1.0)
		{
			if (numScreenEdgeHits < 3)
			{
				/* Move ray back if the jump was too long */
				++numScreenEdgeHits;
				rayPos = prevPos;
				stepSize *= 0.5;
				continue;
			}
			else
				break;
		}
		else
			numScreenEdgeHits = 0;
		
		/* Check if the ray hit any geometry (delta < 0) */
		float depth = textureLod(hiZBuffer, rayPos.xy, 0.0).r;
		float depthDelta = depth - rayPos.z;
		
		if (depthDelta < 0.0)
		{
//			if (Linearize(depthDelta) < -0.01)
//				break;
			
			/*
			Move between the current and previous point and
			make a binary search, to quickly find the final hit point
			*/
			rayPos = (rayPos + prevPos) * 0.5;
			LinearRayTraceBinarySearch(rayPos, rayDir * (stepSize * 0.25));
			return true;
		}
		
		stepSize *= 1.5;
	}
	
	rayPos = INVALID_HIT_POINT;
	return false;
}

/* --- Functions (Cone-Tracing) --- */

float RoughnessToSpecularPower(float roughness)
{
	return mix(MAX_SPECULAR_POWER, MIN_SPECULAR_POWER, pow(roughness, 0.1));
}

float RoughnessToConeAngle(float roughness)
{
	return mix(0.0, 1.0, roughness);
}

float SpecularPowerToConeAngle(float specularPower)
{
	if (specularPower < MAX_SPECULAR_POWER)
	{
		/* Based on phong reflection model */
		const float xi = 0.244;
		float exponent = 1.0 / (specularPower + 1.0);
		return acos(pow(xi, exponent));
	}
	return 0.0;
}

float IsoscelesTriangleOpposite(float adjacentLength, float coneTheta)
{
	/*
	Simple trig and algebra - soh, cah, toa - tan(theta) = opp/adj,
	opp = tan(theta) * adj, then multiply by 2 for isosceles triangle base
	*/
	return 2.0 * tan(coneTheta) * adjacentLength;
}

float IsoscelesTriangleInRadius(float a, float h)
{
	float h4 = h*4.0;
	return (a * (sqrt(a*a + h4*h) - a)) / max(h4, 0.00001);
}

vec4 ConeSampleWeightedColor(vec2 samplePos, float mipLevel)
{
	/* Sample color buffer with pre-integrated visibility */
	vec3 color = textureLod(colorBuffer, samplePos, mipLevel).rgb;
	float visibility = textureLod(visibilityBuffer, samplePos, mipLevel).r;
	return vec4(color * visibility, visibility);
}

#if 0
vec4 ConeSampleWeightedColor2(vec2 samplePos, float mipLevel, vec3 rayStartVS, float radiusSS, float gloss)
{
	// sample center and take additional sample points around the cone perimeter and inside to better blend the color result
	vec3 sampleColor = textureLod(colorBuffer, samplePos, mipLevel).rgb;
	
	vec3 sr = textureLod(colorBuffer, samplePos + vec2(radiusSS, 0.0), mipLevel).rgb;
	vec3 sl = textureLod(colorBuffer, samplePos - vec2(radiusSS, 0.0), mipLevel).rgb;
	vec3 sb = textureLod(colorBuffer, samplePos + vec2(0.0, radiusSS), mipLevel).rgb;
	vec3 st = textureLod(colorBuffer, samplePos - vec2(0.0, radiusSS), mipLevel).rgb;
	
	float halfRadiusSS = radiusSS * 0.5;
	
	vec3 srh = textureLod(colorBuffer, samplePos + vec2(halfRadiusSS, halfRadiusSS), mipLevel).rgb;
	vec3 slh = textureLod(colorBuffer, samplePos - vec2(halfRadiusSS, halfRadiusSS), mipLevel).rgb;
	vec3 sbh = textureLod(colorBuffer, samplePos + vec2(halfRadiusSS, halfRadiusSS), mipLevel).rgb;
	vec3 sth = textureLod(colorBuffer, samplePos - vec2(halfRadiusSS, halfRadiusSS), mipLevel).rgb;
	
	vec3 blendedColor = (sr + sl + sb + st + srh + slh + sbh + sth + sampleColor) / 9.0;
	
	float visibility = textureLod(visibilityBuffer, samplePos, mipLevel).r;
	
	float depth = textureLod(hiZBuffer, samplePos, 0.0).r;
	vec3 rayEndVS = UnprojectPoint(vec3(samplePos, depth));
	float distanceTraveled = length(rayEndVS - rayStartVS);
	
	// distance squared caused the effect to fade too fast for larger objects in the distance
	float attenuation = (distanceTraveled == 0.0 ? 1.0 : saturate(1.0 / distanceTraveled));
	visibility *= attenuation;
	
	// try mix and matching what goes into the alpha component - don't want to drive it down too much
	return vec4(blendedColor * (visibility * gloss), visibility); // gloss intentionally left out of alpha
}
#endif

float IsoscelesTriangleNextAdjacent(float adjacentLength, float incircleRadius)
{
	/*
	Subtract the diameter of the incircle
	to get the adjacent side of the next level on the cone
	*/
	return adjacentLength - (incircleRadius * 2.0);
}

/*
Hi-Z cone tracing function.
rayPos: ray intersection point
*/
vec4 HiZConeTrace(vec2 rayPos)
{
	/* Read roughness from specular map and convert to a BRDF specular power */
	float roughness = globalRoughness;
	float specularPower = RoughnessToSpecularPower(roughness);
	
	/*
	Cone angle is maximum extent of the specular lobe aperture.
	We only need the half cone angle, because the computations of our isosceles
	triangle is based on a right-angled triangle (only the half of our cone).
	*/
	#ifdef SPECULAR_POWER_TO_CONE_ANGLE
	float coneTheta = SpecularPowerToConeAngle(specularPower)*0.5;
	#else
	float coneTheta = RoughnessToConeAngle(roughness)*0.5;
	#endif
	
	/* Cone tracing using an isosceles triangle to approximate a cone in screen space */
	vec2 deltaPos = rayPos.xy - vertexTexCoord;
	
	float adjacentLength = length(deltaPos);
	vec2 adjacentUnit = normalize(deltaPos);
	
	vec4 reflectionColor = vec4(0.0);
	
	/* Append offset to adjacent length, so we have our first inner circle */
	adjacentLength += IsoscelesTriangleOpposite(adjacentLength, coneTheta);
	
	for (int i = 0; i < 7; ++i)
	{
		/* Intersection length is the adjacent side, get the opposite side using trigonometry */
		float oppositeLength = IsoscelesTriangleOpposite(adjacentLength, coneTheta);
		
		/* Calculate in-radius of the isosceles triangle now */
		float incircleSize = IsoscelesTriangleInRadius(oppositeLength, adjacentLength);
		
		/* Get the sample position in screen space */
		vec2 samplePos = vertexTexCoord + adjacentUnit * (adjacentLength - incircleSize);
		
		/*
		Convert the in-radius into screen space and then check what power N
		we have to raise 2 to reach it. That power N becomes our mip level to sample from.
		*/
		float mipLevel = log2(incircleSize * max(resolution.x, resolution.y));
		
		/*
		Read color and accumulate it using trilinear filtering (blending in xy and mip direction) and weight it.
		Uses pre-convolved image, pre-integrated visibility buffer and Hi-Z buffer. It checks if cone sphere is below,
		in between, or above the Hi-Z minimum and maximum and weights it together with transparency.
		Visibility is accumulated in the alpha channel.
		*/
		reflectionColor += ConeSampleWeightedColor(samplePos, mipLevel);
//		reflectionColor += ConeSampleWeightedColor2(samplePos, mipLevel, viewPos, incircleSize, 1.0);
		
		if (reflectionColor.a > 1.0)
			break;
		
		/* Calculate next smaller triangle that approximates the cone in screen space */
		adjacentLength = IsoscelesTriangleNextAdjacent(adjacentLength, incircleSize);
	}
	
	/* Normalize reflection color (if visiblity is greater than 1.0 */
	reflectionColor /= reflectionColor.a;
	
	return reflectionColor;
}

#ifdef ENABLE_FALLBACK_ENVMAP

float ConeAngleToMIPLevel(float coneTheta, float mipMaxLevel)
{
	return mix(0.0, mipMaxLevel, coneTheta);
}

// Samples the environment cube-map with the specified cone angle 'theta' (must be the full cone angle).
vec3 SampleCubeMap(vec3 dir, float coneTheta)
{
	float mipMaxLevel = float(textureQueryLevels(envColorBuffer));
	float mipLevelBias = ConeAngleToMIPLevel(coneTheta, mipMaxLevel);
	return texture(envColorBuffer, dir, mipLevelBias).rgb;
}

#endif

/* --- Pixel Shader Main Function --- */

#ifdef _DEBUG_SSCT_

vec2 _DebVecToRGB(vec2 normal)
{
	return normal*vec2(0.5)+vec2(0.5);
}
vec3 _DebVecToRGB(vec3 normal)
{
	return normal*vec3(0.5)+vec3(0.5);
}

#endif

void main()
{
	/* ----- Ray Tracing Pass ----- */
	
	/* Sample normal- and specular buffer */
	vec3 normal = textureLod(normalBuffer, vertexTexCoord, 0.0).rgb;
	float specular = textureLod(specularBuffer, vertexTexCoord, 0.0).r;
	
	if (dot(normal, normal) <= EARLY_OUT_EPSILON || specular <= EARLY_OUT_EPSILON)
	{
		/* Only sample color buffer; this is used for the skybox, which has is no reflector) */
		fragColor = textureLod(colorBuffer, vertexTexCoord, 0.0);
		return;
	}
	
	vec3 viewNormal = normalize(mat3(viewMatrix) * normal);
	
	/* Sample Hi-Z buffer */
	float depth = textureLod(hiZBuffer, vertexTexCoord, 0.0).r;
	
	/* Calculate view position in view space */
	vec3 screenPos	= vec3(vertexTexCoord, depth);
	vec3 viewPos	= UnprojectPoint(screenPos);
	vec3 viewDir	= normalize(viewPos);
	
	/* Calculate position and reflection ray in screen space */
	vec3 viewReflect = reflect(viewDir, viewNormal);
	
	vec3 screenReflectPos	= ProjectPoint(viewPos + viewReflect * nearPlane);
	vec3 screenReflect		= screenReflectPos - screenPos;
	
	/* Find intersection point of the reflection ray */
	#if 1
	#	ifdef _DEBUG_RAY_MARCH_
	int _Deb_Dummy = 0;
	int _Deb_TraceMethod = 0;
	vec3 rayPos = HiZRayTrace(screenPos, screenReflect, vec2(-100.0), _Deb_Dummy, false, _Deb_TraceMethod);
	#	else
	vec3 rayPos = HiZRayTrace(screenPos, screenReflect);
	#	endif
	#else
	vec3 rayPos = screenPos;
	LinearRayTrace(rayPos, screenReflect);
	#endif
	
	#ifdef _DEBUG_SSCT_
	
	// !!!!!!!!!!!!!!!!!! DEBUGGING !!!!!!!!!!!!!!!!!!
	
	if (_deb_options.z != 0)
	{
		// COMMON TESTS
		vec3 color = vec3(0.0);
		switch (_deb_options.z)
		{
			case 1: color = fract(viewPos); break; // view-space position
			case 2: color = vec3(fract(length(viewPos))); break; // linear depth
			case 3: color = vec3(depth); break; // post-procested depth
			case 4: color = _DebVecToRGB(viewNormal); break; // view-space normal
			case 5: color = _DebVecToRGB(viewReflect); break; // view-space reflection
			case 6: color.rg = _DebVecToRGB(normalize(screenReflect.xy)); break; // screen-space reflection
			case 7: color = vec3((screenReflect.z > 0.0 ? screenReflect.z : 0.0), 0.0, (screenReflect.z <= 0.0 ? -screenReflect.z : 0.0))*100.0; break; // screen-space reflection
			//case 8: color = vec3(abs(screenReflect.xy/screenReflect.z), 0.0); break; // screen-space reflection
			case 8:
				switch (_Deb_TraceMethod)
				{
					case _DEBUG_TRACE_METHOD_NONE: color = vec3(0.0, 0.0, 0.0); break;
					case _DEBUG_TRACE_METHOD_HIZ: color = vec3(0.0, 1.0, 0.0); break;
					case _DEBUG_TRACE_METHOD_BS: color = vec3(1.0, 0.0, 0.0); break;
				}
				break;
			case 9: color.rg = rayPos.xy; break; // ray hit point
		}
		fragColor = vec4(color, 1.0);
	}
	
	#ifdef _DEBUG_RAY_MARCH_
	
	if (_deb_options.y != 0)
	{
		normal = textureLod(normalBuffer, _deb_mousepos.xy, 0.0).rgb;
		depth = textureLod(hiZBuffer, _deb_mousepos.xy, 0.0).r;
		
		viewNormal = normalize(mat3(viewMatrix) * normal);
		
		vec3 screenPos2	= vec3(_deb_mousepos.xy, depth);
		viewPos			= UnprojectPoint(screenPos2);
		viewDir 		= normalize(viewPos);
		
		vec3 viewReflect2 = reflect(viewDir, viewNormal);
		
		screenReflectPos	= ProjectPoint(viewPos + viewReflect2 * nearPlane);
		screenReflect		= screenReflectPos - screenPos2;
		
		int ReachedPixelIt = -1;
		vec3 _ray = HiZRayTrace(screenPos2, screenReflect, gl_FragCoord.xy/resolution, ReachedPixelIt, gl_SampleID == 0, _Deb_TraceMethod);
		
		if (ReachedPixelIt >= 0)
		{
			vec3 colorPallete[] = {
				vec3(0.8, 0.9, 1.0), vec3(1.0, 0.7, 0.8), vec3(1.0, 0.8, 0.6),
				vec3(0.5, 1.0, 0.9), vec3(0.7, 0.7, 1.0), vec3(0.8, 0.9, 0.5),
				vec3(0.5, 0.9, 0.7), vec3(0.8, 0.7, 1.0), vec3(1.0, 0.4, 1.0),
			};
			fragColor = vec4(colorPallete[ReachedPixelIt % 9], 1.0);
			return;
		}
	}
	
	if (_deb_options.z != 0)
		return;
	
	#endif
	
	// !!!!!!!!!!!!!!!!!! /DEBUGGING !!!!!!!!!!!!!!!!!!
	
	#endif
	
	/* ----- Cone Tracing Pass ----- */
	
	/* Compute Hi-Z cone tracing */
	vec4 reflectionColor = HiZConeTrace(rayPos.xy);
	
	/* Sample base image color */
	vec4 diffuseColor = textureLod(colorBuffer, vertexTexCoord, 0.0);
	
	#ifdef ENABLE_FADING
	
	/* Compute fading on border */
	float boundary = distance(rayPos.xy, vec2(0.5))*2.0;
	float fadeOnBorder = 1.0 - saturate((boundary - FADE_BORDER_START) / (FADE_BORDER_END - FADE_BORDER_START));
	
	#	ifndef ENABLE_FALLBACK_LINEAR_SEARCH
	
	/* Compute fading on mirror (rays towards the camera) */
	float fadeOnMirror = saturate(viewReflect.z * FADE_MIRROR_FACTOR);
	reflectionColor.rgb *= (fadeOnBorder * fadeOnMirror);
	
	#	else
	
	reflectionColor.rgb *= fadeOnBorder;
	
	#	endif
	
	#endif
	
	#ifdef ENABLE_FALLBACK_ENVMAP
	
	#ifdef ENABLE_ENVMAP_FADING
	if (hiZFading < 1.0)
	{
	#endif
		vec3 invViewReflect = normalize(transpose(mat3(viewMatrix)) * viewReflect);
		vec3 envSample = SampleCubeMap(invViewReflect, coneTheta*2.0);
	#ifndef ENABLE_ENVMAP_FADING
		reflectionColor.rgb += mix(envSample, reflectionColor.rgb, hiZFading) * ENVMAP_INTENSITY;
	#else
		reflectionColor.rgb = mix(envSample, reflectionColor.rgb, hiZFading);
	}
	#endif
	
	#endif
	
	/* Apply reflectivity factor */
	reflectionColor.rgb *= globalReflectivity;
	
	/* Add final reflection color */
	#ifdef _DEBUG_SHOW_REFLECTIONS_ONLY_
	diffuseColor.rgb = reflectionColor.rgb + diffuseColor.rgb*0.0001;//!!!
	#else
	diffuseColor.rgb += reflectionColor.rgb;
	#endif
	
	fragColor = diffuseColor;
}
