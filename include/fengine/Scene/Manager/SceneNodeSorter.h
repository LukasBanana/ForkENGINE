/*
 * Scene node sorter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_NODE_SORTER_H__
#define __FORK_SCENE_NODE_SORTER_H__


#include "Core/Export.h"
#include "Scene/Node/SceneNodeDecl.h"
#include "Math/Core/Transform3D.h"

#include <vector>


namespace Fork
{

namespace Scene
{

namespace SceneNodeSorter
{


//! Scene node sort methods.
enum class SortMethods
{
    NearToFar,              //!< Sort all objects from near to far.
    FarToNear,              //!< Sort all objects from far to near.
    OpaqueToTransparent,    //!< Sort opaque (near to far) to transparent (far to near).
};


/**
Sorts the scene node list with the specified sort method.
\param[in,out] sceneNodes Resulting scene node list which is to be sorted.
\param[in] compareTransform Specifies the transformation to which each scene node will be compared, to sort them.
This should be the camera's transformation (in world space and not the view space!).
\param[in] method Specifies the sort method.
\param[in] isGlobal Specifies whether the object transformations should be global or local.
\see SortMethods
\see SortSceneNodesGlobal
*/
FORK_EXPORT void SortSceneNodes(
    std::vector<SceneNodePtr>& sceneNodes, const Math::Transform3Df& compareTransform,
    const SortMethods method, bool isGlobal
);


} // /namespace SceneNodeSorter

} // /namespace Scene

} // /namespace Fork


#endif



// ========================