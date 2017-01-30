/*
 * Triangle collision functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_TRIANGLE_COLLISIONS_H__
#define __FORK_MATH_TRIANGLE_COLLISIONS_H__


#include "Math/Geometry/Triangle.h"
#include "Math/Collision/PlaneCollisions.h"
#include "Math/Geometry/Line.h"
#include "Math/Geometry/Ray.h"


namespace Fork
{

namespace Math
{


//! Computes the distance between the specified plane and point.
/*template <typename T> inline T ComputeDistanceToPlane(const Plane<T>& plane, const Point3<T>& point)
{
    return (Dot(normal, point) - distance) / Dot(normal, normal);
}

//! Computes the point onto the plane with the nearest distance between the specified plane and point.
template <typename T> inline Point3<T> ComputeClosestPointToPlane(const Plane<T>& plane, const Point3<T>& point)
{
    return point - plane.normal * ComputeDistanceToPlane(plane, point);
}*/

/**
Checks the ray-triangle boundary.
\param[in] triangle Specifies the triangle.
\param[in] rayOrigin Specifies the ray origin.
\param[in] rayDirection Specifies the ray direction. This need not be normalized!
\param[out] intersection Specifies the resulting intersection point.
\remarks This is only used internally by the functions "CheckIntersectionWithTriangle".
\see CheckIntersectionWithTriangle(const Triangle3<T>&, const Line3<T>&, Point3<T>&)
\see CheckIntersectionWithTriangle(const Triangle3<T>&, const Ray3<T>&, Point3<T>&)
*/
template <typename T> bool CheckRayTriangleBoundary(
    const Triangle3<T>& triangle, const Point3<T>& rayOrigin, const Vector3<T>& rayDirection, Point3<T>& intersection)
{
    /* Get ray/triangle edges */
    const Vector3<T> pa { triangle.a - rayOrigin };
    const Vector3<T> pb { triangle.b - rayOrigin };
    const Vector3<T> pc { triangle.c - rayOrigin };
    
    /* Check if direction is inside the edges AB, BC, CA */
    intersection.x = Dot(pb, Cross(rayDirection, pc));
    if (intersection.x < T(0))
        return false;
    
    intersection.y = Dot(pc, Cross(rayDirection, pa));
    if (intersection.y < T(0))
        return false;
    
    intersection.z = Dot(pa, Cross(rayDirection, pb));
    if (intersection.z < T(0))
        return false;
    
    return true;
}

/**
Makes an intersection test with the specified line and triangle.
\param[in] triangle Specifies the triangle against the intersection test is to be done.
\param[in] line Specifies the line whose intersection with the triangle is to be tested.
\param[out] intersection Specifies the resulting intersection point.
This is perhaps written only partial if no intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithTriangle(
    const Triangle3<T>& triangle, const Line3<T>& line, Point3<T>& intersection)
{
    /* Check line-triangle boundary and make intersection test with the triangle's plane */
    return
        CheckRayTriangleBoundary(triangle, line.start, line.Direction(), intersection) &&
        CheckIntersectionWithPlane(Plane<T>(triangle), line, intersection);
}

/**
Makes an intersection test with the specified ray and triangle.
\param[in] triangle Specifies the triangle against the intersection test is to be done.
\param[in] ray Specifies the ray whose intersection with the triangle is to be tested.
\param[out] intersection Specifies the resulting intersection point.
This is perhaps written only partial if no intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithTriangle(
    const Triangle3<T>& triangle, const Ray3<T>& ray, Point3<T>& intersection)
{
    /* Check ray-triangle boundary and make intersection test with the triangle's plane */
    return
        CheckRayTriangleBoundary(triangle, ray.origin, ray.direction, intersection) &&
        CheckIntersectionWithPlane(Plane<T>(triangle), ray, intersection);
}

/**
Makes an intersection test with the triangle A against the triangle B.
\param[in] triangleA Specifies the first triangle.
\param[in] triangleB Specifies the second triangle.
\param[out] intersection Specifies the resulting intersection line segment.
This is perhaps written only partial if no intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithTriangle(
    const Triangle3<T>& triangleA, const Triangle3<T>& triangleB, Line3<T>& intersection)
{
    /*
    The algorithm of this function works as follows:
    - Make intersection tests: triangle A against edges of triangle B.
    - Make intersection tests: triangle B against edges of triangle A.
      -> Collect all intersection points (can only be two)
    - Collected points result in the intersection segment.
    */
    Vector3<T> intersectionPoint;
    size_t pointIndex = 0;
    
    auto CheckTriangleAgainstEdge = [&pointIndex, &intersectionPoint, &intersection](const Triangle3<T>& triangle, const Line3<T>& edge)
    {
        /* Make line/triangle intersection test */
        if (pointIndex < 2 && CheckIntersectionWithTriangle(triangle, edge, intersectionPoint))
        {
            if (pointIndex == 0 || !CompareVecEqual(intersection.start, intersectionPoint))
            {
                /* Add new intersection point */
                if (pointIndex == 0)
                    intersection.start = intersectionPoint;
                else
                    intersection.end = intersectionPoint;
                ++pointIndex;
            }
        }
    };

    auto CheckTriangleAgainstEdges = [&CheckTriangleAgainstEdge](const Triangle3<T>& triangle, const Triangle3<T>& oppositeTriangle)
    {
        /* Check edges in both directions */
        CheckTriangleAgainstEdge(triangle, { oppositeTriangle.a, oppositeTriangle.b });
        CheckTriangleAgainstEdge(triangle, { oppositeTriangle.b, oppositeTriangle.a });

        CheckTriangleAgainstEdge(triangle, { oppositeTriangle.b, oppositeTriangle.c });
        CheckTriangleAgainstEdge(triangle, { oppositeTriangle.c, oppositeTriangle.b });

        CheckTriangleAgainstEdge(triangle, { oppositeTriangle.c, oppositeTriangle.a });
        CheckTriangleAgainstEdge(triangle, { oppositeTriangle.a, oppositeTriangle.c });
    };

    /* Get all intersections with the triangle A and the three edges of triangle B */
    CheckTriangleAgainstEdges(triangleA, triangleB);
    if (pointIndex >= 2)
        return true;
    
    /* Get all intersections with the triangle B and the three edges of triangle A  */
    CheckTriangleAgainstEdges(triangleB, triangleA);
    if (pointIndex >= 2)
        return true;
    
    /* No intersection detected */
    return false;
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================