/*
 * Capsule header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CAPSULE_H__
#define __FORK_CAPSULE_H__


#include "Math/Geometry/Ray.h"
#include "Math/Core/MathConstants.h"


namespace Fork
{

namespace Math
{


template <typename T = float> class Capsule
{
    
    public:
        
        Capsule() = default;
        Capsule(const T& cpRadius, const T& cpHeight) :
            radius{ cpRadius },
            height{ cpHeight }
        {
        }
        Capsule(const T& cpRadius, const T& cpHeight, const Ray3<T>& cpOrientation) :
            radius      { cpRadius      },
            height      { cpHeight      },
            orientation { cpOrientation }
        {
        }

        /* === Functions === */

        //! Returns the volume of this capsule.
        inline T Volume() const
        {
            /* Compute capsule volume by adding its cylinder- and sphere volume */
            const T sqRadius = Math::Sq(radius);
            const T volCylinder = Math::PI<T>()*sqRadius*height;
            const T volSphere = T(4)/T(3) * Math::PI<T>() * sqRadius*radius;
            return volCylinder + volSphere;
        }

        //! Returns the surface area of this capsule.
        inline T SurfaceArea() const
        {
            /* Compute capsule surface area by adding its cylinder- and sphere surface area */
            const T areaMantle = T(2) * Math::PI<T>() * radius * height;
            const T areaSphere = T(4) * Math::PI<T>() * Math::Sq(radius);
            return areaMantle + areaSphere;
        }

        /* === Members === */

        T radius = 0, height = 0;
        Ray3<T> orientation = { { 0, 0, 0 }, { 0, -1, 0 } }; //!< Capsule orientation. Points from the tip to the bottom.

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================