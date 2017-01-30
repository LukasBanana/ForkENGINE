/*
 * Cylinder collision functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_CYLINDER_COLLISIONS_H__
#define __FORK_MATH_CYLINDER_COLLISIONS_H__


#include "Math/Geometry/Line.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/Cylinder.h"
#include "Math/Core/MathConstants.h"
#include "Math/Core/BaseMath.h"
#include "Math/Collision/PlaneCollisions.h"


namespace Fork
{

namespace Math
{


/**
Makes an intersection test with the specified line and cylinder.
\param[in] cylinder Specifies the cylinder against the intersection test is to be done.
\param[in] line Specifies the line whose intersection with the box is to be tested.
\param[out] intersection Specifies the resulting intersection point.
This will only be written if an intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithCylinder(
    const Cylinder<T>& cylinder, const Line3<T>& line, Point3<T>& intersection)
{
    const auto& sa = line.start;
    const auto& sb = line.end;
    const auto& r = cylinder.radius;
    const auto& p = cylinder.orientation.origin;
    auto q = cylinder.orientation.Lerp(cylinder.height);
    T t = T(0);

    /* Make intersection tests with the cylinder's top cap */
    Vector3<T> planeIntersection;

    auto topCap = cylinder.TopPlane();

    if (CheckIntersectionWithPlane(topCap, line, planeIntersection))
    {
        if (DistanceSq(cylinder.TopPoint(), planeIntersection) < Sq(cylinder.radius))
        {
            intersection = planeIntersection;
            return true;
        }
    }

    /* Make intersection tests with the cylinder's bottom cap */
    auto bottomCap = cylinder.BottomPlane();

    if (CheckIntersectionWithPlane(bottomCap, line, planeIntersection))
    {
        if (DistanceSq(cylinder.BottomPoint(), planeIntersection) < Sq(cylinder.radius))
        {
            intersection = planeIntersection;
            return true;
        }
    }

    /* Make intersection tests with the cylinder's mantle */
    auto d = q - p, m = sa - p, n = sb - sa;

    T md = Dot(m, d);
    T nd = Dot(n, d);
    T dd = Dot(d, d);

    /* Test if segment fully outside either endcap of cylinder */
    if (md < T(0) && md + nd < T(0))
        return false;
    if (md > dd && md + nd > dd)
        return false;

    T nn = Dot(n, n);
    T mn = Dot(m, n);

    T a = dd*nn - nd*nd;
    T k = Dot(m, m) - r*r;
    T c = dd*k - md*md;

    if (std::abs(a) < Epsilon<T>())
    {
        /* Segment runs parallel to cylinder axis */
        if (c > T(0))
            return false;

        /*
        Now known that segment intersects cylinder
        -> figure out how it intersects
        */
        if (md < T(0))
            t = -mn/nn;
        else if (md > dd)
            t = (nd - mn) / nn;
        else
            t = T(0);

        intersection = line.Lerp(t);

        return true;
    }

    T b = dd*mn - nd*md;
    T discr = b*b - a*c;
    if (discr < T(0))
        return false;

    t = (-b - std::sqrt(discr)) / a;

    if (t < T(0) || t > T(1))
        return false;

    if (md + t*nd < T(0))
    {
        if (nd <= T(0))
            return false;

        t = -md / nd;

        /* Keep intersection if Dot(S(t) - p, S(t) - p) <= r^2 */
        if (k + T(2)*t*(mn + t*nn) <= T(0))
        {
            intersection = line.Lerp(t);
            return true;
        }

        return false;
    }
    else if (md + t*nd > dd)
    {
        if (nd >= T(0))
            return false;

        t = (dd - md) / nd;

        /* Keep intersection if Dot(S(t) - q, S(t) - q) <= r^2 */
        if (k + dd - T(2)*md + t*(T(2)*(mn - nd) + t*nn) <= T(0))
        {
            intersection = line.Lerp(t);
            return true;
        }

        return false;
    }

    /* Segment intersects cylinder between the endcaps; t is correct */
    intersection = line.Lerp(t);

    return true;
}

template <typename T> bool CheckIntersectionWithCylinder(const Cylinder<T>& cylinder, const Line3<T>& line)
{
    Point3<T> intersection;
    return CheckIntersectionWithCylinder(cylinder, line, intersection);
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================