/*
 * Vector2 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VECTOR2_H__
#define __FORK_VECTOR2_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Arithmetic/VectorArithmetic.h"


namespace Fork
{

namespace Math
{


/**
Vector2 class with components x and y.
\ingroup math_core
*/
template <typename T> class Vector2
{
    
    public:
        
        //! Number of components: 2.
        static const size_t num = 2;

        Vector2() = default;
        explicit Vector2(const T& size) :
            x{ size },
            y{ size }
        {
        }
        Vector2(const T& vX, const T& vY) :
            x{ vX },
            y{ vY }
        {
        }

        /* === Operators === */

        Vector2<T>& operator += (const Vector2<T>& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2<T>& operator -= (const Vector2<T>& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2<T>& operator *= (const Vector2<T>& other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        Vector2<T>& operator /= (const Vector2<T>& other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }

        Vector2<T>& operator *= (const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2<T>& operator /= (const T& scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        //! Returns the additive inverse of this vector.
        inline Vector2<T> operator - () const
        {
            return Vector2<T>(-x, -y);
        }

        //! Returns a constant reference to the specified component by index. Index must be 0 or 1.
        inline const T& operator [] (const size_t index) const
        {
            return *((&x) + index);
        }
        //! Returns a reference to the specified component by index. Index must be 0 or 1.
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

        inline Vector2<T>& Resize(const T& length)
        {
            Normalize();
            *this *= length;
            return *this;
        }

        inline Vector2<T>& Normalize()
        {
            Math::Normalize(*this);
            return *this;
        }

        //! Flips this 2D vector counter clock wise by 90 degrees. The result is { -y, x }.
        inline Vector2<T> FlipCCW() const
        {
            return { -y, x };
        }
        //! Flips this 2D vector clock wise by 90 degrees. The result is { y, -x }.
        inline Vector2<T> FlipCW() const
        {
            return { y, -x };
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

        template <typename C> inline Vector2<C> Cast() const
        {
            return Vector2<C>(
                static_cast<C>(x),
                static_cast<C>(y)
            );
        }

        /* === Members === */

        T x = 0, y = 0;

};


/**
Point2 is a synonym for Vector2. Actually a point is not the same as a vector,
but in this engine, they are treated equally. Use this synonym whenever you
want to store a point in space and not just a direction. And when you want to
store a direction, use "Vector".
*/
template <typename T> using Point2 = Vector2<T>;


DEFAULT_MATH_BASE_OPERATORS(Vector2)
DEFAULT_SCALAR_OPERATORS(Vector2)
DEFAULT_COMPARE_OPERATORS(Vector2)

DEFAULT_MATH_TYPEDEFS(Vector2)
DEFAULT_MATH_TYPEDEFS(Point2)


} // /namespace Math

} // /namespace Fork


#endif



// ========================