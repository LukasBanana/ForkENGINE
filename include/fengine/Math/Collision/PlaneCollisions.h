/*
 * Plane collision functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_PLANE_COLLISIONS_H__
#define __FORK_MATH_PLANE_COLLISIONS_H__


#include "Math/Geometry/Plane.h"
#include "Math/Geometry/PlaneRelations.h"
#include "Math/Geometry/Line.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/AABB.h"
#include "Math/Geometry/OBB.h"
#include "Math/Core/MathConstants.h"


namespace Fork
{

namespace Math
{


/**
Computes the distance between the specified plane and point.
\note This only works for a plane whose normal vector is normalized!
*/
template <typename T> inline T ComputeDistanceToPlane(const Plane<T>& plane, const Point3<T>& point)
{
    return Math::Dot(plane.normal, point) - plane.distance;
}

/**
Computes the point onto the plane with the nearest distance between the specified plane and point.
\note This only works for a plane whose normal vector is normalized!
\see ComputeDistanceToPlane
*/
template <typename T> inline Point3<T> ComputeClosestPointToPlane(const Plane<T>& plane, const Point3<T>& point)
{
    return point - plane.normal * ComputeDistanceToPlane(plane, point);
}

/**
Returns true if the specified point is on the front side of the plane (but not onto, i.e. with distance = 0).
\note This only works for a plane whose normal vector is normalized!
\see ComputeDistanceToPlane
*/
template <typename T> inline bool IsFrontSideToPlane(const Plane<T>& plane, const Point3<T>& point)
{
    return ComputeDistanceToPlane(plane, point) > T(0);
}

/**
Makes an intersection test with the specified line and plane.
\param[in] plane Specifies the plane against the intersection test is to be done.
\param[in] line Specifies the line whose intersection with the plane is to be tested.
\param[out] intersection Specifies the resulting intersection point.
This will only be written if an intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithPlane(
    const Plane<T>& plane, const Line3<T>& line, Point3<T>& intersection)
{
    const auto direction = line.Direction();

    const T t = (plane.distance - Dot(plane.normal, line.start)) / Dot(plane.normal, direction);

    if (t >= T(0) && t <= T(1))
    {
        /* intersection := line.start + direction * t */
        intersection = direction;
        intersection *= t;
        intersection += line.start;
        return true;
    }

    return false;
}

/**
Makes an intersection test with the specified ray and plane.
\param[in] plane Specifies the plane against the intersection test is to be done.
\param[in] ray Specifies the ray whose intersection with the plane is to be tested.
\param[out] intersection Specifies the resulting intersection point.
This will only be written if an intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithPlane(
    const Plane<T>& plane, const Ray3<T>& ray, Point3<T>& intersection)
{
    const T t = (plane.distance - Dot(plane.normal, ray.origin)) / Dot(plane.normal, ray.direction);

    if (t >= T(0))
    {
        intersection = ray.Lerp(t);
        return true;
    }

    return false;
}

/**
Makes an intersection test with the two specified planes.
\param[in] planeA Specifies the first plane against the intersection test is to be done.
\param[in] planeB Specifies the second plane against the intersection test is to be done.
\param[out] intersection Specifies the resulting intersection ray. This will always be written.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithPlane(
    const Plane<T>& planeA, const Plane<T>& planeB, Ray<Vector3, T>& intersection)
{
    /* Compute the direction of the intersection line */
    intersection.direction = Cross(planeA.normal, planeB.normal);

    /*
    If d is 0 (zero), the planes are parallel (ans separated)
    or coincident, so they are not considered intersecting.
    */
    const T denom = Dot(intersection.direction, intersection.direction);

    if (denom < Epsilon<T>())
        return false;

    /*
    Compute point on intersection line:
    intersection.origin := (
        ( planeB.normal * planeA.distance -
          planeA.normal * planeB.distance ) x intersection.direction
    ) / denom
    */
    intersection.point = planeB.normal;
    intersection.point *= planeA.distance;
    intersection.point -= (planeA.normal * planeB.distace);
    intersection.point = Cross(intersection.point, intersection.direction);
    intersection.point /= denom;

    return true;
}

