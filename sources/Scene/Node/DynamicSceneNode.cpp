/*
 * Dynamic scene node file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Node/DynamicSceneNode.h"


namespace Fork
{

namespace Scene
{


DynamicSceneNode::~DynamicSceneNode()
{
}

const Math::Matrix4f& DynamicSceneNode::LocalTransform() const
{
    return transform.GetMatrix();
}

void DynamicSceneNode::GlobalTransform(Math::Matrix4f& matrix) const
{
    if (GetParent())
        GetParent()->GlobalTransform(matrix);
    matrix *= transform.GetMatrix();
}

Math::Matrix4f DynamicSceneNode::GlobalTransform() const
{
    /*
    Just return base function to support easy function access,
    i.e. this will avoid that the client programmer needs to write:
    "dynamicSceneNode->Scene::SceneNode::GlobalTransform()"
    */
    return SceneNode::GlobalTransform();
}

bool DynamicSceneNode::HasTransform() const
{
    return true;
}


} // /namespace Scene

} // /namespace Fork



// ========================