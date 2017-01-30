/*
 * Vector4 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VECTOR4_H__
#define __FORK_VECTOR4_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Arithmetic/VectorArithmetic.h"
#include "Vector2.h"
#include "Vector3.h"


namespace Fork
{

namespace Math
{


/**
Vector4 class with components x, y, z and w.
\ingroup math_core
*/
template <typename T> class Vector4
{
    
    public:
        
        //! Number of components: 4.
        static const size_t num = 4;

        Vector4() = default;
        explicit Vector4(const T& size) :
            x{ size },
            y{ size },
            z{ size }
        {
        }
        explicit Vector4(const Vector3<T>& vec) :
            x{ vec.x },
            y{ vec.y },
            z{ vec.z }
        {
        }
        Vector4(const Vector3<T>& vec, const T& vW) :
            x{ vec.x },
            y{ vec.y },
            z{ vec.z },
            w{ vW    }
        {
        }
        Vector4(const T& vX, const T& vY, const T& vZ, const T& vW = T(1)) :
            x{ vX },
            y{ vY },
            z{ vZ },
            w{ vW }
        {
        }

        /* === Operators === */

        Vector4<T>& operator += (const Vector4<T>& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        Vector4<T>& operator -= (const Vector4<T>& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        Vector4<T>& operator *= (const Vector4<T>& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return *this;
        }

        Vector4<T>& operator /= (const Vector4<T>& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            w /= other.w;
            return *this;
        }

        Vector4<T>& operator *= (const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        Vector4<T>& operator /= (const T& scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        //! Returns the additive inverse of this vector.
        inline Vector4<T> operator - () const
        {
            return Vector4<T>(-x, -y, -z, -w);
        }

        //! Returns a constant reference to the specified component by index. Index must be 0, 1, 2 or 3.
        inline const T& operator [] (const size_t index) const
        {
            return *((&x) + index);
        }
        //! Returns a reference to the specified component by index. Index must be 0, 1, 2 or 3.
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

        inline Vector4<T>& Resize(const T& length)
        {
            Normalize();
            *this *= length;
            return *this;
        }

        inline Vector4<T>& Normalize()
        {
            Math::Normalize(*this);
            return *this;
        }

        //! Returns this 4D vector as truncated 2D vector. This will be { x, y }.
        inline Vector2<T> Vec2() const
        {
            return { x, y };
        }
        //! Returns this 4D vector as truncated 3D vector. This will be { x, y, z }.
        inline Vector3<T> Vec3() const
        {
            return { x, y, z };
        }

        /**
        Converts this 4D vector into a 2D vector. This will be { x/w, y/w }.
        RHW stands for "Reciprocal Homogenous W".
        */
        inline Vector2<T> Vec2RHW() const
        {
            return { x/w, y/w };
        }
        /**
        Converts this 4D vector into a 3D vector. This will be { x/w, y/w, z/w }.
        RHW stands for "Reciprocal Homogenous W".
        */
        inline Vector3<T> Vec3RHW() const
        {
            return { x/w, y/w, z/w };
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

        template <typename C> inline Vector4<C> Cast() const
        {
            return Vector4<C>(
                static_cast<C>(x),
                static_cast<C>(y),
                static_cast<C>(z),
                static_cast<C>(w)
            );
        }

        /* === Members === */

        T x = 0, y = 0, z = 0, w = 1;

};


/**
Point4 is a synonym for Vector4. Actually a point is not the same as a vector,
but in this engine, they are treated equally. Use this synonym whenever you
want to store a point in space and not just a direction. And when you want to
store a direction, use "Vector".
*/
template <typename T> using Point4 = Vector4<T>;


DEFAULT_MATH_BASE_OPERATORS(Vector4)
DEFAULT_SCALAR_OPERATORS(Vector4)
DEFAULT_COMPARE_OPERATORS(Vector4)

DEFAULT_MATH_TYPEDEFS(Vector4)
DEFAULT_MATH_TYPEDEFS(Point4)


} // /namespace Math

} // /namespace Fork


#endif



// ========================