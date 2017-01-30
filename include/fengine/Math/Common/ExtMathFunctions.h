/*
 * Extended math function header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_COMMON_EXT_MATH_FUNCTIONS_H__
#define __FORK_MATH_COMMON_EXT_MATH_FUNCTIONS_H__


#include "Math/Common/AxisDirections.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Size3.h"
#include "Math/Core/Quaternion.h"
#include "Math/Core/Arithmetic/MatrixArithmetic.h"
#include "Math/Core/MathConstants.h"
#include "Math/Geometry/AABB.h"
#include "Math/Geometry/Line.h"
#include "Math/Geometry/Ray.h"

#include <cmath>
#include <array>


namespace Fork
{

namespace Math
{


/* === Global template functions === */

/**
Computes a bernstein interpolation. This can be used for NURBS (Non-Uniform-Ratial-B-Spline) generation.
\tparam n Specifies the number of control points.
\tparam V Specifies the vector type. This should be Vector2, Vector3 or Vector4.
\tparam T Specifies the data type. This should be float or double.
\param[in] t Specifies the interpolation factor. This should be in the range [0.0 .. 1.0].
\param[in] points Specifies the array of control points.
*/
template <
    unsigned int n,
    template <typename> class V,
    typename T >
V<T> ComputeBernsteinInterpolation(const T& t, const std::array<V<T>, n>& points)
{
    V<T> result;

    for (unsigned int i = 0; i < n; ++i)
        result += points[n - i - 1] * ComputeBernsteinPolynomial<T>(i, n - 1, t);

    return result;
}

/**
Computes the dominant axis of the specified vector.
\tparam T Specifies the vector data type. This type must has the components x, y and z (e.g. Vector3 or Vector4).
\param[in] vec Specifies the vector (with at least three components).
\return The dominant axis type. This is a value of the "AxisDirections" enumeration.
\see Math::AxisDirections
\ingroup math_core
*/
template <typename T> AxisDirections DominantAxis(const T& vec)
{
    /* Get absolute vector */
    const auto xAbs = std::abs(vec.x);
    const auto yAbs = std::abs(vec.y);
    const auto zAbs = std::abs(vec.z);
    
    /* Find dominant axis */
    if (xAbs >= yAbs && xAbs >= zAbs)
        return (vec.x > 0 ? AxisDirections::XPositive : AxisDirections::XNegative);
    if (yAbs >= xAbs && yAbs >= zAbs)
        return (vec.y > 0 ? AxisDirections::YPositive : AxisDirections::YNegative);
    return (vec.z > 0 ? AxisDirections::ZPositive : AxisDirections::ZNegative);
}

//! Checks whether the specified point is between the two other points a and b or not.
template <typename T> bool IsBetweenPoints(const T& point, const T& a, const T& b)
{
    const auto lenSq = LengthSq(b - a);
    const auto distA = DistanceSq(point, a);
    const auto distB = DistanceSq(point, b);
    return distA <= lenSq && distB <= lenSq;
}

//! Checks whether the specified point is on the same side as the triangle's normal described with a, b and c.
template <typename T> bool IsPointOnSameSide(const T& point, const T& a, const T& b, const T& c)
{
    const auto diff = c - b;
    const auto p = Cross(diff, point - b);
    const auto q = Cross(diff, a - b);
    return Dot(p, q) >= 0;
}

//! Checks whether the specified point is inside the specified triangle.
template <typename T, typename V> bool IsPointInsideTriangle(const T& triangle, const V& point)
{
    return
        IsPointOnSameSide(point, triangle.a, triangle.b, triangle.c) &&
        IsPointOnSameSide(point, triangle.b, triangle.a, triangle.c) &&
        IsPointOnSameSide(point, triangle.c, triangle.a, triangle.b);
}

/**
Returns true if the specified 2D triangle faces towards the viewport.
\tparam T Specifies the triangle type. This type must has three components a, b and c.
And the type of these components must has two components x and y.
*/
template <typename T> bool IsTriangle2DFrontFacing(const T& triangle)
{
    return (
        (triangle.b.x - triangle.a.x)*(triangle.c.y - triangle.a.y) -
        (triangle.b.y - triangle.a.y)*(triangle.c.x - triangle.a.x)
    ) >= 0;
}

