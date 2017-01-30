/*
 * Scene node file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Node/SceneNode.h"


namespace Fork
{

namespace Scene
{


static Math::Matrix4f identityMatrix;

SceneNode::~SceneNode()
{
}

void SceneNode::Visit(SceneVisitor* visitor)
{
    visitor->VisitSceneNode(this);
}

const Math::Matrix4f& SceneNode::LocalTransform() const
{
    return identityMatrix;
}

void SceneNode::GlobalTransform(Math::Matrix4f& matrix) const
{
    /* Dummy */
}

Math::Matrix4f SceneNode::GlobalTransform() const
{
    /* Comfort function with simple return type */
    Math::Matrix4f matrix;
    GlobalTransform(matrix);
    return matrix;
}

bool SceneNode::HasTransform() const
{
    return false;
}

void SceneNode::AddChild(const SceneNodePtr& sceneNode)
{
    /* Remove reference to previous parent */
    if (sceneNode->parent_)
    {
        /*
        Use temporary parent variable for safety.
        -> 'sceneNode->parent_' will be set to null in the "RemoveChild" function!
        */
        auto prevParent = sceneNode->parent_;
        prevParent->RemoveChild(sceneNode);
    }

    /* Set new parnet refernece to this scene node */
    sceneNode->parent_ = this;

    /* Add scene node to child list */
    children_.push_back(sceneNode);
}

void SceneNode::RemoveChild(const SceneNodePtr& sceneNode)
{
    for (auto it = children_.begin(); it != children_.end(); ++it)
    {
        if (*it == sceneNode)
        {
            sceneNode->parent_ = nullptr;
            children_.erase(it);
            break;
        }
    }
}

void SceneNode::ReleaseChildren()
{
    /* Remove parent links and clear children list */
    for (auto& child : children_)
        child->parent_ = nullptr;
    children_.clear();
}

void SceneNode::SortSceneNodes(
    const Transform& compareTransform, const SceneNodeSorter::SortMethods method, bool isGlobal)
{
    SceneNodeSorter::SortSceneNodes(children_, compareTransform, method, isGlobal);
}

void SceneNode::SetupParent(const SceneNodePtr& sceneNode, SceneNode* parent)
{
    if (sceneNode && sceneNode->parent_ != parent)
    {
        if (parent)
            parent->AddChild(sceneNode);
        else
            parent->RemoveChild(sceneNode);
    }
}


} // /namespace Scene

} // /namespace Fork



// ========================