/*
 * Dynamic bezier header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DYNAMIC_BEZIER_H__
#define __FORK_DYNAMIC_BEZIER_H__


#include "Math/Core/BaseMath.h"

#include <vector>


namespace Fork
{

namespace Math
{


/**
Generic dynamic bezier class. This is the dynamic version fo the bezier curve, i.e. the number of control points
can be changed dynamically during run-time. If a static number of control points if enough for your purpose,
you can use the static version (StaticBezier) which may be faster.
during run-time, use the "DynamicBezier" class.
\tparam V Specifies the class for the control points. This should be Vector2f, Vector2d, Vector3f or Vector3d.
\see StaticBezier
*/
template <class V> class DynamicBezier
{
    
    public:
        
        DynamicBezier()
        {
        }
        DynamicBezier(const size_t numPoints) :
            points{ numPoints }
        {
        }
        DynamicBezier(const std::vector<V>& dbPoints) :
            points{ dbPoints }
        {
        }

        /**
        Interpolates this spline.
        \param[in] t Specifies the interpolation time point in the range [0.0 .. 1.0].
        */
        template <typename I> void Interpolate(V& result, const I& t) const
        {
            const size_t numPoints = points.size();

            if (numPoints > 2)
            {
                /* Compute control points */
                DynamicBezier<V> subBezier(numPoints - 1);
                for (size_t i = 0; i + 1 < numPoints; ++i)
                    Lerp<V, I>(subBezier.points[i], points[i], points[i + 1], t);

                /* Make next sub-interpolation */
                subBezier.Interpolate(result, t);
            }
            else if (numPoints == 2)
                Lerp<V, I>(result, points[0], points[1], t);
            else if (numPoints == 1)
                result = points[0];
        }

        //! \see Interpolate(V&, const I&)
        template <typename I> V Interpolate(const I& t) const
        {
            V result = V(0);
            Interpolate(result, t);
            return result;
        }

        //! Bezier control points.
        std::vector<V> points;

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================