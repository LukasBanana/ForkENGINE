/*
 * Size2 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIZE2_H__
#define __FORK_SIZE2_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Math/Core/Vector2.h"

#include <algorithm>


namespace Fork
{

namespace Math
{


//! Base 2D size class with the components: width and height.
template <typename T> class Size2
{
    
    public:
        
        //! Number of components: 2.
        static const size_t num = 2;

        Size2() = default;
        explicit Size2(const T& size) :
            width   { size },
            height  { size }
        {
        }
        Size2(const T& szWidth, const T& szHeight):
            width   { szWidth  },
            height  { szHeight }
        {
        }
        explicit Size2(const Vector2<T>& vec) :
            width   { vec.x },
            height  { vec.y }
        {
        }

        /* === Operators === */

        Size2<T>& operator += (const Size2<T>& other)
        {
            width   += other.width;
            height  += other.height;
            return *this;
        }

        Size2<T>& operator -= (const Size2<T>& other)
        {
            width   -= other.width;
            height  -= other.height;
            return *this;
        }

        Size2<T>& operator *= (const Size2<T>& other)
        {
            width   *= other.width;
            height  *= other.height;
            return *this;
        }

        Size2<T>& operator /= (const Size2<T>& other)
        {
            width   /= other.width;
            height  /= other.height;
            return *this;
        }

        Size2<T>& operator *= (const T& scalar)
        {
            width   *= scalar;
            height  *= scalar;
            return *this;
        }

        Size2<T>& operator /= (const T& scalar)
        {
            width   /= scalar;
            height  /= scalar;
            return *this;
        }

        //! Returns a constant reference to the specified component by index. Index must be 0 or 1.
        inline const T& operator [] (const size_t index) const
        {
            return *((&width) + index);
        }
        //! Returns a reference to the specified component by index. Index must be 0 or 1.
        inline T& operator [] (const size_t index)
        {
            return *((&width) + index);
        }

        /* === Functions === */

        //! Returns the size's area (width*height).
        inline T Area() const
        {
            return width*height;
        }

        //! Returns the minmal value of the size: min { width, height }.
        inline T Min() const
        {
            return std::min(width, height);
        }
        //! Returns the maximal value of the size: max { width, height }.
        inline T Max() const
        {
            return std::max(width, height);
        }

        //! \see Math::ScaledSize
        Size2<T> ScaledSize(const Size2<T>& destSize) const;

        template <typename C> inline Size2<C> Cast() const
        {
            return Size2<C>(
                static_cast<C>(width),
                static_cast<C>(height)
            );
        }

        /* === Members === */

        T width = 0, height = 0;

};


/* --- Global operators --- */

//! Compares the two sizes' components.
template <typename T> bool operator == (const Size2<T>& a, const Size2<T>& b)
{
    return
        a.width == b.width &&
        a.height == b.height;
}

//! Compares the two sizes' components.
template <typename T> bool operator != (const Size2<T>& a, const Size2<T>& b)
{
    return !(a == b);
}

/**
Compares the two 2D sizes by their area, not by their components!
\see Size2::Area
*/
template <typename T> bool operator < (const Size2<T>& a, const Size2<T>& b)
{
    return a.Area() < b.Area();
}

/**
Compares the two 2D sizes by their area, not by their components!
\see Size2::Area
*/
template <typename T> bool operator <= (const Size2<T>& a, const Size2<T>& b)
{
    return a.Area() <= b.Area();
}

/**
Compares the two 2D sizes by their area, not by their components!
\see Size2::Area
*/
template <typename T> bool operator > (const Size2<T>& a, const Size2<T>& b)
{
    return a.Area() > b.Area();
}

/**
Compares the two 2D sizes by their area, not by their components!
\see Size2::Area
*/
template <typename T> bool operator >= (const Size2<T>& a, const Size2<T>& b)
{
    return a.Area() >= b.Area();
}


/* --- Automatic definitions --- */

DEFAULT_MATH_BASE_OPERATORS(Size2)
DEFAULT_SCALAR_OPERATORS(Size2)
DEFAULT_MATH_TYPEDEFS(Size2)


} // /namespace Math

} // /namespace Fork


#endif



// ========================