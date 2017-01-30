/*
 * Scene helper file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/SceneHelper.h"
#include "Scene/Manager/SceneManager.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Scene
{


FORK_EXPORT GeometryNodePtr LoadMesh(
    SceneManager& sceneMngr, SceneNode& sceneGraph, const std::string& filename, const ModelReader::Flags::DataType flags)
{
    auto sceneNode = sceneMngr.CreateGeometryNode(sceneMngr.LoadGeometry(filename, flags));
    sceneNode->metaData.name = ExtractFileName(filename);
    sceneGraph.AddChild(sceneNode);
    return sceneNode;
}

FORK_EXPORT GeometryNodePtr LoadMesh(
    SceneManager* sceneMngr, SceneNode* sceneGraph, const std::string& filename, const ModelReader::Flags::DataType flags)
{
    if (sceneMngr)
    {
        auto sceneNode = sceneMngr->CreateGeometryNode(sceneMngr->LoadGeometry(filename, flags));
        if (sceneGraph)
            sceneGraph->AddChild(sceneNode);
        return sceneNode;
    }
    return nullptr;
}


} // /namespace Scene

} // /namespace Fork



// ========================