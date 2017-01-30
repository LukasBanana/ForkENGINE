/*
 * Light node file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Node/LightNode.h"


namespace Fork
{

namespace Scene
{


LightNode::~LightNode()
{
}

void LightNode::Visit(SceneVisitor* visitor)
{
    visitor->VisitLightNode(this);
}


} // /namespace Scene

} // /namespace Fork



// ========================