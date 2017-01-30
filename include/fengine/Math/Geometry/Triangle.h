/*
 * Triangle header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TRIANGLE_H__
#define __FORK_TRIANGLE_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/Arithmetic/VectorArithmetic.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"

#include <cmath>


namespace Fork
{

namespace Math
{


/**
Triangle base class with components a, b and c.
\tparam V Specifies the vector type. This type must be suitable for the "Math::Cross" function.
\see Math::Cross
*/
//template < template <typename> class V, typename T > class Triangle // (problem with MSVC12 compiler)
template <typename VecT, typename T> class Triangle
{
    
    public:
        
        //! Vector class type.
        //typedef V<T> VecT;

        Triangle() = default;
        Triangle(const VecT& pA, const VecT& pB, const VecT& pC) :
            a{ pA },
            b{ pB },
            c{ pC }
        {
        }

        /* === Operators === */

        inline const VecT& operator [] (const size_t index) const
        {
            return *((&a) + index);
        }
        inline VecT& operator [] (const size_t index)
        {
            return *((&a) + index);
        }

        /* === Functions === */

        //! Returns the squared (or rather unnormalized) normal vector of this triangle.
        inline VecT NormalSq() const
        {
            return Math::Cross(b - a, c - a);
        }

        //! Returns the (normalized) normal vector of this triangle.
        inline VecT Normal() const
        {
            auto normal = NormalSq();
            Math::Normalize(normal);
            return normal;
        }

        //! Returns the center of this triangle.
        inline VecT Center() const
        {
            return VecT((a + b + c) / 3);
        }

        //! Returns the triangle's area.
        inline T Area() const
        {
            return NormalSq().Length() / 2;
        }

        //! Flips the triangle, or rather swaps the components a and c.
        inline void MakeFlip()
        {
            std::swap(a, c);
        }

        //inline Triangle<V, T> Flip() const
        inline Triangle<VecT, T> Flip() const
        {
            auto result = *this;
            result.MakeFlip();
            return result;
        }

        /* === Members === */

        VecT a, b, c;

};


/*template <typename T> using Triangle2 = Triangle<Point2, T>; //<-- maybe a problem because of "Math::Cross"!!!
template <typename T> using Triangle3 = Triangle<Point3, T>;
template <typename T> using Triangle4 = Triangle<Point4, T>;*/
template <typename T> using Triangle2 = Triangle<Point2<T>, T>; //<-- maybe a problem because of "Math::Cross"!!!
template <typename T> using Triangle3 = Triangle<Point3<T>, T>;
template <typename T> using Triangle4 = Triangle<Point4<T>, T>;

DEFAULT_MATH_TYPEDEFS(Triangle2)
DEFAULT_MATH_TYPEDEFS(Triangle3)
DEFAULT_MATH_TYPEDEFS(Triangle4)


} // /namespace Math

} // /namespace Fork


#endif



// ========================