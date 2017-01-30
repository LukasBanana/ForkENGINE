/*
 * Cone header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CONE_H__
#define __FORK_CONE_H__


#include "Math/Geometry/Ray.h"
#include "Math/Geometry/Line.h"
#include "Math/Core/MathConstants.h"


namespace Fork
{

namespace Math
{


/**
3D cone class. The orientation ray points from the
cone's tip (or rather top point of the cone) to the bottom.
\code
// Visualization:
// 
// Y       O
// ^       ^
// |      /|\
// |     / | \
// |    /  |h \
// |   /   |   \
// |  /____|____\
// |       <--r-->
// 0----------------> X
// 
// O := orientation ray (points down here) (see Cone::orientation)
// r := bottom radius (see Cone::radius)
// h := cone height (see Cone::height)
\endcode
*/
template <typename T = float> class Cone
{
    
    public:
        
        Cone() = default;
        Cone(const T& coRadius, const T& coHeight) :
            radius{ coRadius },
            height{ coHeight }
        {
        }
        Cone(const T& coRadius, const T& coHeight, const Ray3<T>& coOrientation) :
            radius      { coRadius      },
            height      { coHeight      },
            orientation { coOrientation }
        {
        }

        /* === Functions === */

        //! Returns the cone's mantle line (independent of the cone's orientation) from the tip to the ground.
        inline Line3<T> MantleLine() const
        {
            return { {}, { 0, -height, radius } };
        }

        /**
        Returns the cone's mantle line length. This is equivalent (but faster) to the following code:
        \code
        MantleLine().Length();
        \endcode
        */
        inline T MantleLineLength() const
        {
            return std::sqrt(Math::Sq(radius) + Math::Sq(height));
        }

        //! Returns the volume of this cone.
        inline T Volume() const
        {
            return T(1)/T(3) * Math::PI<T>() * Math::Sq(radius) * height;
        }

        //! Returns the mantle area of this cone.
        inline T MantleArea() const
        {
            const T s = MantleLineLength();
            return radius * s * Math::PI<T>();
        }

        //! Returns the ground area of this cone.
        inline T GroundArea() const
        {
            return Math::PI<T>() * Math::Sq(radius);
        }

        //! Returns the surface area of this cone (SurfaceArea + GroundArea).
        inline T SurfaceArea() const
        {
            return GroundArea() + GroundArea();
        }

        /**
        Returns the cone angle (in radians).
        \remarks This is the double of the angle between the vertical- and the mantle line.
        */
        inline T Angle() const
        {
            return Math::VectorAngle(
                Vector3<T>(0, 1, 0),
                Vector3<T>(0, height, radius)
            )*T(2);
        }

        /* === Members === */

        T       radius      { 0 };                          //!< Cone bottom radius.
        T       height      { 0 };                          //!< 
        Ray3<T> orientation { { 0, 0, 0 }, { 0, -1, 0 } };  //!< Cone orientation ray. Points from the tip to the bottom.

};


} // /namespace Math

} // /namespace Fork


#endif



// ========================