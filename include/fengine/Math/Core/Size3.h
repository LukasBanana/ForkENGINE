/*
 * Size3 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIZE3_H__
#define __FORK_SIZE3_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Math/Core/Size2.h"
#include "Math/Core/Vector3.h"

#include <algorithm>


namespace Fork
{

namespace Math
{


//! Base 3D size class with the components: width, height and depth.
template <typename T> class Size3
{
    
    public:
        
        //! Number of components: 3.
        static const size_t num = 3;

        Size3() = default;
        explicit Size3(const T& size) :
            width   { size },
            height  { size },
            depth   { size }
        {
        }
        Size3(const T& szWidth, const T& szHeight, const T& szDepth):
            width   { szWidth  },
            height  { szHeight },
            depth   { szDepth  }
        {
        }
        explicit Size3(const Vector3<T>& vec) :
            width   { vec.x },
            height  { vec.y },
            depth   { vec.z }
        {
        }

        /* === Operators === */

        Size3<T>& operator += (const Size3<T>& other)
        {
            width   += other.width;
            height  += other.height;
            depth   += other.depth;
            return *this;
        }

        Size3<T>& operator -= (const Size3<T>& other)
        {
            width   -= other.width;
            height  -= other.height;
            depth   -= other.depth;
            return *this;
        }

        Size3<T>& operator *= (const Size3<T>& other)
        {
            width   *= other.width;
            height  *= other.height;
            depth   *= other.depth;
            return *this;
        }

        Size3<T>& operator /= (const Size3<T>& other)
        {
            width   /= other.width;
            height  /= other.height;
            depth   /= other.depth;
            return *this;
        }

        Size3<T>& operator *= (const T& scalar)
        {
            width   *= scalar;
            height  *= scalar;
            depth   *= scalar;
            return *this;
        }

        Size3<T>& operator /= (const T& scalar)
        {
            width   /= scalar;
            height  /= scalar;
            depth   /= scalar;
            return *this;
        }

        //! Returns a constant reference to the specified component by index. Index must be 0, 1 or 2.
        inline const T& operator [] (const size_t index) const
        {
            return *((&width) + index);
        }
        //! Returns a reference to the specified component by index. Index must be 0, 1 or 2.
        inline T& operator [] (const size_t index)
        {
            return *((&width) + index);
        }

        /* === Functions === */

        //! Returns the size's volume (width*height*depth).
        inline T Volume() const
        {
            return width*height*depth;
        }

        //! Returns the minmal value of the size: min { width, height, depth }.
        inline T Min() const
        {
            return std::min({ width, height, depth });
        }
        //! Returns the maximal value of the size: max { width, height, depth }.
        inline T Max() const
        {
            return std::max({ width, height, depth });
        }

        //! Returns a Size2 with the first two components of this Size3.
        inline Size2<T> Sz2() const
        {
            return Size2<T>(width, height);
        }

        //! \see Math::ScaledSize
        Size3<T> ScaledSize(const Size3<T>& destSize) const;

        template <typename C> inline Size3<C> Cast() const
        {
            return Size3<C>(
                static_cast<C>(width),
                static_cast<C>(height),
                static_cast<C>(depth)
            );
        }

        /* === Members === */

        T width = 0, height = 0, depth = 0;

};


/* --- Global operators --- */

//! Compares the two sizes' components.
template <typename T> bool operator == (const Size3<T>& a, const Size3<T>& b)
{
    return
        a.width == b.width &&
        a.height == b.height &&
        a.depth == b.depth;
}

//! Compares the two sizes' components.
template <typename T> bool operator != (const Size3<T>& a, const Size3<T>& b)
{
    return !(a == b);
}

/**
Compares the two 3D sizes by their volume, not by their components!
\see Size3::Volume
*/
template <typename T> bool operator < (const Size3<T>& a, const Size3<T>& b)
{
    return a.Volume() < b.Volume();
}

/**
Compares the two 3D sizes by their volume, not by their components!
\see Size3::Volume
*/
template <typename T> bool operator <= (const Size3<T>& a, const Size3<T>& b)
{
    return a.Volume() <= b.Volume();
}

/**
Compares the two 3D sizes by their volume, not by their components!
\see Size3::Volume
*/
template <typename T> bool operator > (const Size3<T>& a, const Size3<T>& b)
{
    return a.Volume() > b.Volume();
}

/**
Compares the two 3D sizes by their volume, not by their components!
\see Size3::Volume
*/
template <typename T> bool operator >= (const Size3<T>& a, const Size3<T>& b)
{
    return a.Volume() >= b.Volume();
}


/* --- Automatic definitions --- */

DEFAULT_MATH_BASE_OPERATORS(Size3)
DEFAULT_SCALAR_OPERATORS(Size3)
DEFAULT_MATH_TYPEDEFS(Size3)


} // /namespace Math

} // /namespace Fork


#endif



// ========================