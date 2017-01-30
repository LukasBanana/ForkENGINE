/*
 * Scene manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Manager/SceneManager.h"
#include "Scene/FileHandler/ModelFileHandler.h"
#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"
#include "Scene/Node/CameraNode.h"
#include "Scene/Node/LightNode.h"
#include "Core/STLHelper.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{


SceneManager::~SceneManager()
{
}

/* --- Geometry functions --- */

GeometryPtr SceneManager::CreateGeometryFromFile(const std::string& filename, const ModelReader::Flags::DataType flags)
{
    return Scene::ModelFileHandler::ReadModel(filename, texturePathDict, flags);
}

GeometryPtr SceneManager::LoadGeometry(const std::string& filename, const ModelReader::Flags::DataType flags)
{
    if (flags == 0)
    {
        /* Check if model already exists */
        auto geometry = geometries.Find(filename);
        if (geometry)
            return geometry;

        /* Otherwise load model from file */
        geometry = CreateGeometryFromFile(filename);
        geometries.hashMap[filename] = geometry;

        return geometry;
    }
    return CreateGeometryFromFile(filename, flags);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateCube(const GeometryGenerator::CubeDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateCube, desc, basicGeometryContainer.cube);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateWireCube(const GeometryGenerator::CubeDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateWireCube, desc, basicGeometryContainer.wireCube);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateCone(const GeometryGenerator::ConeDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateCone, desc, basicGeometryContainer.cone);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateCylinder(const GeometryGenerator::CylinderDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateCylinder, desc, basicGeometryContainer.cylinder);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateUVSphere(const GeometryGenerator::UVSphereDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateUVSphere, desc, basicGeometryContainer.uvSphere);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateIcoSphere(const GeometryGenerator::IcoSphereDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateIcoSphere, desc, basicGeometryContainer.icoSphere);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateTorus(const GeometryGenerator::TorusDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateTorus, desc, basicGeometryContainer.torus);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateSpiral(const GeometryGenerator::SpiralDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateSpiral, desc, basicGeometryContainer.spiral);
}

GeometryGenerator::GeometryTypePtr SceneManager::GenerateCapsule(const GeometryGenerator::CapsuleDescription& desc)
{
    return GenerateBasicGeometry(GeometryGenerator::GenerateCapsule, desc, basicGeometryContainer.capsule);
}

/* --- Scene node functions --- */

GeometryNodePtr SceneManager::CreateGeometryNode()
{
    return AddSceneNode(std::make_shared<Scene::GeometryNode>());
}

GeometryNodePtr SceneManager::CreateGeometryNode(const GeometryPtr& geometry)
{
    auto sceneNode = CreateGeometryNode();
    sceneNode->geometry = geometry;
    return sceneNode;
}

GeometryNodePtr SceneManager::CreateGeometryNodeFromFile(const std::string& filename)
{
    return CreateGeometryNode(LoadGeometry(filename));
}

CameraNodePtr SceneManager::CreateCameraNode()
{
    return AddSceneNode(std::make_shared<Scene::CameraNode>());
}

LightNodePtr SceneManager::CreateLightNode()
{
    return AddSceneNode(std::make_shared<Scene::LightNode>());
}

LightNodePtr SceneManager::CreateLightNode(const LightSourcePtr& lightSource)
{
    auto sceneNode = CreateLightNode();
    sceneNode->lightSource = lightSource;
    return sceneNode;
}

/* --- Common functions --- */

void SceneManager::ReleaseSceneNode(const SceneNode* sceneNode)
{
    RemoveFromListIf(
        sceneNodes,
        [&sceneNode](const SceneNodePtr& entry)
        {
            return entry.get() == sceneNode;
        }
    );
}

void SceneManager::ReleaseAllSceneNodes()
{
    sceneNodes.clear();
}


/*
 * ======= Private: =======
 */

template <class GenProc, class Desc, class Container>
GeometryGenerator::GeometryTypePtr SceneManager::GenerateBasicGeometry(GenProc genProc, const Desc& desc, Container& container)
{
    /* Check if model already exists */
    auto geometry = container.Find(desc);
    if (geometry)
        return geometry;

    /* Otherwise generate new model */
    auto meshGeometry = std::make_shared<GeometryGenerator::GeometryType>();
    genProc(*meshGeometry, desc);

    container.hashMap[desc] = meshGeometry;

    /* Upload geometry to the GPU */
    meshGeometry->SetupHardwareBuffer();

    return meshGeometry;
}


} // /namespace Scene

} // /namespace Fork



// ========================