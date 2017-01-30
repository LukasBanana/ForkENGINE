// GLSL Shader
// Generated from HLSL Shader
// Thu Dec 18 20:14:07 2014


#pragma once
#include "Common.Core.glsl"
vec4 BuildPlane(vec3 a, vec3 b, vec3 c)
{
    vec4 plane;
    plane.xyz = normalize(cross(b - a, c - a));
    plane.w = dot(plane.xyz, a);
    return plane;
}

float PlanePointDistance(vec4 plane, vec3 point)
{
    return dot(plane.xyz, point) - plane.w;
}

bool IsPointInFrontOfPlane(vec4 plane, vec3 point)
{
    return PlanePointDistance(plane, point) > 0.0;
}

bool RayPlaneIntersection(vec3 rayOrigin, vec3 rayDir, vec4 plane, out vec3 point)
{
    float t = (plane.w - dot(plane.xyz, rayOrigin)) / dot(plane.xyz, rayDir);
    if (t >= 0.0)
    {
        point = rayOrigin + rayDir * t;
        return true;
    }
    return false;
}

bool LinePlaneOcclusion(vec3 start, vec3 end, vec4 plane)
{
    float t = (plane.w - dot(plane.xyz, start)) / dot(plane.xyz, end - start);
    return t >= 0.0 && t <= 1.0;
}

bool RaySphereIntersection(vec3 rayOrigin, vec3 rayDir, vec4 sphere, out vec3 point)
{
    vec3 dif = rayOrigin - sphere.xyz;
    float c = LengthSq(dif) - Sq(sphere.w);
    if (c < 0.0)
        return false;
    float b = dot(dif, rayDir);
    if (b > 0.0)
        return false;
    float discr = Sq(b) - c;
    if (discr < 0.0)
        return false;
    float t = -b - sqrt(discr);
    point = rayOrigin + rayDir * t;
    return true;
}

bool LineSphereOcclusion(vec3 start, vec3 end, vec3 dir, vec4 sphere)
{
    vec3 dif = start - sphere.xyz;
    float c = LengthSq(dif) - Sq(sphere.w);
    if (c < 0.0)
        return false;
    float b = dot(dif, dir);
    if (b > 0.0)
        return false;
    float discr = Sq(b) - c;
    if (discr < 0.0)
        return false;
    float t = -b - sqrt(discr);
    return Sq(t) <= LengthSq(start - end);
}

bool RayTriangleIntersection(vec3 rayOrigin, vec3 rayDir, vec3 a, vec3 b, vec3 c, out vec3 point)
{
    vec3 pa = a - rayOrigin;
    vec3 pb = b - rayOrigin;
    vec3 pc = c - rayOrigin;
    point.x = dot(pb, cross(rayDir, pc));
    if (point.x < 0.0)
        return false;
    point.y = dot(pc, cross(rayDir, pa));
    if (point.y < 0.0)
        return false;
    point.z = dot(pa, cross(rayDir, pb));
    if (point.z < 0.0)
        return false;
    return RayPlaneIntersection(rayOrigin, rayDir, BuildPlane(a, b, c), point);
}

