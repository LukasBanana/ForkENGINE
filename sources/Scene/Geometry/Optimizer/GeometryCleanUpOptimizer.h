/*
 * Geometry clean-up optimizer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_CLEANUP_OPTIMIZER_H__
#define __FORK_GEOMETRY_CLEANUP_OPTIMIZER_H__


#include "GeometryOptimizer.h"


namespace Fork
{

namespace Scene
{


//! Optimizes a geometry node graph by cleaning up redundant geometry nodes.
class GeometryCleanUpOptimizer : public GeometryOptimizer
{
    
    public:

        void OptimizeGeometryGraph(GeometryPtr& geometry) override;

    private:
        
        void VisitGeometryNode          (GeometryNode*          node) override;
        void VisitLODGeometry           (LODGeometry*           node) override;
        void VisitCompositionGeometry   (CompositionGeometry*   node) override;
        void VisitTexturedGeometry      (TexturedGeometry*      node) override;

        void CleanUpGeometry(GeometryPtr& node);

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================