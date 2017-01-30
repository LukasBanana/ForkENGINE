/*
 * Spot light source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SPOT_LIGHT_SOURCE_H__
#define __FORK_SPOT_LIGHT_SOURCE_H__


#include "Scene/LightSource/LightSource.h"
#include "Scene/LightSource/LightParameters.h"


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(SpotLightSource);

//! Spot light source.
class FORK_EXPORT SpotLightSource : public LightSource
{
    
    public:
        
        virtual ~SpotLightSource()
        {
        }

        //! Returns LightSource::Types::Spot.
        Types Type() const
        {
            return Types::Spot;
        }

        /**
        Spot light cone parameters.
        \see LightCone
        */
        LightCone<float> spotCone;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================