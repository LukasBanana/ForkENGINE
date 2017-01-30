/*
 * Cuboid header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CUBOID_H__
#define __FORK_CUBOID_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Vector3.h"
#include "Size3.h"


namespace Fork
{

namespace Math
{


//! Base cuboid class with the components: left, top, front, right, bottom and back.
template <typename T> class Cuboid
{
    
    public:
        
        Cuboid() = default;
        Cuboid(const T& cbLeft, const T& cbTop, const T& cbFront, const T& cbRight, const T& cbBottom, const T& cbBack) :
            left    { cbLeft   },
            top     { cbTop    },
            front   { cbFront  },
            right   { cbRight  },
            bottom  { cbBottom },
            back    { cbBack   }
        {
        }
        Cuboid(const Point2<T>& leftTopFrontPoint, const Size3<T>& size) :
            left    { leftTopFrontPoint.x               },
            top     { leftTopFrontPoint.y               },
            front   { leftTopFrontPoint.z               },
            right   { leftTopFrontPoint.x + size.width  },
            bottom  { leftTopFrontPoint.y + size.height },
            back    { leftTopFrontPoint.z + size.depth  }
        {
        }
        Cuboid(const Point3<T>& leftTopFrontPoint, const Point3<T>& rightBottomBackPoint) :
            left    { leftTopFrontPoint.x    },
            top     { leftTopFrontPoint.y    },
            front   { leftTopFrontPoint.z    },
            right   { rightBottomBackPoint.x },
            bottom  { rightBottomBackPoint.y },
            back    { rightBottomBackPoint.z }
        {
        }

        /* === Operators === */

        /**
        Returns the specified member field.
        \param[in] index Specifies the member index. Must be in the range [0 .. 5].
        \throws IndexOutOfBoundsException If the index is out of bounds.
        */
        const T& operator [] (size_t index) const
        {
            switch (index)
            {
                case 0: return left;
                case 1: return top;
                case 2: return front;
                case 3: return right;
                case 4: return bottom;
                case 5: return back;
                default:
                    throw IndexOutOfBoundsException(__FUNCTION__, index);
            }
            return left;
        }

        /**
        Returns a reference to the specified member field.
        \param[in] index Specifies the member index. Must be in the range [0 .. 5].
        \throws IndexOutOfBoundsException If the index is out of bounds.
        */
        T& operator [] (size_t index)
        {
            switch (index)
            {
                case 0: return left;
                case 1: return top;
                case 2: return front;
                case 3: return right;
                case 4: return bottom;
                case 5: return back;
                default:
                    throw IndexOutOfBoundsException(__FUNCTION__, index);
            }
            return left;
        }

        /* === Functions === */

        Cuboid<T>& Repair()
        {
            if (left > right)
                std::swap(left, right);
            if (top > bottom)
                std::swap(top, bottom);
            if (front > back)
                std::swap(front, back);
            return *this;
        }

        inline Vector3<T> Center() const
        {
            return Vector3<T>(
                (left + right) / 2,
                (top + bottom) / 2,
                (front + back) / 2
            );
        }

        void MakeCenter(const Vector3<T>& point)
        {
            const auto offset = point - Center();
            
            left    += offset.x;
            top     += offset.y;
            front   += offset.z;

            right   += offset.x;
            bottom  += offset.y;
            back    += offset.z;
        }

        inline Vector3<T> LTFPoint() const
        {
            return Vector3<T>(left, top, front);
        }
        inline void LocateLTFPoint(const Vector3<T>& point)
        {
            left    = point.x;
            top     = point.y;
            front   = point.z;
        }

        //! Returns right-bottom-back point.
        inline Vector3<T> RBBPoint() const
        {
            return Vector3<T>(right, bottom, back);
        }
        //! Locates the right-bottom-back point.
        inline void LocateRBBPoint(const Vector3<T>& point)
        {
            right   = point.x;
            bottom  = point.y;
            back    = point.z;
        }

        inline T Width() const
        {
            return right - left;
        }
        inline void ResizeWidth(const T& width)
        {
            right = left + width;
        }

        inline T Height() const
        {
            return bottom - top;
        }
        inline void ResizeHeight(const T& height)
        {
            bottom = top + height;
        }

        inline T Depth() const
        {
            return back - front;
        }
        inline void ResizeDepth(const T& depth)
        {
            back = front + depth;
        }

        inline Size3<T> Size() const
        {
            return Size3<T>(Width(), Height(), Depth());
        }
        inline void Resize(const Size3<T>& size)
        {
            ResizeWidth (size.width );
            ResizeHeight(size.height);
            ResizeDepth (size.depth );
        }

        //! Returns the cuboid's volume (width*height*depth).
        inline T Volume() const
        {
            return GetSize().Volume();
        }

        //! Returns the cuboid's diagonal length (sqrt(width^2 + height^2 + depth^2)).
        inline T Diagonal() const
        {
            return std::sqrt(Sq(Width()), Sq(Height()), Sq(Depth()));
        }

        /**
        Resizes the cuboid linear by the specified offset:
        { left - offset, top - offset, front - offset, right + offset, bottom + offset, back + offset }.
        */
        void ResizeLinear(const T& offset)
        {
            left    -= offset;
            top     -= offset;
            front   -= offset;
            right   += offset;
            bottom  += offset;
            back    += offset;
        }

        //! Returns true if the cuboids's volume is zero.
        inline bool Empty() const
        {
            return Math::Equal(Volume(), T(0));
        }

        inline bool Overlap(const Cuboid<T>& other) const
        {
            return
                bottom > other.top && top < other.bottom &&
                right > other.left && left < other.right &&
                back > other.front && front < other.back;
        }

        inline bool Overlap(const Vector3<T>& point) const
        {
            return
                point.x >= left && point.x < right &&
                point.y >= top && point.y < bottom &&
                point.z >= front && point.z < back;
        }

        template <typename C> inline Cuboid<C> Cast() const
        {
            return Cuboid<C>(
                static_cast<C>(left),
                static_cast<C>(top),
                static_cast<C>(front),
                static_cast<C>(right),
                static_cast<C>(bottom),
                static_cast<C>(back)
            );
        }

        /* === Members === */

        T left = 0, top = 0, front = 0, right = 0, bottom = 0, back = 0;

};


/* --- Global operators --- */

template <typename T> bool operator == (const Cuboid<T>& a, const Cuboid<T>& b)
{
    return
        a.left      == b.left   &&
        a.top       == b.top    &&
        a.front     == b.front  &&
        a.right     == b.right  &&
        a.bottom    == b.bottom &&
        a.back      == b.back;
}

template <typename T> bool operator != (const Cuboid<T>& a, const Cuboid<T>& b)
{
    return !(a == b);
}

DEFAULT_MATH_TYPEDEFS(Cuboid)


} // /namespace Math

} // /namespace Fork


#endif



// ========================