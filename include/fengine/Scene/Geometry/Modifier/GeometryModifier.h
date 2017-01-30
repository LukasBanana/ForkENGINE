/*
 * Geometry modifier header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_MODIFIER_H__
#define __FORK_GEOMETRY_MODIFIER_H__


#include "Scene/Node/DefaultSceneVisitor.h"
#include "Scene/Geometry/Node/Geometry.h"


namespace Fork
{

namespace Scene
{


//! Geometry modifier base class.
class FORK_EXPORT GeometryModifier : protected DefaultSceneVisitor
{
    
    public:
        
        /**
        Modifies the specified geometry.
        \param[in] geometry Specifies the geometry which is to be modified.
        If this is null, the function call has no effect.
        */
        virtual void ModifyGeometryGraph(Geometry* geometry)
        {
            if (geometry)
                geometry->Visit(this);
        }

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================