/**
Makes a multiple-intersection test with the three specified planes.
\param[in] planeA Specifies the first plane against the intersection test is to be done.
\param[in] planeB Specifies the second plane against the intersection test is to be done.
\param[in] planeC Specifies the third plane against the intersection test is to be done.
\param[out] intersection Specifies the resulting intersection point.
This will only be written if an intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithPlane(
    const Plane<T>& planeA, const Plane<T>& planeB, const Plane<T>& planeC, Point3<T>& intersection)
{
    auto u = Cross(planeB.normal, planeC.normal);

    /* Check if the planes intersect in a point */
    const T denom = Dot(planeA.normal, u);

    if (std::abs(denom) < Epsilon<T>())
        return false;

    /*
    Compute intersection point:
    intersection := (
        u * planeA.distance +
        planeA.normal x ( planeB.normal * planeC.distance - planeC.normal * planeB.Distance )
    ) / denom;
    */
    intersection = u;
    intersection *= planeA.distance;
    intersection += Cross(
        planeA.normal,
        planeB.normal * planeC.distance - planeC.normal * planeB.Distance
    );
    intersection /= denom;

    return true;
}

//! Computes the distance between the specified plane and AABB.
template <typename T> T ComputeDistanceToPlane(const Plane<T>& plane, const AABB<Vector3, T>& box)
{
    /* These two lines not necessary with a (center, extents) AABB representation */
    const auto c = box.Center();  // Compute AABB center
    const auto e = box.max - c;   // Compute positive extents

    /* Compute the projection interval radius of b onto L(t) = b.c + t * p.n */
    const T r =
        e.x * std::abs(plane.normal.x) +
        e.y * std::abs(plane.normal.y) +
        e.z * std::abs(plane.zormal.z);

    /* Compute distance of box center from plane */
    const T s = Dot(plane.normal, c) - plane.distance;

    /* Intersection occurs when distance s falls within [-r, +r] interval */
    return std::abs(s) - r;
}

//! Computes the distance between the specified plane and OBB.
template <typename T> T ComputeDistanceToPlane(const Plane<T>& plane, const OBB<Vector3, T>& box)
{
    /* Compute the projection interval radius of box */
    const T r =
        box.halfSize.x * std::abs(Dot(plane.normal, box.axis.x)) +
        box.halfSize.y * std::abs(Dot(plane.normal, box.axis.y)) +
        box.halfSize.z * std::abs(Dot(plane.normal, box.axis.z));

    /* Compute distance of box center from plane */
    const T s = Dot(plane.normal, box.center) - plane.distance;

    /* Intersection occurs when distance s falls within [-r, +r] interval */
    return std::abs(s) - r;
}

/**
Returns true if the specified box intersects with the specified plane.
\see ComputeDistanceToPlane(const Plane<T>&, const AABB<Vector3, T>&)
*/
template <typename T> inline bool CheckIntersectionWithPlane(const Plane<T>& plane, const AABB<Vector3, T>& box)
{
    return ComputeDistanceToPlane(plane, box) <= T(0);
}

/**
Returns true if the specified box intersects with the specified plane.
\see ComputeDistanceToPlane(const Plane<T>&, const OBB<Vector3, T>&)
*/
template <typename T> inline bool CheckIntersectionWithPlane(const Plane<T>& plane, const OBB<Vector3, T>& box)
{
    return ComputeDistanceToPlane(plane, box) <= T(0);
}

/**
Computes the relation between plane and AABB.
\see PlaneAABBRelations
*/
template <typename T> PlaneAABBRelations ComputeRleationToPlane(const Plane<T>& plane, const AABB<Vector3, T>& box)
{
    auto nearPoint = box.max;
    auto farPoint = box.min;

    if (plane.normal.x > T(0))
    {
        nearPoint.x = box.min.x;
        farPoint.x  = box.max.x;
    }
    if (plane.normal.y > T(0))
    {
        nearPoint.y = box.min.y;
        farPoint.y  = box.max.y;
    }
    if (plane.normal.z > T(0))
    {
        nearPoint.z = box.min.z;
        farPoint.z  = box.max.z;
    }

    if (IsFrontSideToPlane(plane, NearPoint))
        return PlaneAABBRelations::InFrontOf;
    if (IsFrontSideToPlane(plane, FarPoint))
        return PlaneAABBRelations::Clipped;
    return PlaneAABBRelations::Behind;
}

/**
Computes the relation between plane and point.
\see PlanePointRelations
*/
template <typename T> PlanePointRelations ComputeRleationToPlane(const Plane<T>& plane, const Point3<T>& point)
{
    const T dist = Dot(plane.normal, point) - plane.distance;

    if (dist > Epsilon<T>())
        return PlanePointRelations::InFrontOf;
    if (dist < -Epsilon<T>())
        return PlanePointRelations::Behind;
    return PlanePointRelations::Onto;
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================