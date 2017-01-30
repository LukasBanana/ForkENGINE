/*
 * Ray collision functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_RAY_COLLISIONS_H__
#define __FORK_MATH_RAY_COLLISIONS_H__


#include "Math/Geometry/Line.h"
#include "Math/Geometry/Ray.h"
#include "Math/Core/MathConstants.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace Math
{


/**
Computes the closest line segment between the two specified rays.
The start point of the resulting segment will lie onto the first ray 'rayA' and
the end point of the resulting segment will lie onto the second ray 'rayB'.
*/
template < template <typename> class V, typename T >
Line<V, T> ComputeClosestSegmentBetweenRay(const Ray<V, T>& rayA, const Ray<V, T>& rayB)
{
    V<T> r = (rayA.origin - rayB.origin);
    
    /* Compute components for linear equations */
    T c = Dot(rayA.direction, r);
    T f = Dot(rayB.direction, r);
    
    /* The general non-degenerate case starts here */
    T b = Dot(rayA.direction, rayB.direction);
    T denom = T(1) - b*b;

    /* If segments are  not parallel, compute closest point on rayA to rayB */
    T s, t;
    
    if (denom != T(0))
    {
        s = (b*f - c) / denom;
        t = b*s + f;
    }
    else
    {
        s = T(0);
        t = T(0);
    }

    /* Return final line segment */
    return Line<V, T>(rayA.Lerp(s), rayB.Lerp(t));
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================