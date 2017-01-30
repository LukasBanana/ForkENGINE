/*
 * Rect header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RECT_H__
#define __FORK_RECT_H__


#include "DefaultMathTypeDefs.h"
#include "DefaultMathOperators.h"
#include "Vector2.h"
#include "Size2.h"

#include <algorithm>


namespace Fork
{

namespace Math
{


//! Base rectangle class with the components: left, top, right and bottom.
template <typename T> class Rect
{
    
    public:
        
        Rect() = default;
        Rect(const T& rcLeft, const T& rcTop, const T& rcRight, const T& rcBottom) :
            left    { rcLeft   },
            top     { rcTop    },
            right   { rcRight  },
            bottom  { rcBottom }
        {
        }
        Rect(const Point2<T>& leftTopPoint, const Size2<T>& size) :
            left    { leftTopPoint.x               },
            top     { leftTopPoint.y               },
            right   { leftTopPoint.x + size.width  },
            bottom  { leftTopPoint.y + size.height }
        {
        }
        Rect(const Point2<T>& leftTopPoint, const Point2<T>& rightBottomPoint) :
            left    { leftTopPoint.x     },
            top     { leftTopPoint.y     },
            right   { rightBottomPoint.x },
            bottom  { rightBottomPoint.y }
        {
        }

        /* === Functions === */

        /**
        Repairs this rectangle, so that 'right' is larger
        than 'left' and 'bottom' is larger than 'top'.
        */
        Rect<T>& Repair()
        {
            if (left > right)
                std::swap(left, right);
            if (top > bottom)
                std::swap(top, bottom);
            return *this;
        }

        //! Returns the center pointer of this rectangle.
        inline Vector2<T> Center() const
        {
            return Vector2<T>(
                (left + right) / 2,
                (top + bottom) / 2
            );
        }

        //! Locates the new center for this rectangle.
        void MakeCenter(const Vector2<T>& point)
        {
            const auto offset = point - Center();
            
            left += offset.x;
            top += offset.y;

            right += offset.x;
            bottom += offset.y;
        }

        //! Returns the left-top point.
        inline Vector2<T> LTPoint() const
        {
            return Vector2<T>(left, top);
        }
        //! Locates the left-top point.
        inline void LocateLTPoint(const Vector2<T>& point)
        {
            left = point.x;
            top = point.y;
        }

        //! Returns the right-bottom point.
        inline Vector2<T> RBPoint() const
        {
            return Vector2<T>(right, bottom);
        }
        //! Locates the right-bottom point.
        inline void LocateRBPoint(const Vector2<T>& point)
        {
            right = point.x;
            bottom = point.y;
        }

        //! Returns the width of this rectangle (right - left).
        inline T Width() const
        {
            return right - left;
        }
        //! Resizes the width of this rectangle (right := left + width).
        inline void ResizeWidth(const T& width)
        {
            right = left + width;
        }

        //! Returns the height of this rectangle (bottom - top).
        inline T Height() const
        {
            return bottom - top;
        }
        //! Resizes the height of this rectangle (bottom := top + height).
        inline void ResizeHeight(const T& height)
        {
            bottom = top + height;
        }

        //! Returns the 2D size of this rectangle: { Width(), Height() }.
        inline Size2<T> Size() const
        {
            return { Width(), Height() };
        }
        /**
        Resizes the width and height of this rectangle.
        \see ResizeWidth
        \see ResizeHeight
        */
        inline void Resize(const Size2<T>& size)
        {
            ResizeWidth(size.width);
            ResizeHeight(size.height);
        }

        //! Returns the rectangle's area (width*height).
        inline T Area() const
        {
            return GetSize().Area();
        }

        /**
        Resizes the rectangle linear by the specified offset:
        { left - offset, top - offset, right + offset, bottom + offset }.
        */
        void ResizeLinear(const T& offset)
        {
            left    -= offset;
            top     -= offset;
            right   += offset;
            bottom  += offset;
        }

        //! Moves this rectangle into the specified direction.
        void Move(const Vector2<T>& direction)
        {
            left    += direction.x;
            top     += direction.y;
            right   += direction.x;
            bottom  += direction.y;
        }

        /**
        Clamps this rectangle to be not larger than the specified rectangle.
        This may decrease the area of this rectangle.
        */
        void ClampMin(const Rect<T>& other)
        {
            left    = std::max( left,   other.left   );
            top     = std::max( top,    other.top    );
            right   = std::min( right,  other.right  );
            bottom  = std::min( bottom, other.bottom );
        }

        /**
        Clamps this rectangle to be not smaller than the specified rectangle.
        This may increase the area of this rectangle.
        */
        void ClampMax(const Rect<T>& other)
        {
            left    = std::min( left,   other.left   );
            top     = std::min( top,    other.top    );
            right   = std::max( right,  other.right  );
            bottom  = std::max( bottom, other.bottom );
        }

        //! Returns true if the rectangle's area is zero.
        inline bool Empty() const
        {
            return Math::Equal(Area(), T(0));
        }

        //! Returns true if the specified rectangle overlaps with this rectangle.
        inline bool Overlap(const Rect<T>& other) const
        {
            return (bottom > other.top && top < other.bottom && right > other.left && left < other.right);
        }

        /**
        Returns true if the specified point overlaps with this rectangle.
        This is the case for: point.x in [left .. right) and point.y in [top .. bottom).
        */
        inline bool Overlap(const Point2<T>& point) const
        {
            return (point.x >= left && point.x < right && point.y >= top && point.y < bottom);
        }

        template <typename C> inline Rect<C> Cast() const
        {
            return Rect<C>(
                static_cast<C>(left),
                static_cast<C>(top),
                static_cast<C>(right),
                static_cast<C>(bottom)
            );
        }

        /* === Members === */

        T left = 0, top = 0, right = 0, bottom = 0;

};


/* --- Global operators --- */

template <typename T> bool operator == (const Rect<T>& a, const Rect<T>& b)
{
    return
        a.left      == b.left   &&
        a.top       == b.top    &&
        a.right     == b.right  &&
        a.bottom    == b.bottom;
}

template <typename T> bool operator != (const Rect<T>& a, const Rect<T>& b)
{
    return !(a == b);
}

DEFAULT_MATH_TYPEDEFS(Rect)


} // /namespace Math

} // /namespace Fork


#endif



// ========================