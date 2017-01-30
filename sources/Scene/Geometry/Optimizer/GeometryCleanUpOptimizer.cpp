/*
 * Geometry clean-up optimizer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GeometryCleanUpOptimizer.h"
#include "Scene/Node/GeometryNode.h"
#include "Scene/Geometry/Node/CompositionGeometry.h"
#include "Scene/Geometry/Node/LODGeometry.h"
#include "Scene/Geometry/Node/TexturedGeometry.h"


namespace Fork
{

namespace Scene
{


void GeometryCleanUpOptimizer::OptimizeGeometryGraph(GeometryPtr& geometry)
{
    CleanUpGeometry(geometry);
    geometry->Visit(this);
}


/*
 * ======= Private: =======
 */

void GeometryCleanUpOptimizer::VisitGeometryNode(GeometryNode* node)
{
    CleanUpGeometry(node->geometry);
    DefaultSceneVisitor::VisitGeometryNode(node);
}

void GeometryCleanUpOptimizer::VisitLODGeometry(LODGeometry* node)
{
    for (auto& subGeom : node->lodGeometries)
        CleanUpGeometry(subGeom);
    DefaultSceneVisitor::VisitLODGeometry(node);
}

void GeometryCleanUpOptimizer::VisitCompositionGeometry(CompositionGeometry* node)
{
    for (auto& subGeom : node->subGeometries)
        CleanUpGeometry(subGeom);
    DefaultSceneVisitor::VisitCompositionGeometry(node);
}

void GeometryCleanUpOptimizer::VisitTexturedGeometry(TexturedGeometry* node)
{
    CleanUpGeometry(node->actualGeometry);
    DefaultSceneVisitor::VisitTexturedGeometry(node);
}

void GeometryCleanUpOptimizer::CleanUpGeometry(GeometryPtr& node)
{
    /* Clean up all composition geometries with only a single sub geometry */
    while (node->Type() == Geometry::Types::Composition)
    {
        auto compositionGeom = dynamic_cast<CompositionGeometry*>(node.get());
        if (compositionGeom && compositionGeom->subGeometries.size() == 1)
        {
            /* Replace this node with the single sub geometry node */
            auto metaData = node->metaData;
            node = compositionGeom->subGeometries.front();
            node->metaData = metaData;
        }
        else
            break;
    }
}


} // /namespace Scene

} // /namespace Fork



// ========================