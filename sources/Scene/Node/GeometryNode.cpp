/*
 * Geometry node file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Node/GeometryNode.h"


namespace Fork
{

namespace Scene
{


GeometryNode::~GeometryNode()
{
}

void GeometryNode::Visit(SceneVisitor* visitor)
{
    visitor->VisitGeometryNode(this);
}


} // /namespace Scene

} // /namespace Fork



// ========================