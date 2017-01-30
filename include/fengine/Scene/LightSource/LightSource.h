/*
 * Light source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LIGHT_SOURCE_H__
#define __FORK_LIGHT_SOURCE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGB.h"


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(LightSource);

//! Light source interface.
class FORK_EXPORT LightSource
{
    
    public:
        
        //! Light source types.
        enum class Types
        {
            Point = 0,      //!< Point (also omni-directional-) light source. \see PointLightSource
            Spot,           //!< Spot light source. \see SpotLightSource
            Directional,    //!< Directional light source. \see DirectionalLightSource
        };

        virtual ~LightSource()
        {
        }

        //! Returns the type of this light source.
        virtual Types Type() const = 0;

        /* === Members === */

        //! Main light color. By default white.
        Video::ColorRGBf color;

        //! Light energy used for physically based shading (in mega watts). By default 1000.0.
        float energy = 1000.0f;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================