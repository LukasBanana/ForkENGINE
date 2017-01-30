/*
 * Directional light source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DIRECTIONAL_LIGHT_SOURCE_H__
#define __FORK_DIRECTIONAL_LIGHT_SOURCE_H__


#include "Scene/LightSource/LightSource.h"
#include "Math/Geometry/Ray.h"


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(DirectionalLightSource);

//! Directional light source.
class FORK_EXPORT DirectionalLightSource : public LightSource
{
    
    public:
        
        virtual ~DirectionalLightSource()
        {
        }

        //! Returns LightSource::Types::Directional.
        Types Type() const
        {
            return Types::Directional;
        }

        //! Global light direction. By default (0, 0, 1).
        Math::Vector3f direction { 0, 0, 1 };

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================