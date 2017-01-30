/*
 * Plane header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLANE_H__
#define __FORK_PLANE_H__


#include "Math/Core/Vector3.h"
#include "Math/Geometry/Triangle.h"


namespace Fork
{

namespace Math
{


/**
The plane class contains a normal vector and a distance from the origin.
The plane's front face points into the direction of the normal vector.
\remarks The normal vector must always be normalized!
\note The plane equation for this class is: ( ax + by + bz = d ) where x, y, z specify
the plane's normal vector, 'd' specifies the plane's distance and a, b, c specify a point onto the plane.
Another often used form is ( ax + by + bz + d = 0 ), which results in slightly different computations.
*/
template <typename T = float> class Plane
{
    
    public:
        
        Plane() = default;
        /**
        Constructs the plane with the specified normal vector and distance.
        \remarks The plane will be normalized after construction.
        */
        Plane(const Vector3<T>& plNormal, const T& plDistance) :
            normal  { plNormal   },
            distance{ plDistance }
        {
            Normalize();
        }
        /**
        Constructs the plane with the specified plane equation values.
        \param[in] x Specifies the X value of the normal vector.
        \param[in] y Specifies the Y value of the normal vector.
        \param[in] z Specifies the Z value of the normal vector.
        \param[in] d Specifies the plane's distance.
        \remarks The plane will be normalized after construction.
        */
        Plane(const T& x, const T& y, const T& z, const T& d) :
            normal  { x, y, z },
            distance{ d       }
        {
            Normalize();
        }
        /**
        Constructs the plane with the specified three points.
        \param[in] a Specifies the first point.
        \param[in] b Specifies the second point.
        \param[in] c Specifies the third point.
        \remarks The plane will be normalized after construction.
        */
        Plane(const Point3<T>& a, const Point3<T>& b, const Point3<T>& c)
        {
            Build(a, b, c);
        }
        /**
        Constructs the plane with the three points of the specified triangle.
        \remarks The plane will be normalized after construction.
        */
        explicit Plane(const Triangle3<T>& triangle)
        {
            Build(triangle.a, triangle.b, triangle.c);
        }

        /* === Functions === */

        //! Builds the plane by the three spaning 3D points.
        void Build(const Point3<T>& a, const Point3<T>& b, const Point3<T>& c)
        {
            /* Build plane with equation: normal := || (b - a) x (c - a) || */
            normal = b;
            normal -= a;
            normal = Math::Cross(normal, c - a);
            normal.Normalize();

            distance = Math::Dot(normal, a);
        }

        //! Normalizes the plane (normal and distance).
        Plane<T>& Normalize()
        {
            T invLen = normal.Length();
            
            if (invLen > T(0))
            {
                invLen = T(1) / invLen;
                normal *= invLen;
                distance *= invLen;
            }

            return *this;
        }

        //! Returns a member point which lies onto the plane (normal * distance).
        inline Point3<T> MemberPoint() const
        {
            return normal * distance;
        }

        //! Flips the plane, or rather inverses the components normal and distance.
        inline void MakeFlip()
        {
            normal = -normal;
            distance = -distance;
        }

        inline Plane<T> Flip() const
        {
            auto result = *this;
            result.MakeFlip();
            return result;
        }

        template <typename C> inline Plane<C> Cast() const
        {
            return Plane<C>(
                normal.Cast<C>(),
                static_cast<C>(distance)
            );
        }

        /* === Members === */

        Vector3<T>  normal;             //!< Plane's normal vector. Must always be normalized.
        T           distance = T(0);    //!< Plane's distance to the origin.

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================