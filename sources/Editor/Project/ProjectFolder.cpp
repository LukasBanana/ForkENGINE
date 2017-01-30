/*
 * Editor project folder file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProjectFolder.h"
#include "../Core/Component/UIComponentFactory.h"
#include "../Frame/Main/MainFrame.h"
#include "../Entity/Entity.h"
#include "IO/FileSystem/PhysicalFile.h"
#include "Core/StringModifier.h"
#include "../Core/Paths.h"
#include "IO/Core/Log.h"
#include "Core/Exception/DefaultException.h"

#include "Engine/Component/GeometryComponent.h"
#include "Engine/Component/MetaDataComponent.h"
#include "../Tools/AssetBrowser/Asset/ModelUIAsset.h"


namespace Fork
{

namespace Editor
{


using namespace Engine;

ProjectFolder*      ProjectFolder::activeProject_       = nullptr;
const std::string   ProjectFolder::defaultProjectName   = "ForkProject";

ProjectFolder::ProjectFolder() :
    directory_{ Paths::dirProjects }
{
}
ProjectFolder::~ProjectFolder()
{
    if (IsActive())
        ProjectFolder::activeProject_ = nullptr;
}

/* --- Common --- */

void ProjectFolder::Activate()
{
    ProjectFolder::activeProject_ = this;
    UpdateMainFrameLabel();
}

bool ProjectFolder::IsActive() const
{
    return ProjectFolder::Active() == this;
}

ProjectFolder* ProjectFolder::Active()
{
    return activeProject_;
}

Engine::GameScene* ProjectFolder::ActiveScene()
{
    return Active() != nullptr ? Active()->activeGameScene : nullptr;
}

bool ProjectFolder::LoadProject(const std::string& filename)
{
    IO::PhysicalFile file(filename, IO::File::OpenFlags::Read);
    if (LoadContent(file))
    {
        ChangeFilename(filename);
        return true;
    }
    return false;
}

bool ProjectFolder::SaveProject(const std::string& filename)
{
    IO::PhysicalFile file(filename, IO::File::OpenFlags::Write);
    if (SaveContent(file))
    {
        ChangeFilename(filename);
        return true;
    }
    return false;
}

bool ProjectFolder::IsValidProjectName(const std::string& projectName)
{
    /* Check if string is empty */
    if (projectName.empty())
        return false;

    /* Check for alphanumeric characters */
    auto it = std::find_if_not(
        projectName.begin(),
        projectName.end(),
        [](const char& chr) -> bool
        {
            return
                ( chr >= 'a' && chr <= 'z' ) ||
                ( chr >= 'A' && chr <= 'Z' ) ||
                ( chr >= '0' && chr <= '9' ) ||
                chr == '_' ||
                chr == '-' ||
                chr == '.' ||
                chr == ' ';
        }
    );

    if (it != projectName.end())
        return false;

    /* Check if start and end characters are no white spaces */
    return projectName.front() != ' ' && projectName.back() != ' ';
}

void ProjectFolder::ChangeName(const std::string& projectName)
{
    if (ProjectFolder::IsValidProjectName(projectName))
    {
        name = projectName;
        if (IsActive())
            UpdateMainFrameLabel();
    }
}

Scene::SceneNode* ProjectFolder::ActiveSceneGraph() const
{
    return (activeGameScene != nullptr) ? &(activeGameScene->sceneGraph) : nullptr;
}

/* --- Game objects --- */

Entity* ProjectFolder::CreateModelEntity(const ModelUIAsset* asset)
{
    /* Create entity */
    auto entity = std::make_unique<Entity>(GetIDFactory());

    auto entityRef = entity.get();
    gameObjects.push_back(std::move(entity));

    try
    {
        /* Add meta data*/
        auto metaDataComponent = static_cast<MetaDataComponent*>(entityRef->CreateComponent(Component::Types::MetaData));
        metaDataComponent->SetupName(asset->GetLabel());

        /* Add transform */
        entityRef->CreateComponent(Component::Types::Transform);

        #if 1//!TESTING!
        entityRef->CreateComponent(Component::Types::AutoUVMap);
        #endif

        /* Add geometry */
        auto geometryComponent = static_cast<GeometryComponent*>(entityRef->CreateComponent(Component::Types::Geometry));
        geometryComponent->SetupGeometry(asset->GetSharedGeometryResource());
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
    }

    entityRef->UpdateSelectionCollider();

    /* Add to all scenes */
    AddToAllScenes(entityRef);

    return entityRef;
}

void ProjectFolder::DeleteEntity(Entity* entity)
{
    DeleteGameObject(entity);
}


/*
 * ======= Private: =======
 */

void ProjectFolder::UpdateMainFrameLabel()
{
    MainFrame::Instance()->ChangeLabelWithProjectName(GetName());
}

void ProjectFolder::ChangeFilename(const std::string& filename)
{
    filename_   = filename;
    directory_  = ExtractFilePath(filename_);
}


} // /namespace Editor

} // /namespace Fork



// ========================