//! Computes the area of the specified 2D AABB.
template <typename T> inline T ComputeAABBArea(const AABB2<T>& aabb)
{
    return aabb.Size().Area();
}

//! Computes the volume of the specified 3D AABB.
template <typename T> inline T ComputeAABBVolume(const AABB3<T>& aabb)
{
    return aabb.Size().Volume();
}

/**
Converts the rotation of the specified matrix into a quaternion rotation.
\tparam M Specifies matrix type. The matrix must has at least 3x3 entries and must overload the
"T operator (size_t column, size_t row) const" operator. This could be "Math::Matrix3" or "Math::Matrix4".
\tparam Q Specifies the quaternion type. This must be suitable for the "Normalize" function.
The quaternion must have the components x, y, z and w. This could be "Math::Quaternion".
\param[in] mat Specifies the matrix from which the rotation will be extracted.
\param[out] quaternion Specifies the resulting quaternion.
\see Math::Matrix3
\see Math::Matrix4
\see Math::Quaternion
*/
template < template <typename> class M, template <typename> class Q, typename T> void ConvertMatrixToQuaternion(
    M<T> mat, Q<T>& quaternion)
{
    /* Make sure the matrix has an identitiy scaling */
    mat.SetScale({ 1, 1, 1 });

    /* Only get the trace of the 3x3 upper left matrix */
    const T trace = mat(0, 0) + mat(1, 1) + mat(2, 2) + T(1);
    
    if (trace > T(0))
    {
        const T s = T(2) * std::sqrt(trace);
        quaternion.x = (mat(1, 2) - mat(2, 1)) / s;
        quaternion.y = (mat(2, 0) - mat(0, 2)) / s;
        quaternion.z = (mat(0, 1) - mat(1, 0)) / s;
        quaternion.w = T(0.25) * s;
    }
    else
    {
        if (mat(0, 0) > mat(1, 1) && mat(0, 0) > mat(2, 2))
        {
            const T s = T(2) * std::sqrt(T(1) + mat(0, 0) - mat(1, 1) - mat(2, 2));
            quaternion.x = T(0.25) * s;
            quaternion.y = (mat(1, 0) + mat(0, 1) ) / s;
            quaternion.z = (mat(0, 2) + mat(2, 0) ) / s;
            quaternion.w = (mat(1, 2) - mat(2, 1) ) / s;
        }
        else if (mat(1, 1) > mat(2, 2))
        {
            const T s = T(2) * std::sqrt(T(1) + mat(1, 1) - mat(0, 0) - mat(2, 2));
            quaternion.x = (mat(1, 0) + mat(0, 1) ) / s;
            quaternion.y = T(0.25) * s;
            quaternion.z = (mat(2, 1) + mat(1, 2) ) / s;
            quaternion.w = (mat(2, 0) - mat(0, 2) ) / s;
        }
        else
        {
            const T s = T(2) * std::sqrt(T(1) + mat(2, 2) - mat(0, 0) - mat(1, 1));
            quaternion.x = (mat(2, 0) + mat(0, 2) ) / s;
            quaternion.y = (mat(2, 1) + mat(1, 2) ) / s;
            quaternion.z = T(0.25) * s;
            quaternion.w = (mat(0, 1) - mat(1, 0) ) / s;
        }
    }

    Normalize(quaternion);
}

