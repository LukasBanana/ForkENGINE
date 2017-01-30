/*
 * Convex hull header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CONVEX_HULL_H__
#define __FORK_CONVEX_HULL_H__


#include "Math/Core/DefaultMathTypeDefs.h"
#include "Math/Geometry/Plane.h"
#include "Math/Geometry/Sphere.h"
#include "Math/Collision/PlaneCollisions.h"

#include <array>


namespace Fork
{

namespace Math
{


/**
Convex hull class.
\tparam T Specifies the data type. This should be float or double.
\tparam numPlanes Specifies the number of planes of the convex hull, e.g. a cube is convex hull with 6 planes.
\note All planes of a convex hull always point into the hull!
*/
template <typename T, size_t numPlanes> class ConvexHull
{
    
    public:
        
        ConvexHull() = default;
        ConvexHull(const ConvexHull<T, numPlanes>& other)
        {
            for (size_t i = 0; i < numPlanes; ++i)
                planes[i] = other.planes[i];
        }
        virtual ~ConvexHull()
        {
        }

        /* === Functions === */

        //! Normalizes all planes
        void Normalize()
        {
            for (auto& plane : planes)
                plane.Normalize();
        }

        //! Returns true if the point is inside the convex hull or rather behind all planes of the convex hull.
        bool IsPointInside(const Point3<T>& point) const
        {
            for (const auto& plane : planes)
            {
                if (!IsFrontSideToPlane(plane, point))
                    return false;
            }
            return true;
        }

        //! Returns true if the sphere is inside the convex hull.
        bool IsSphereInside(const Sphere<T>& sphere) const
        {
            for (const auto& plane : planes)
            {
                if (ComputeDistanceToPlane(plane, sphere.point) < -sphere.radius)
                    return false;
            }
            return true;
        }

        /* === Members === */
        
        std::array<Plane<T>, numPlanes> planes; //!< Plane array of the convex hull.

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================