/*
 * Ray header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RAY_H__
#define __FORK_RAY_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"


namespace Fork
{

namespace Math
{


// Forward declaration
template <template <typename> class V, typename T> class Line;

/**
Abstract ray class. A ray consists of the members: "origin" and "direction".
The direction vector must always be normalized!
\tparam V Specifies the vector type. This must be Vector2, Vector3 or Vector4.
\tparam T Specifies the data type. This should be float or double.
*/
template <template <typename> class V, typename T> class Ray
{
    
    public:
        
        typedef V<T> VecT;

        Ray() = default;
        /**
        Initialize the ray with the specified origin and direction.
        The direction will be normalized automatically.
        */
        Ray(const VecT& ryOrigin, const VecT& ryDirection) :
            origin      { ryOrigin    },
            direction   { ryDirection }
        {
            direction.Normalize();
        }
        explicit Ray(const Line<V, T>& line);

        /* === Functions === */

        //! Flips the ray, or rather negates the direction.
        inline void MakeFlip()
        {
            direction = -direction;
        }

        inline Ray<V, T> Flip() const
        {
            auto result = *this;
            result.MakeFlip();
            return result;
        }

        /**
        Returns a linear-interpolation ('lerp') with the specified interpolator 't'.
        This is equivalent to: (origin + direction * t).
        */
        inline V<T> Lerp(const T& t) const
        {
            V<T> point(direction);
            point *= t;
            point += origin;
            return point;
        }

        /**
        Converts this ray into a line segment.
        \param[in] length Specifies the length of the line segment.
        \return The new converted line segment: { ray.origin, ray.Lerp(length) }.
        */
        Line<V, T> ToLine(const T& length) const
        {
            return { origin, Lerp(length) };
        }

        template <typename C> inline Ray<V, C> Cast() const
        {
            return Ray<V<C>, C>(
                origin.Cast<C>(),
                direction.Cast<C>()
            );
        }

        /* === Members === */

        VecT origin;    //!< Specifies the ray origin.
        VecT direction; //!< Specifies the ray direction. This vector must always be normalized!

};


template <typename T> using Ray2 = Ray<Vector2, T>;
template <typename T> using Ray3 = Ray<Vector3, T>;
template <typename T> using Ray4 = Ray<Vector4, T>;

DEFAULT_MATH_TYPEDEFS(Ray2)
DEFAULT_MATH_TYPEDEFS(Ray3)
DEFAULT_MATH_TYPEDEFS(Ray4)


} // /namespace Math

} // /namespace Fork


#endif



// ========================