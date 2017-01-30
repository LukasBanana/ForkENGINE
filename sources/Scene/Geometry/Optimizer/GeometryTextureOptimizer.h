/*
 * Geometry texture optimizer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_TEXTURE_OPTIMIZER_H__
#define __FORK_GEOMETRY_TEXTURE_OPTIMIZER_H__


#include "GeometryOptimizer.h"


namespace Fork
{

namespace Scene
{


//! Optimizes a geometry node graph for better texture usage.
class GeometryTextureOptimizer : public GeometryOptimizer
{
    
    private:
        
        void VisitCompositionGeometry(CompositionGeometry* node) override;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================