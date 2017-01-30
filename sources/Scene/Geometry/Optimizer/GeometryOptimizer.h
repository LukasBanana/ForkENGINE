/*
 * Geometry optimizer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_OPTIMIZER_H__
#define __FORK_GEOMETRY_OPTIMIZER_H__


#include "Scene/Node/DefaultSceneVisitor.h"
#include "Scene/Geometry/Node/Geometry.h"


namespace Fork
{

namespace Scene
{


//! Geometry optimization base class.
class GeometryOptimizer : protected DefaultSceneVisitor
{
    
    public:
        
        virtual void OptimizeGeometryGraph(GeometryPtr& geometry)
        {
            geometry->Visit(this);
        }

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================