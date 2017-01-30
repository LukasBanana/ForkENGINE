/*
 * Math core equal header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_CORE_EQUAL_H__
#define __FORK_MATH_CORE_EQUAL_H__


#include "Math/Core/MathConstants.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/* --- Template 'Equal' function with tolerance parameter --- */

/**
Approximated equality check with tolerance parameter.
For integer types it is not an approximation but a correct comparision (a == b) where the tolerance value is ignored.
*/
template <typename T, typename E = T> inline bool Equal(const T& a, const T& b, const E& tolerance)
{
    return a == b;
}

//! Approximated equality check with default tolerance parameter.
template <typename T, typename E = T> inline bool Equal(const T& a, const T& b)
{
    return Equal(a, b, E(0));
}


/* --- Specialized 'Equal' functions --- */

template <> inline bool Equal<float, float>(const float& a, const float& b, const float& tolerance)
{
    return std::abs(a - b) < tolerance;
}

template <> inline bool Equal<double, double>(const double& a, const double& b, const double& tolerance)
{
    return std::abs(a - b) < tolerance;
}

/*
template <> inline bool Equal<Vector2f, float>(const Vector2f& a, const Vector2f& b, const float& tolerance)
{
    return
        std::abs(a.x - b.x) < tolerance &&
        std::abs(a.y - b.y) < tolerance;
}

template <> inline bool Equal<Vector2d, double>(const Vector2d& a, const Vector2d& b, const double& tolerance)
{
    return
        std::abs(a.x - b.x) < tolerance &&
        std::abs(a.y - b.y) < tolerance;
}


template <> inline bool Equal<Vector3f, float>(const Vector3f& a, const Vector3f& b, const float& tolerance)
{
    return
        std::abs(a.x - b.x) < tolerance &&
        std::abs(a.y - b.y) < tolerance &&
        std::abs(a.z - b.z) < tolerance;
}

template <> inline bool Equal<Vector3d, double>(const Vector3d& a, const Vector3d& b, const double& tolerance)
{
    return
        std::abs(a.x - b.x) < tolerance &&
        std::abs(a.y - b.y) < tolerance &&
        std::abs(a.z - b.z) < tolerance;
}


template <> inline bool Equal<Vector4f, float>(const Vector4f& a, const Vector4f& b, const float& tolerance)
{
    return
        std::abs(a.x - b.x) < tolerance &&
        std::abs(a.y - b.y) < tolerance &&
        std::abs(a.z - b.z) < tolerance &&
        std::abs(a.w - b.w) < tolerance;
}

template <> inline bool Equal<Vector4d, double>(const Vector4d& a, const Vector4d& b, const double& tolerance)
{
    return
        std::abs(a.x - b.x) < tolerance &&
        std::abs(a.y - b.y) < tolerance &&
        std::abs(a.z - b.z) < tolerance &&
        std::abs(a.w - b.w) < tolerance;
}
*/

/* --- Template 'Equal' function with default tolerance parameter (epsilon or epsilon64) --- */

template <> inline bool Equal<float, float>(const float& a, const float& b)
{
    return Equal(a, b, epsilon);
}

template <> inline bool Equal<double, double>(const double& a, const double& b)
{
    return Equal(a, b, epsilon64);
}

/*
template <> inline bool Equal<Vector2f, float>(const Vector2f& a, const Vector2f& b)
{
    return Equal(a, b, epsilon);
}

template <> inline bool Equal<Vector2d, double>(const Vector2d& a, const Vector2d& b)
{
    return Equal(a, b, epsilon);
}


template <> inline bool Equal<Vector3f, float>(const Vector3f& a, const Vector3f& b)
{
    return Equal(a, b, epsilon);
}

template <> inline bool Equal<Vector3d, double>(const Vector3d& a, const Vector3d& b)
{
    return Equal(a, b, epsilon);
}


template <> inline bool Equal<Vector4f, float>(const Vector4f& a, const Vector4f& b)
{
    return Equal(a, b, epsilon);
}

template <> inline bool Equal<Vector4d, double>(const Vector4d& a, const Vector4d& b)
{
    return Equal(a, b, epsilon);
}
*/

} // /namespace Math

} // /namespace Fork


#endif



// ========================