/*
 * Vector3 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VECTOR3_H__
#define __FORK_VECTOR3_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/DefaultMathOperators.h"
#include "Math/Core/Arithmetic/VectorArithmetic.h"
#include "Math/Core/Vector2.h"


namespace Fork
{

namespace Math
{


/**
Vector3 class with components x, y and z.
\ingroup math_core
*/
template <typename T> class Vector3
{
    
    public:
        
        //! Number of components: 3.
        static const size_t num = 3;

        Vector3() = default;
        explicit Vector3(const T& size) :
            x{ size },
            y{ size },
            z{ size }
        {
        }
        Vector3(const T& vX, const T& vY, const T& vZ) :
            x{ vX },
            y{ vY },
            z{ vZ }
        {
        }

        /* === Operators === */

        Vector3<T>& operator += (const Vector3<T>& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3<T>& operator -= (const Vector3<T>& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3<T>& operator *= (const Vector3<T>& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        Vector3<T>& operator /= (const Vector3<T>& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        }

        Vector3<T>& operator *= (const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vector3<T>& operator /= (const T& scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        //! Returns the additive inverse of this vector.
        inline Vector3<T> operator - () const
        {
            return Vector3<T>(-x, -y, -z);
        }

        //! Returns a constant reference to the specified component by index. Index must be 0, 1 or 2.
        inline const T& operator [] (const size_t index) const
        {
            return *((&x) + index);
        }
        //! Returns a reference to the specified component by index. Index must be 0, 1 or 2.
        inline T& operator [] (const size_t index)
        {
            return *((&x) + index);
        }

        /* === Functions === */

        inline T LengthSq() const
        {
            return Math::LengthSq(*this);
        }
        inline T Length() const
        {
            return Math::Length(*this);
        }

        inline Vector3<T>& Resize(const T& length)
        {
            Normalize();
            *this *= length;
            return *this;
        }

        inline Vector3<T>& Normalize()
        {
            Math::Normalize(*this);
            return *this;
        }

        //! Returns a Vector2 with the first two components of this Vector3.
        inline Vector2<T> Vec2() const
        {
            return Vector2<T>(x, y);
        }

        //! Returns this vector as constant raw pointer.
        inline const T* Ptr() const
        {
            return &x;
        }
        //! Returns this vector as raw pointer.
        inline T* Ptr()
        {
            return &x;
        }

        template <typename C> inline Vector3<C> Cast() const
        {
            return Vector3<C>(
                static_cast<C>(x),
                static_cast<C>(y),
                static_cast<C>(z)
            );
        }

        /* === Members === */

        T x = 0, y = 0, z = 0;

};


/**
Point3 is a synonym for Vector3. Actually a point is not the same as a vector,
but in this engine, they are treated equally. Use this synonym whenever you
want to store a point in space and not just a direction. And when you want to
store a direction, use "Vector".
*/
template <typename T> using Point3 = Vector3<T>;


DEFAULT_MATH_BASE_OPERATORS(Vector3)
DEFAULT_SCALAR_OPERATORS(Vector3)
DEFAULT_COMPARE_OPERATORS(Vector3)

DEFAULT_MATH_TYPEDEFS(Vector3)
DEFAULT_MATH_TYPEDEFS(Point3)


} // /namespace Math

} // /namespace Fork


#endif



// ========================