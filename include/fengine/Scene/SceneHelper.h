/*
 * Scene helper header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_HELPER_H__
#define __FORK_SCENE_HELPER_H__


#include "Core/Export.h"
#include "Scene/Node/GeometryNode.h"
#include "Scene/FileHandler/ModelReader.h"


namespace Fork
{

namespace Scene
{


class SceneNode;
class SceneManager;

/**
This is a helper function to load a 3D mesh from file and attach it to a scene graph.
\param[in] sceneMngr Specifies the scene manager which is used to load and manage the geometry instance.
This is ued to avoid loading the same file several times from file (see 'CreateGeometryNode' and 'LoadGeometry').
\param[in] sceneGraph Specifies the scene graph to which the new mesh will be attached to (see 'AddChild').
\param[in] filename Specifies the file of the 3D mesh which is to be loaded.
\param[in] flags Optional geometry reading flags (see "SceneManager::CreateGeometryFromFile"). By default 0.
\return Shared pointer to the new geometry scene node.
\see SceneManager::CreateGeometryNode
\see SceneManager::LoadGeometry
\see SceneManager::CreateGeometryFromFile
\see SceneNode::AddChild
*/
FORK_EXPORT GeometryNodePtr LoadMesh(
    SceneManager& sceneMngr, SceneNode& sceneGraph,
    const std::string& filename, const ModelReader::Flags::DataType flags = 0
);
/**
This is a helper function to load a 3D mesh from file and attach it to a scene graph.
\param[in] sceneMngr Specifies the scene manager which is used to load and manage the geometry instance.
This is ued to avoid loading the same file several times from file (see 'CreateGeometryNode' and 'LoadGeometry').
\param[in] sceneGraph Optional raw pointer to the scene graph to which the new mesh will be attached to (see 'AddChild').
If this is null the new geometry scene node will not be attached to any scene graph.
\param[in] filename Specifies the file of the 3D mesh which is to be loaded.
\param[in] flags Optional geometry reading flags (see "SceneManager::CreateGeometryFromFile"). By default 0.
\return Shared pointer to the new geometry scene node or null if 'sceneMngr' is null.
\see SceneManager::CreateGeometryNode
\see SceneManager::LoadGeometry
\see SceneManager::CreateGeometryFromFile
\see SceneNode::AddChild
*/
FORK_EXPORT GeometryNodePtr LoadMesh(
    SceneManager* sceneMngr, SceneNode* sceneGraph,
    const std::string& filename, const ModelReader::Flags::DataType flags = 0
);


} // /namespace Scene

} // /namespace Fork


#endif



// ========================