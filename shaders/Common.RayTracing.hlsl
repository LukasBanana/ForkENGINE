/*
 * Common.RayTracing HLSL shader
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#pragma once

#include "Common.Core.hlsl"

/*
Plane:	float4 -> normal (xyz), distance (w)
Sphere:	float4 -> position (xyz), radius (w)
*/

/* --- Functions --- */

// Builds a plane with the specified three coordinates.
float4 BuildPlane(float3 a, float3 b, float3 c)
{
	float4 plane;
	
	/*
	normal := || (b - a) x (c - a) ||
	distance := normal * a
	*/
	plane.xyz	= normalize(cross(b - a, c - a));
	plane.w		= dot(plane.xyz, a);
	
	return plane;
}

/*
Computes the signed distance between the plane and the point.
@note This only works for a plane whose normal vector is normalized!
*/
float PlanePointDistance(float4 plane, float3 point)
{
	return dot(plane.xyz, point) - plane.w;
}

// Returns true if the point is in front of the plane.
bool IsPointInFrontOfPlane(float4 plane, float3 point)
{
	return PlanePointDistance(plane, point) > 0.0;
}

// Computes the intersection between the ray and plane.
bool RayPlaneIntersection(float3 rayOrigin, float3 rayDir, float4 plane, out float3 point)
{
	float t = (plane.w - dot(plane.xyz, rayOrigin)) / dot(plane.xyz, rayDir);
	
	if (t >= 0.0)
	{
		point = rayOrigin + rayDir * t;
		return true;
	}
	
	return false;
}

// Checks whether there is an intersection between the ray and plane.
bool LinePlaneOcclusion(float3 start, float3 end, float4 plane)
{
	float t = (plane.w - dot(plane.xyz, start)) / dot(plane.xyz, end - start);
	return t >= 0.0 && t <= 1.0;
}

// Computes the intersection between the ray and sphere.
bool RaySphereIntersection(float3 rayOrigin, float3 rayDir, float4 sphere, out float3 point)
{
	/* Compute the sphere/ray vectors and scalar products */
	float3 dif = rayOrigin - sphere.xyz;
	float c = LengthSq(dif) - Sq(sphere.w);
	
	/* Exit if ray is inside sphere */
	if (c < 0.0)
		return false;
	
	float b = dot(dif, rayDir);
	
	/* Exit if ray points away from sphere (b > 0) */
	if (b > 0.0)
		return false;
	
	float discr = Sq(b) - c;
	
	/* Negative discriminant corresponds to ray missing sphere */
	if (discr < 0.0)
		return false;
	
	/* Compute final intersection point */
	float t = -b - sqrt(discr);
	
	point = rayOrigin + rayDir * t;
	
	return true;
}

// Checks whether there is an intersection between the ray and sphere.
bool LineSphereOcclusion(float3 start, float3 end, float3 dir, float4 sphere)
{
	/* Compute the sphere/ray vectors and scalar products */
	float3 dif = start - sphere.xyz;
	float c = LengthSq(dif) - Sq(sphere.w);
	
	/* Exit if line is inside sphere */
	if (c < 0.0)
		return false;
	
	float b = dot(dif, dir);
	
	/* Exit if line direction points away from sphere (b > 0) */
	if (b > 0.0)
		return false;
	
	float discr = Sq(b) - c;
	
	/* Negative discriminant corresponds to ray missing sphere */
	if (discr < 0.0)
		return false;
	
	/* Compute final intersection point */
	float t = -b - sqrt(discr);
	
    return Sq(t) <= LengthSq(start - end);
}

// Computes the intersection between the ray and triangle.
bool RayTriangleIntersection(float3 rayOrigin, float3 rayDir, float3 a, float3 b, float3 c, out float3 point)
{
	/* Get ray/triangle edge */
	float3 pa = a - rayOrigin;
	float3 pb = b - rayOrigin;
	float3 pc = c - rayOrigin;
	
	/* Check if direction is inside the edge AB, BC, CA */
	point.x = dot(pb, cross(rayDir, pc));
	if (point.x < 0.0)
		return false;
	
	point.y = dot(pc, cross(rayDir, pa));
	if (point.y < 0.0)
		return false;
	
	point.z = dot(pa, cross(rayDir, pb));
	if (point.z < 0.0)
		return false;
	
	/* Check ray intersection with plane, which is spanned by the triangle */
	return RayPlaneIntersection(rayOrigin, rayDir, BuildPlane(a, b, c), point);
}

