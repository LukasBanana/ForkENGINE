/*
 * Sphere coordinate header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SPHERE_POINT_H__
#define __FORK_SPHERE_POINT_H__


#include "Math/Core/DefaultMathTypeDefs.h"


namespace Fork
{

namespace Math
{


/**
SpherePoint base class with components theta, phi and radius. This represents a sperical coordinate.
'Theta' is used for vertical rotation and 'phi' is used for horizontal rotation.
*/
template <typename T> class SpherePoint
{
    
    public:
        
        SpherePoint() = default;
        SpherePoint(const T& vRadius, const T& vTheta, const T& vPhi) :
            radius  { vRadius },
            theta   { vTheta  },
            phi     { vPhi    }
        {
        }

        /* === Functions === */

        /**
        Returns the squared sphere point length (from origin). This is the radius^2.
        The function is just provided for uniformity with the vector classes: Vector2, Vector3 and Vector4.
        */
        inline T LengthSq() const
        {
            return radius*radius;
        }
        /**
        Returns the sphere point length (from origin). This is the radius.
        The function is just provided for uniformity with the vector classes: Vector2, Vector3 and Vector4.
        */
        inline T Length() const
        {
            return radius;
        }

        //! Resizes the sphere point by setting the radius to the specified length.
        inline SpherePoint<T>& Resize(const T& length)
        {
            radius = length;
            return *this;
        }

        //! Normalizes the sphere point by setting the radius to 1.
        inline SpherePoint<T>& Normalize()
        {
            radius = T(1);
            return *this;
        }

        //! Returns this vector as constant raw pointer.
        inline const T* Ptr() const
        {
            return &radius;
        }
        //! Returns this vector as raw pointer.
        inline T* Ptr()
        {
            return &radius;
        }

        /* === Members === */

        T radius = 0, theta = 0, phi = 0;

};


DEFAULT_MATH_TYPEDEFS(SpherePoint)


} // /namespace Math

} // /namespace Fork


#endif



// ========================