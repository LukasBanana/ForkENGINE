/*
 * AABB collision functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_AABB_COLLISIONS_H__
#define __FORK_MATH_AABB_COLLISIONS_H__


#include "Math/Geometry/Line.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/AABB.h"
#include "Math/Core/MathConstants.h"
#include "Math/Core/BaseMath.h"

#include <algorithm>
#include <limits>


namespace Fork
{

namespace Math
{


/**
Computes the intersection linear-interpolation factor with the specified ray and AABB.
\param[in] box Specifies the axis-aligned bounding box against the intersection test is to be done.
\param[in] ray Specifies the ray whose intersection with the box is to be computed.
\param[out] lerp Specifies the resulting linear-interpolation factor.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool ComputeIntersectionLerpWithAABB(
    const AABB3<T>& box, const Ray3<T>& ray, T& lerp)
{
    T tmin = T(0);
    T tmax = std::numeric_limits<T>::max();

    /* Loop for all three slabs */
    for (int i = 0; i < 3; ++i)
    {
        if (std::abs(ray.direction[i]) < Epsilon<T>())
        {
            /* Ray is parallel to slab. No hit if origin not within slab */
            if (ray.origin[i] < box.min[i] || ray.origin[i] > box.max[i])
                return false;
        }
        else
        {
            /* Compute intersection t value of ray with near and far plane of slab */
            T ood = T(1) / ray.direction[i];
            T t1 = (box.min[i] - ray.origin[i]) * ood;
            T t2 = (box.max[i] - ray.origin[i]) * ood;

            /* Make t1 be intersection with near plane, t2 with far plane */
            if (t1 > t2)
                std::swap(t1, t2);

            /* Compute the intersection of slab intersection intervals */
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);

            /* Exit with no collision as soon as slab intersection becomes empty */
            if (tmin > tmax)
                return false;
        }
    }

    /* Return intersection interpolation factor */
    lerp = tmin;

    return true;
}

/**
Makes an intersection test with the specified line and AABB.
\param[in] box Specifies the axis-aligned bounding box against the intersection test is to be done.
\param[in] line Specifies the line whose intersection with the box is to be tested.
\param[out] intersection Specifies the resulting intersection point.
This will only be written if an intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithAABB(
    const AABB3<T>& box, const Line3<T>& line, Vector3<T>& intersection)
{
    T lerp = T(0);
    Ray3<T> ray(line);

    if (!ComputeIntersectionLerpWithAABB(box, ray, lerp))
        return false;

    /* Check if intersection is outside line */
    lerp /= line.Length();

    if (lerp < T(0) || lerp > T(1))
        return false;

    /* Compute the intersection point */
    intersection = ray.Lerp(lerp);

    return true;
}

/**
Makes an intersection test with the specified line and AABB.
\param[in] box Specifies the axis-aligned bounding box against the intersection test is to be done.
\param[in] line Specifies the line whose intersection with the box is to be tested.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithAABB(const AABB3<T>& box, const Line3<T>& line)
{
    T lerp = T(0);
    Ray3<T> ray(line);

    if (!ComputeIntersectionLerpWithAABB(box, ray, lerp))
        return false;

    /* Check if intersection is outside line */
    lerp /= line.Length();

    return lerp >= T(0) && lerp <= T(1);
}

/**
Makes an intersection test with the specified line and AABB.
\param[in] box Specifies the axis-aligned bounding box against the intersection test is to be done.
\param[in] ray Specifies the ray whose intersection with the box is to be tested.
\param[out] intersection Specifies the resulting intersection point.
This will only be written if an intersection occurs.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithAABB(
    const AABB3<T>& box, const Ray3<T>& ray, Vector3<T>& intersection)
{
    T lerp = T(0);
    if (!ComputeIntersectionLerpWithAABB(box, ray, lerp))
        return false;

    if (lerp < T(0))
        return false;

    /* Compute the intersection point */
    intersection = ray.Lerp(lerp);

    return true;
}

/**
Makes an intersection test with the specified line and AABB.
\param[in] box Specifies the axis-aligned bounding box against the intersection test is to be done.
\param[in] ray Specifies the ray whose intersection with the box is to be tested.
\return True if an intersection occurs, otherwise false.
*/
template <typename T> bool CheckIntersectionWithAABB(const AABB3<T>& box, const Ray3<T>& ray)
{
    T lerp = T(0);
    if (!ComputeIntersectionLerpWithAABB(box, ray, lerp))
        return false;
    return lerp >= T(0);
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================