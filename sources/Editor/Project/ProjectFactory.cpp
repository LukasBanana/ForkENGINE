/*
 * Editor project factory file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ProjectFactory.h"
#include "ProjectFolder.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Editor
{


ProjectFactory* ProjectFactory::Instance()
{
    static ProjectFactory instance;
    return &instance;
}

ProjectFolder* ProjectFactory::CreateProjectFolder()
{
    /* Create new project folder, add to list and return reference */
    auto project = std::make_unique<ProjectFolder>();

    auto projectFolderRef = project.get();
    projectFolderRef->Activate();

    projectFolders_.push_back(std::move(project));

    return projectFolderRef;
}

void ProjectFactory::RemoveProjectFolder(ProjectFolder* project)
{
    RemoveAllFromListIf(
        projectFolders_,
        [&project](const std::unique_ptr<ProjectFolder>& entry)
        {
            return entry.get() == project;
        }
    );
}

void ProjectFactory::RemoveAllProjectFolders()
{
    projectFolders_.clear();
}


} // /namespace Engine

} // /namespace Fork



// ========================