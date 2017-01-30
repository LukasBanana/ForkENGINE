/*
 * Editor project factory header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_PROJECT_FACTORY_H__
#define __FORK_EDITOR_PROJECT_FACTORY_H__


#include "Engine/Game/Game.h"

#include <vector>
#include <memory>


namespace Fork
{

namespace Editor
{


class ProjectFolder;

/**
Engine project factory singleton class.
\remarks This factory does NOT share the owernship of its objects.
\remarks To access the active ProjectFolder use "ProjectFolder::Active".
\see ProjectFolder::Active
*/
class ProjectFactory : public Engine::Game
{
    
    public:
        
        ProjectFactory(const ProjectFactory&) = delete;
        ProjectFactory& operator = (const ProjectFactory&) = delete;

        //! Returns the singleton instance.
        static ProjectFactory* Instance();

        /**
        Creates a new project folder and return the non-owning raw pointer.
        \see GameProject
        */
        ProjectFolder* CreateProjectFolder();
        /**
        Removes the specified project from the list (and deletes the object).
        \note Make sure no further reference to this project is used after this call!
        */
        void RemoveProjectFolder(ProjectFolder* project);

        /**
        Removes all project folders.
        \note Make sure no further references to any project are used after this call!
        */
        void RemoveAllProjectFolders();

    private:
        
        ProjectFactory() = default;

        std::vector<std::unique_ptr<ProjectFolder>> projectFolders_;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================