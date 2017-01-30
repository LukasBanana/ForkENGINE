/*
 * Static bezier header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STATIC_BEZIER_H__
#define __FORK_STATIC_BEZIER_H__


#include "Math/Core/BaseMath.h"

#include <array>


namespace Fork
{

namespace Math
{


/**
Generic static bezier class. This is the static version fo the bezier curve, i.e. the number of control points
are static (or rather fixed at compile time). If you want to change the number of control points dynamically
during run-time, use the "DynamicBezier" class.
\tparam V Specifies the class for the control points. This should be Vector2f, Vector2d, Vector3f or Vector3d.
\tparam numPoints Specifies the number of control points. By default 4 for a classic bezier curve.
\note For the static bezier class template meta-programming has been used. But don't worry, it's not that
difficult ;-). The template function "Interpolate", interpolates all control points of the bezier curve,
i.e. interpolations between the 'numPoints' points will be made and stored in a sub-bezier curve with
'numPoints - 1' points. Then this sub-bezier performs a further interpolation and so forth. This happens
until numPoints == 2. Then the specialized bezier class (StaticBezier<V, 2>) will be used for a simple
linear-interpolation between the two control points (see Math::Lerp). And that's it.
\see Math::Lerp
\see DynamicBezier
*/
template <class V, size_t numPoints = 4u> class StaticBezier
{
    
    public:
        
        StaticBezier()
        {
            static_assert(numPoints >= 2, "Bezier curve must have at least two control points");
        }
        StaticBezier(const std::array<V, numPoints>& ctrlPoints) :
            points(ctrlPoints)
        {
            static_assert(numPoints >= 2, "Bezier curve must have at least two control points");
        }

        /**
        Interpolates this spline.
        \param[in] t Specifies the interpolation time point in the range [0.0 .. 1.0].
        */
        template <typename I> void Interpolate(V& result, const I& t) const
        {
            /* Compute control points */
            StaticBezier<V, numPoints - 1> subBezier;
            for (size_t i = 0; i + 1 < numPoints; ++i)
                Lerp<V, I>(subBezier.points[i], points[i], points[i + 1], t);

            /* Make next sub-interpolation */
            subBezier.Interpolate(result, t);
        }

        //! \see Interpolate(V&, const I&)
        template <typename I> V Interpolate(const I& t) const
        {
            V result = V(0);
            Interpolate(result, t);
            return result;
        }

        //! Bezier control points.
        std::array<V, numPoints> points;

};

/**
Specialized version of the bezier template class. Just performs a simple linear-interpolation.
See base class for descriptions.
*/
template <class V> class StaticBezier<V, 2u>
{
    
    public:
        
        StaticBezier() = default;
        StaticBezier(const std::array<V, 2u>& ctrlPoints) :
            points(ctrlPoints)
        {
        }

        template <typename I> void Interpolate(V& result, const I& t) const
        {
            Lerp<V, I>(result, points[0], points[1], t);
        }

        template <typename I> V Interpolate(const I& t) const
        {
            return Lerp<V, I>(points[0], points[1], t);
        }

        std::array<V, 2u> points;

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================