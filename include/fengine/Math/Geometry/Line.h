/*
 * Cylinder header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LINE_H__
#define __FORK_LINE_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"


namespace Fork
{

namespace Math
{


template <template <typename> class V, typename T> class Ray;

/**
Line segment class.
\tparam V Specifies the vector type. This should be Vector2, Vector3 or Vector4.
\tparam T Specifies the data type. This should be float or double.
*/
template <template <typename> class V, typename T> class Line
{
    
    public:
        
        typedef V<T> VecT;

        Line() = default;
        Line(const VecT& lnStart, const VecT& lnEnd) :
            start   { lnStart },
            end     { lnEnd   }
        {
        }
        explicit Line(const Ray<V, T>& ray, const T& length);

        /* === Functions === */

        //! Returns the center of this triangle.
        inline VecT Center() const
        {
            return VecT((start + end) / 2);
        }

        //! Returns the (un-normalized) direction vector of this line (end - start).
        inline VecT Direction() const
        {
            VecT dir = end;
            dir -= start;
            return dir;
        }

        //! Returns the squared length of this line.
        inline T LengthSq() const
        {
            return Direction().LengthSq();
        }

        //! Returns the length of this line.
        inline T Length() const
        {
            return Direction().Length();
        }

        //! Returns true if the specified point is between the start- and end points of the line.
        inline bool IsBetweenPoints(const VecT& point) const;

        //! Flips the line, or rather swaps the components start and end.
        inline void MakeFlip()
        {
            std::swap(start, end);
        }

        inline Line<V, T> Flip() const
        {
            auto result = *this;
            result.MakeFlip();
            return result;
        }

        /**
        Returns a linear-interpolation ('lerp') with the specified interpolator 't'.
        This is equivalent to: (start + Direction() * t).
        */
        inline V<T> Lerp(const T& t) const
        {
            V<T> point(end);
            point -= start;
            point *= t;
            point += start;
            return point;
        }

        template <typename C> inline Line<V, C> Cast() const
        {
            return Line<V<C>, C>(
                start.Cast<C>(),
                end.Cast<C>()
            );
        }

        /* === Members === */

        VecT start, end;

};


template <typename T> using Line2 = Line<Vector2, T>;
template <typename T> using Line3 = Line<Vector3, T>;
template <typename T> using Line4 = Line<Vector4, T>;

DEFAULT_MATH_TYPEDEFS(Line2)
DEFAULT_MATH_TYPEDEFS(Line3)
DEFAULT_MATH_TYPEDEFS(Line4)


} // /namespace Math

} // /namespace Fork


#endif



// ========================