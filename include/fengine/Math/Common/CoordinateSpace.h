/*
 * Cordinate space header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MATH_COMMON_COORDINATE_SPACE_H__
#define __FORK_MATH_COMMON_COORDINATE_SPACE_H__


#include "Math/Geometry/Triangle.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Matrix3.h"
#include "Math/Core/SpherePoint.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/**
Returns the 2D triangle area.
\note This is actually only used inside the "Math::ComputeBarycentricCoordiates" function.
\see Math::ComputeBarycentricCoordiates
*/
template <typename T> inline T ComputeTriangleArea2D(
    const T& x1, const T& y1,
    const T& x2, const T& y2,
    const T& x3, const T& y3)
{
    return (x1 - x2)*(y2 - y3) - (x2 - x3)*(y1 - y2);
}


/**
Returns the barycentric coordinates given by the cartesian point respectively to the given triangle.
\param[in] triangle Specifies the 3D triangle.
\param[in] point Specifies the cartesian point. This point must lie onto the triangle.
\return Barycentric coordinate of the given point respectively to the given triangle.
*/
template <typename T> Point3<T> ComputeBarycentricCoordiates(
    const Triangle3<T>& triangle, const Point3<T>& point)
{
    const auto& p = point;
    const auto& a = triangle.a;
    const auto& b = triangle.b;
    const auto& c = triangle.c;
    
    /* Unnormalized triangle normal */
    auto m = triangle.NormalSq();
    
    /* Nominators and one-over-denominator for u and v ratios */
    T nu, nv, ood;
    
    /* Absolute components for determining projection plane */
    const T x = std::abs(m.x);
    const T y = std::abs(m.y);
    const T z = std::abs(m.z);
    
    /* Compute areas in plane of largest projection */
    if (x >= y && x >= z)
    {
        /* x is largest, project to the yz plane */
        nu = ComputeTriangleArea2D(p.y, p.z, b.y, b.z, c.y, c.z);   // Area of PBC in yz plane
        nv = ComputeTriangleArea2D(p.y, p.z, c.y, c.z, a.y, a.z);   // Area of PCA in yz plane
        ood = T(1) / m.x;                                           // 1/(2*area of ABC in yz plane)
    }
    else if (y >= x && y >= z)
    {
        /* y is largest, project to the xz plane */
        nu = ComputeTriangleArea2D(p.x, p.z, b.x, b.z, c.x, c.z);
        nv = ComputeTriangleArea2D(p.x, p.z, c.x, c.z, a.x, a.z);
        ood = T(1) / -m.y;
    }
    else
    {
        /* z is largest, project to the xy plane */
        nu = ComputeTriangleArea2D(p.x, p.y, b.x, b.y, c.x, c.y);
        nv = ComputeTriangleArea2D(p.x, p.y, c.x, c.y, a.x, a.y);
        ood = T(1) / m.z;
    }
    
    const T u = nu * ood;
    const T v = nv * ood;
    
    return Point3<T>(u, v, T(1) - u - v);
}

/**
Converts the specified sphercial coordinates into cartesian coordinates.
\param[out] cartesianCoord Specifies the output cartesian coordinate.
\param[in] sphericalCoord Specifies the spherical coordiante which is to be converted. The angles are in radian.
*/
template <typename T> void ConvertToCartesianCoordiantes(Vector3<T>& cartesianCoord, const SpherePoint<T>& sphericalCoord)
{
    const auto sinTheta = std::sin(sphericalCoord.theta);
    
    cartesianCoord.x = sphericalCoord.radius * std::cos(sphericalCoord.phi) * sinTheta;
    cartesianCoord.y = sphericalCoord.radius * std::cos(sphericalCoord.theta);
    cartesianCoord.z = sphericalCoord.radius * std::sin(sphericalCoord.phi) * sinTheta;
}

/**
Converts the specified cartesian coordinates into sphercial coordinates.
\param[in] cartesianCoord Specifies the cartesian coordinate which is to be converted.
\param[out] sphericalCoord Specifies the output spherical coordinate. The angles are in radian.
*/
template <typename T> void ConvertToSphericalCoordiantes(const Vector3<T>& cartesianCoord, SpherePoint<T>& sphericalCoord)
{
    sphericalCoord.radius = cartesianCoord.Length();
    
    if (sphericalCoord.radius > T(0))
    {
        sphericalCoord.theta = std::atan2(cartesianCoord.z, cartesianCoord.x);
        sphericalCoord.phi = std::acos(cartesianCoord.y / sphericalCoord.radius);
    }
    else
    {
        sphericalCoord.theta = T(0);
        sphericalCoord.phi = T(0);
    }
}

/**
Computes the tangent space for the specified triangle.
\param[in] triangleCoords Specifies the triangle 3D coordinates.
\param[in] triangleTexCoords Specifies the triangle texture 2D coordinates.
\return Tangent space as 3x3 matrix. The first column contains the tangent,
the second column contains the bitangent (also called "binormal") and the third column contains the normal vector.
\code
//       / t b n \
//  M := | t b n |
//       \ t b n /
auto tangentSpace = ComputeTangentSpace(triangleCoords, triangleTexCoords);
auto& tangent   = tangentSpace.GetColumn(0);
auto& bitangent = tangentSpace.GetColumn(1);
auto& normal    = tangentSpace.GetColumn(2);
\endcode
*/
template <typename T> Matrix3<T> ComputeTangentSpace(const Triangle3<T>& triangleCoords, const Triangle2<T>& triangleTexCoords)
{
    /* Get triangle vectors */
    auto v1 = triangleCoords.b - triangleCoords.a;
    auto v2 = triangleCoords.c - triangleCoords.a;

    auto st1 = triangleTexCoords.b - triangleTexCoords.a;
    auto st2 = triangleTexCoords.c - triangleTexCoords.a;

    /* Get tangent space vectors */
    Matrix3<T> tangentSpace;
    auto& tangent   = tangentSpace.GetColumn(0);
    auto& bitangent = tangentSpace.GetColumn(1);
    auto& normal    = tangentSpace.GetColumn(2);

    /* Setup tangent space */
    tangent     = (v1 * st2.y) - (v2 * st1.y);
    bitangent   = (v1 * st2.x) - (v2 * st1.x);
    normal      = Cross(v1, v2);

    Normalize(tangent);
    Normalize(bitangent);
    Normalize(normal);

    /* Adjust tangent space */
    if (Dot(Cross(tangent, bitangent), normal) > T(0))
    {
        tangent     = -tangent;
        bitangent   = -bitangent;
    }

    return tangentSpace;
}


} // /namespace Math

} // /namespace Fork


#endif



// ========================