/**
Converts the rotation of the specified quaternion into a matrix rotation.
\tparam Q Specifies the quaternion type. The quaternion must have the
components x, y, z and w. This could be "Math::Quaternion".
\tparam M Specifies matrix type. The matrix must has at least 3x3 entries and must overload the
"T& operator (size_t column, size_t row)" operator. This could be "Math::Matrix3" or "Math::Matrix4".
\see Math::Matrix3
\see Math::Matrix4
\see Math::Quaternion
*/
template < template <typename> class Q, template <typename> class M, typename T> void ConvertQuaternionToMatrix(
    const Q<T>& quaternion, M<T>& mat)
{
    const auto& x = quaternion.x;
    const auto& y = quaternion.y;
    const auto& z = quaternion.z;
    const auto& w = quaternion.w;

    mat(0, 0) = T(1) - T(2)*y*y - T(2)*z*z;
    mat(0, 1) =        T(2)*x*y + T(2)*z*w;
    mat(0, 2) =        T(2)*x*z - T(2)*y*w;

    mat(1, 0) =        T(2)*x*y - T(2)*z*w;
    mat(1, 1) = T(1) - T(2)*x*x - T(2)*z*z;
    mat(1, 2) =        T(2)*z*y + T(2)*x*w;

    mat(2, 0) =        T(2)*x*z + T(2)*y*w;
    mat(2, 1) =        T(2)*z*y - T(2)*x*w;
    mat(2, 2) = T(1) - T(2)*x*x - T(2)*y*y;
}

/**
Converts the rotation of the specified quaternion into a transposed matrix rotation.
\tparam Q Specifies the quaternion type. The quaternion must have the
components x, y, z and w. This could be "Math::Quaternion".
\tparam M Specifies matrix type. The matrix must has at least 3x3 entries and must overload the
"T& operator (size_t column, size_t row)" operator. This could be "Math::Matrix3" or "Math::Matrix4".
\see Math::Matrix3
\see Math::Matrix4
\see Math::Quaternion
*/
template < template <typename> class Q, template <typename> class M, typename T> void ConvertQuaternionToMatrixTransposed(
    const Q<T>& quaternion, M<T>& mat)
{
    const auto& x = quaternion.x;
    const auto& y = quaternion.y;
    const auto& z = quaternion.z;
    const auto& w = quaternion.w;

    mat(0, 0) = T(1) - T(2)*y*y - T(2)*z*z;
    mat(1, 0) =        T(2)*x*y + T(2)*z*w;
    mat(2, 0) =        T(2)*x*z - T(2)*y*w;

    mat(0, 1) =        T(2)*x*y - T(2)*z*w;
    mat(1, 1) = T(1) - T(2)*x*x - T(2)*z*z;
    mat(2, 1) =        T(2)*z*y + T(2)*x*w;

    mat(0, 2) =        T(2)*x*z + T(2)*y*w;
    mat(1, 2) =        T(2)*z*y - T(2)*x*w;
    mat(2, 2) = T(1) - T(2)*x*x - T(2)*y*y;
}

/**
Returns the number of MIP-levels used for the specified size.
\param[in] imageMaxSizeComponent Specifies the maximal size component.
\code
// Example:
Math::NumMIPLevels(Math::Size3i(width, height, depth).Max())
\endcode
*/
template <typename T> T NumMIPLevels(const T& imageMaxSizeComponent)
{
    return imageMaxSizeComponent > 0 ? static_cast<T>(std::floor(std::log2(imageMaxSizeComponent))) + 1 : 0;
}

/**
Returns the number of MIP-level used for the sepcified size.
This is equivalent to the following code:
\code
Math::NumMIPLevels(imageSize.Max());
\endcode
\param[in] imageSize Specifies the image size.
*/
template <template <typename> class Sz, typename T> T NumMIPLevelsBySize(const Sz<T>& imageSize)
{
    return NumMIPLevels<T>(imageSize.Max());
}

