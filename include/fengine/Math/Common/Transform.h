/*
 * Transform functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_COMMON_TRANSFORM_FUNCTIONS_H__
#define __FORK_MATH_COMMON_TRANSFORM_FUNCTIONS_H__


#include "Math/Geometry/Ray.h"
#include "Math/Geometry/Line.h"
#include "Math/Geometry/Plane.h"
#include "Math/Geometry/Triangle.h"
#include "Math/Geometry/OBB.h"
#include "Math/Collision/Intersection.h"


namespace Fork
{

namespace Math
{


/* === Global "Transform" template functions === */

//! Transforms the specified ray by the specified matrix.
template <
    template <typename> class M,
    template <typename> class V,
    typename T>
inline Ray<V, T> Transform(const M<T>& mat, const Ray<V, T>& ray)
{
    return Ray<V, T>(
        mat * ray.point,
        mat.RotateVector(ray.direction)
    );
}

//! Transforms the specified line by the specified matrix.
template <
    template <typename> class M,
    template <typename> class V,
    typename T>
inline Line<V, T> Transform(const M<T>& mat, const Line<V, T>& line)
{
    return Line<V, T>(
        mat * line.start,
        mat * line.end
    );
}

//! Transforms the specified plane by the specified matrix.
template < template <typename> class M, typename T >
inline Plane<T> Transform(const M<T>& mat, const Plane<T>& plane)
{
    const auto member   = mat * plane.MemberPoint();
    const auto invTrans = mat.Inverse().Transpose();
    const auto normal   = invTrans * plane.normal;
    return Plane<T>(normal, Dot(normal, member));
}

//! Transforms the specified OBB by the specified matrix.
template <
    template <typename> class M,
    template <typename> class V,
    typename T>
inline OBB<V, T> Transform(const M<T>& mat, const OBB<V, T>& box)
{
    return obbox3d<T>(
        mat * box.center,
        mat.RotateVector(box.axis.x),
        mat.RotateVector(box.axis.y),
        mat.RotateVector(box.axis.z)
    );
}

//! Transforms the specified intersection by the specified matrix.
template <
    template <typename> class M,
    class V,
    typename T>
inline Intersection<V> Transform(const M<T>& mat, const Intersection<V>& intersection)
{
    return { mat * intersection.point, mat.RotateVector(intersection.normal) };
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================