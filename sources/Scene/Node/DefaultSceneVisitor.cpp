/*
 * Defualt scene visitor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Node/DefaultSceneVisitor.h"
#include "Scene/Node/GeometryNode.h"
#include "Scene/Node/LightNode.h"
#include "Scene/Node/CameraNode.h"

#include "Scene/Geometry/Node/LODGeometry.h"
#include "Scene/Geometry/Node/CompositionGeometry.h"
#include "Scene/Geometry/Node/TexturedGeometry.h"


namespace Fork
{

namespace Scene
{


/* --- Scene nodes --- */

void DefaultSceneVisitor::VisitSceneNode(SceneNode* node)
{
    VisitSceneNodeChildren(node);
}

void DefaultSceneVisitor::VisitGeometryNode(GeometryNode* node)
{
    node->geometry->Visit(this);
    VisitSceneNodeChildren(node);
}

void DefaultSceneVisitor::VisitLightNode(LightNode* node)
{
    //node->lightSource->Visit(this);
    VisitSceneNodeChildren(node);
}

void DefaultSceneVisitor::VisitCameraNode(CameraNode* node)
{
    VisitSceneNodeChildren(node);
}

/* --- Geometries --- */

void DefaultSceneVisitor::VisitLODGeometry(LODGeometry* node)
{
    VisitSubGeometries(node->lodGeometries);
}

void DefaultSceneVisitor::VisitCompositionGeometry(CompositionGeometry* node)
{
    VisitSubGeometries(node->subGeometries);
}

void DefaultSceneVisitor::VisitTexturedGeometry(TexturedGeometry* node)
{
    if (node->actualGeometry)
        node->actualGeometry->Visit(this);
}


/*
 * ======= Protected: =======
 */

void DefaultSceneVisitor::VisitSceneNodeChildren(SceneNode* node)
{
    for (auto& child : node->GetChildren())
        child->Visit(this);
}

void DefaultSceneVisitor::VisitSubGeometries(const std::vector<GeometryPtr>& subGeometries)
{
    for (auto& subGeom : subGeometries)
        subGeom->Visit(this);
}


} // /namespace Scene

} // /namespace Fork



// ========================