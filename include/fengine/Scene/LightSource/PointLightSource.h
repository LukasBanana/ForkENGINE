/*
 * Point light source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POINT_LIGHT_SOURCE_H__
#define __FORK_POINT_LIGHT_SOURCE_H__


#include "Scene/LightSource/LightSource.h"


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(PointLightSource);

//! Point (also called "omni directional") light source.
class FORK_EXPORT PointLightSource : public LightSource
{
    
    public:
        
        virtual ~PointLightSource()
        {
        }

        //! Returns LightSource::Types::Point.
        Types Type() const
        {
            return Types::Point;
        }

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================