/**
Returns the original size in a scaled form, so that it fits into the destination size, but still has the same aspect ratio.
\tparam Sz Specifies the size type. This could be Size2 or Size3.
\tparam T Specifies the data type. This should be float or double.
\param[in] srcSize Specifies the source size which is to be scaled.
\param[in] destSize Specifies the destination size.
\return New scaled size which fits into the destination size and with the same aspect ratio as the source size.
\see Size2
\see Size3
*/
template <template <typename> class Sz, typename T> Sz<T> ScaledSize(const Sz<T>& srcSize, const Sz<T>& destSize)
{
    static_assert(!std::numeric_limits<T>::is_integer, "This function requires a non-integer type");

    /* Get size scaling */
    Sz<T> scale;
    for (size_t i = 0; i < Sz<T>::num; ++i)
        scale[i] = destSize[i] / srcSize[i];

    const T minScale = scale.Min();

    /* Setup scaled size */
    Sz<T> scaledSize;
    for (size_t i = 0; i < Sz<T>::num; ++i)
        scaledSize[i] = std::min(srcSize[i] * minScale, destSize[i]);

    return scaledSize;
}

/**
Computes a gaussian value for the specified (1-dimensional) position with the specified mean and variance.
\param[in] pos Specifies the 1-dimensional position for which the gaussian value is to be computed.
\param[in] mean Specifies the mean value of the gaussian (or rather normal-) distribution.
\param[in] variance Specifies the variance (sigma ^ 2). If you want to set the standard-deviation, set this parameter to 'Math::Sq(sigma)'.
*/
template <typename T> T GaussianValue(const T& pos, const T& mean, const T& variance)
{
    return std::exp(-Sq(pos - mean) / (T(2) * variance)) / std::sqrt(T(2) * Math::PI<T>() * variance);
}

/**
Computes a gaussian value with the standard normal distribution (mean = 0, variance = 1).
\see GaussianValue(const T&, const T&, const T&)
*/
template <typename T> T GaussianValue(const T& pos)
{
    return std::exp(-Sq(pos) / T(2)) / std::sqrt(T(2) * Math::PI<T>());
}


/* === Further "Quaternion" functions === */

template <typename T> Quaternion<T>::Quaternion(const Matrix3<T>& mat) :
    x{ 0 },
    y{ 0 },
    z{ 0 },
    w{ 0 }
{
    Math::ConvertMatrixToQuaternion(mat, *this);
}
template <typename T> Quaternion<T>::Quaternion(const Matrix4<T>& mat) :
    x{ 0 },
    y{ 0 },
    z{ 0 },
    w{ 0 }
{
    Math::ConvertMatrixToQuaternion(mat, *this);
}

template <typename T> inline Matrix3<T> Quaternion<T>::Mat3() const
{
    Matrix3<T> mat;
    Math::ConvertQuaternionToMatrix(*this, mat);
    return mat;
}
template <typename T> inline Matrix3<T> Quaternion<T>::Mat3Transposed() const
{
    Matrix3<T> mat;
    Math::ConvertQuaternionToMatrixTransposed(*this, mat);
    return mat;
}

template <typename T> inline Matrix4<T> Quaternion<T>::Mat4() const
{
    Matrix4<T> mat;
    Math::ConvertQuaternionToMatrix(*this, mat);
    return mat;
}
template <typename T> inline Matrix4<T> Quaternion<T>::Mat4Transposed() const
{
    Matrix4<T> mat;
    Math::ConvertQuaternionToMatrixTransposed(*this, mat);
    return mat;
}


/* === Further "Line" functions === */

template <template <typename> class V, typename T>
Line<V, T>::Line(const Ray<V, T>& ray, const T& length) :
    start   { ray.origin       },
    end     { ray.Lerp(length) }
{
}

template <template <typename> class V, typename T>
inline bool Line<V, T>::IsBetweenPoints(const VecT& point) const
{
    return Math::IsBetweenPoints(point, start, end);
}


/* === Further "Ray" functions === */

template <template <typename> class V, typename T> Ray<V, T>::Ray(const Line<V, T>& line) :
    origin      { line.start       },
    direction   { line.Direction() }
{
    direction.Normalize();
}


/* === Further "Size2" functions === */

template <typename T> Size2<T> Size2<T>::ScaledSize(const Size2<T>& destSize) const
{
    return Math::ScaledSize(*this, destSize);
}


/* === Further "Size3" functions === */

template <typename T> Size3<T> Size3<T>::ScaledSize(const Size3<T>& destSize) const
{
    return Math::ScaledSize(*this, destSize);
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================