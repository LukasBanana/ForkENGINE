/*
 * Scene node sorter file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Manager/SceneNodeSorter.h"
#include "Scene/Node/SceneNode.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{

namespace SceneNodeSorter
{


/* === Internal functions === */

static Math::Matrix4f invCompareMatrix;

static bool CompareSceneNodesLocalNearToFar(const SceneNodePtr& nodeA, const SceneNodePtr& nodeB)
{
    const auto posA = invCompareMatrix * nodeA->LocalTransform().GetPosition();
    const auto posB = invCompareMatrix * nodeB->LocalTransform().GetPosition();

    return posA < posB;
}

static bool CompareSceneNodesLocalFarToNear(const SceneNodePtr& nodeA, const SceneNodePtr& nodeB)
{
    const auto posA = invCompareMatrix * nodeA->LocalTransform().GetPosition();
    const auto posB = invCompareMatrix * nodeB->LocalTransform().GetPosition();

    return posA > posB;
}

static bool CompareSceneNodesLocalOpaqueToTransparent(const SceneNodePtr& nodeA, const SceneNodePtr& nodeB)
{
    //todo ...
    return false;
}

static bool CompareSceneNodesGlobalNearToFar(const SceneNodePtr& nodeA, const SceneNodePtr& nodeB)
{
    const auto posA = invCompareMatrix * nodeA->GlobalTransform().GetPosition();
    const auto posB = invCompareMatrix * nodeB->GlobalTransform().GetPosition();

    return posA < posB;
}

static bool CompareSceneNodesGlobalFarToNear(const SceneNodePtr& nodeA, const SceneNodePtr& nodeB)
{
    const auto posA = invCompareMatrix * nodeA->GlobalTransform().GetPosition();
    const auto posB = invCompareMatrix * nodeB->GlobalTransform().GetPosition();

    return posA > posB;
}

static bool CompareSceneNodesGlobalOpaqueToTransparent(const SceneNodePtr& nodeA, const SceneNodePtr& nodeB)
{
    //todo ...
    return false;
}


/* === Global functions === */

FORK_EXPORT void SortSceneNodes(
    std::vector<SceneNodePtr>& sceneNodes, const Math::Transform3Df& compareTransform, const SortMethods method, bool isGlobal)
{
    compareTransform.GetMatrix().Inverse(invCompareMatrix);

    if (isGlobal)
    {
        switch (method)
        {
            case SortMethods::NearToFar:
                std::sort(sceneNodes.begin(), sceneNodes.end(), CompareSceneNodesGlobalNearToFar);
                break;
            case SortMethods::FarToNear:
                std::sort(sceneNodes.begin(), sceneNodes.end(), CompareSceneNodesGlobalFarToNear);
                break;
            case SortMethods::OpaqueToTransparent:
                std::sort(sceneNodes.begin(), sceneNodes.end(), CompareSceneNodesGlobalOpaqueToTransparent);
                break;
        }
    }
    else
    {
        switch (method)
        {
            case SortMethods::NearToFar:
                std::sort(sceneNodes.begin(), sceneNodes.end(), CompareSceneNodesLocalNearToFar);
                break;
            case SortMethods::FarToNear:
                std::sort(sceneNodes.begin(), sceneNodes.end(), CompareSceneNodesLocalFarToNear);
                break;
            case SortMethods::OpaqueToTransparent:
                std::sort(sceneNodes.begin(), sceneNodes.end(), CompareSceneNodesLocalOpaqueToTransparent);
                break;
        }
    }
}


} // /namespace SceneNodeSorter

} // /namespace Scene

} // /namespace Fork



// ========================