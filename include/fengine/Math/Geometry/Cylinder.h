/*
 * Cylinder header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CYLINDER_H__
#define __FORK_CYLINDER_H__


#include "Math/Geometry/Ray.h"
#include "Math/Core/MathConstants.h"
#include "Math/Geometry/Plane.h"


namespace Fork
{

namespace Math
{


template <typename T = float> class Cylinder
{
    
    public:
        
        Cylinder() = default;
        Cylinder(const T& cyRadius, const T& cyHeight) :
            radius{ cyRadius },
            height{ cyHeight }
        {
        }
        Cylinder(const T& cyRadius, const T& cyHeight, const Ray3<T>& cyOrientation) :
            radius      { cyRadius      },
            height      { cyHeight      },
            orientation { cyOrientation }
        {
        }

        /* === Functions === */

        //! Returns the volume of this cylinder.
        inline T Volume() const
        {
            return Math::PI<T>()*Math::Sq(radius)*height;
        }

        //! Returns the mantle area of this cylinder.
        inline T MantleArea() const
        {
            return T(2) * Math::PI<T>() * radius * height;
        }

        //! Returns the ground area of this cylinder.
        inline T GroundArea() const
        {
            return Math::PI<T>() * Math::Sq(radius);
        }

        //! Returns the surface area of this cylinder (SurfaceArea + GroundArea*2).
        inline T SurfaceArea() const
        {
            return T(2) * Math::PI<T>() * radius * (height + radius);
        }

        //! Returns the top center point.
        inline Vector3<T> TopPoint() const
        {
            return orientation.origin;
        }
        //! Returns the bottom center point.
        inline Vector3<T> BottomPoint() const
        {
            return orientation.Lerp(height);
        }

        //! Returns the plane which lies onto the cylinder's top cap.
        inline Plane<T> TopPlane() const
        {
            return { -orientation.direction, Length(orientation.origin) };
        }
        //! Returns the plane which lies onto the cylinder's bottom cap.
        inline Plane<T> BottomPlane() const
        {
            return { orientation.direction, Length(orientation.origin) + height };
        }

        /* === Members === */

        T radius = 0, height = 0;
        Ray3<T> orientation { { 0, 0, 0 }, { 0, -1, 0 } }; //!< Cylinder orientation. Points from the tip to the bottom.

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================