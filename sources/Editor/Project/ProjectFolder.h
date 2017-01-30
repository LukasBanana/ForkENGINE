/*
 * Editor project folder header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_PROJECT_FOLDER_H__
#define __FORK_EDITOR_PROJECT_FOLDER_H__


#include "Engine/Game/Game.h"


namespace Fork
{

namespace Editor
{


class Entity;
class ModelUIAsset;

class ProjectFolder : public Engine::Game
{
    
    public:
        
        //! Default project name stirng. This is "ForkProject".
        static const std::string defaultProjectName;

        ProjectFolder();
        ~ProjectFolder();
        
        ProjectFolder(const ProjectFolder&) = delete;
        ProjectFolder& operator = (const ProjectFolder&) = delete;

        /* --- Common --- */

        /**
        Makes this project folder to the active one.
        \see Active
        \see IsActive
        */
        void Activate();
        /**
        Returns true if this is the active project folder.
        \see Active
        */
        bool IsActive() const;

        //! Returns the active project folder.
        static ProjectFolder* Active();
        //! Returns the active game scene of the active project folder.
        static Engine::GameScene* ActiveScene();

        /**
        Returns true if the specified project name is valid.
        This is the case when the following requirements are fulfilled:
        - The string is not empty.
        - The string does not start and end with white spaces (' ', '\t', '\n', '\r').
        - The string does only contain the following alphanumeric symbols: 'a'-'z', 'A'-'Z', '0'-'9', '_', '-', '.', ' '.
        */
        static bool IsValidProjectName(const std::string& projectName);

        /**
        Changes the name of this project folder.
        If this is the active project, the main frame's label with also be changed.
        \param[in] projectName Specifies the new project name.
        If 'IsValidProjectName(projectName)' is false, the function call has no effect.
        By default Game::defaultProjectName.
        \remarks For the project name only a few subset of the ASCII chart is allowed.
        This is due to compatibility with file systems of multiple operating systems.
        \see MainFrame::ChangeLabelWithProjectName
        \see Active
        \see IsValidProjectName
        \see defaultProjectName
        */
        void ChangeName(const std::string& projectName) override;

        //! Returns the active scene graph.
        Scene::SceneNode* ActiveSceneGraph() const;

        /* --- I/O system --- */

        //! Loads this project folder from the specified file.
        bool LoadProject(const std::string& filename);
        //! Saves this project folder to the specified file.
        bool SaveProject(const std::string& filename);

        /**
        Returns the current filename of this project folder.
        This will be changed whenever a project is loaded or saved.
        \see LoadProject
        \see SaveProject
        */
        inline const std::string& GetFilename() const
        {
            return filename_;
        }
        /**
        Returns the current directory of this project folder.
        This will be changed whenever a project is loaded or saved.
        \see LoadProject
        \see SaveProject
        */
        inline const std::string& GetDirectory() const
        {
            return directory_;
        }

        /* --- Game objects --- */

        //! Creates a new entity with the specified model asset.
        Entity* CreateModelEntity(const ModelUIAsset* asset);

        //! Deletes the specified entity.
        void DeleteEntity(Entity* entity);

    private:
        
        void UpdateMainFrameLabel();

        void ChangeFilename(const std::string& filename);

        static ProjectFolder* activeProject_;

        /* === Members === */

        std::string filename_;  //!< Current project folder filename.
        std::string directory_; //!< Directory where the project folder and sub directories are stored.

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================