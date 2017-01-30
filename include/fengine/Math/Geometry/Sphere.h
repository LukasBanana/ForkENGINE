/*
 * Sphere header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SPHERE_H__
#define __FORK_SPHERE_H__


#include "Math/Core/Vector3.h"
#include "Math/Core/MathConstants.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace Math
{


//! Abstract sphere model. This data model contains a radius and 3D center point.
template <typename T = float> class Sphere
{
    
    public:
        
        Sphere() = default;
        explicit Sphere(const T& spRadius) :
            radius{ spRadius }
        {
        }
        Sphere(const T& spRadius, const Vector3<T>& spPoint) :
            radius  { spRadius },
            point   { spPoint  }
        {
        }

        /* === Functions === */

        //! Returns the volume of the sphere (4/3*pi*r^3).
        inline T Volume() const
        {
            return T(4)/T(3) * Math::PI<T>() * radius*radius*radius;
        }

        //! Returns the surface area of the sphere (4*pi*r^2).
        inline T SurfaceArea() const
        {
            return T(4) * Math::PI<T>() * radius*radius;
        }

        /* === Members === */

        T           radius = 0; //!< Sphere radius. By default 0.
        Point3<T>   point;      //!< Sphere center point.

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================