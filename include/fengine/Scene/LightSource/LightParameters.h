/*
 * Light parameters header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LIGHT_PARAMETERS_H__
#define __FORK_LIGHT_PARAMETERS_H__


#include "Math/Core/MathConstants.h"
#include "Math/Core/BaseMath.h"
#include "Video/Core/Projection.h"

#include <numeric>


namespace Fork
{

namespace Scene
{


/**
Light attenuation structure.
\tparam T Specifies the data type. Should be float or double. By default float.
*/
template <typename T = float> struct LightAttenuation
{
    LightAttenuation(const T& att0, const T& att1, const T& att2) :
        attn0{ att0 },
        attn1{ att1 },
        attn2{ att2 }
    {
        static_assert(!std::numeric_limits<T>::is_integer, "Light attenuation can not be stored with integers!");
    }
    LightAttenuation() :
        LightAttenuation{ T(1.0), T(0.1), T(0.4) }
    {
    }

    //! Resizes the radius.
    void Resize(const T& radius)
    {
        if (radius > Math::Epsilon<T>())
        {
            attn0 = T(1);
            attn1 = T(1) / radius;
            attn2 = attn1;
        }
    }
    //! Returns the radius size.
    T Size() const
    {
        return attn1 > Math::Epsilon<T>() ? T(1) / attn1 : T(0);
    }

    T attn0; //!< Constant attenuation.
    T attn1; //!< Linear attenuation.
    T attn2; //!< Quadratic attenuation.
};


/**
Spot light cone structure.
\tparam T Specifies the data type. Should be float or double. By default float.
*/
template <typename T = float> class LightCone
{
    
    public:
        
        LightCone() :
            innerAngle_{ T(30) * Math::Deg2Rad<T>() },
            outerAngle_{ T(60) * Math::Deg2Rad<T>() }
        {
        }
        LightCone(const T& innerAngle, const T& outerAngle) :
            innerAngle_{ innerAngle },
            outerAngle_{ outerAngle }
        {
        }

        //! Sets the inner cone angle (in radians) and clamps it to [0 .. Pi/2].
        inline void SetupInnerAngle(const T& angle)
        {
            innerAngle_ = Math::Clamp(angle, T(0), T(0.5)*Math::PI<T>());
        }
        //! Returns the inner cone angle. By default 3/18*pi (30 degrees).
        inline const T& GetInnerAngle() const
        {
            return innerAngle_;
        }

        //! Sets the outer cone angle (in radians) and clamps it to [0 .. Pi/2].
        inline void SetupOuterAngle(const T& angle)
        {
            outerAngle_ = Math::Clamp(angle, T(0), T(0.5)*Math::PI<T>());
        }
        //! Returns the outer cone angle. By default 6/18*pi (60 degrees).
        inline const T& GetOuterAngle() const
        {
            return outerAngle_;
        }

    private:

        T innerAngle_; //!< Inner spot cone angle (in radians). Also called "theta". By default 3/18*pi (30 degrees).
        T outerAngle_; //!< Outer spot cone angle (in radians). Also called "phi". By default 6/18*pi (60 degrees).

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================