/*
 * Game header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GAME_H__
#define __FORK_GAME_H__


#include "Core/Export.h"
#include "Core/Container/IdentityFactory.h"
#include "Engine/Asset/AssetLibrary.h"
#include "Engine/Game/GameScene.h"
#include "IO/FileSystem/File.h"

#include <string>
#include <vector>
#include <memory>


namespace Fork
{

namespace Engine
{


/**
Main class for a game project.
This class does not contain the game's main loop, but all the game content.
For the main loop, write your own class and inherit from "Engine::App" or "Engine::SimpleApp".
\see SimpleApp
*/
class FORK_EXPORT Game
{
    
    protected:

        typedef std::vector<std::unique_ptr<GameObject>> GameObjectList;
        typedef std::vector<std::unique_ptr<GameScene>> GameSceneList;

    public:
        
        Game();
        virtual ~Game();
        
        Game(const Game&) = delete;
        Game& operator = (const Game&) = delete;

        /* --- Common --- */

        //! Changes the name of this game.
        virtual void ChangeName(const std::string& gameName);

        //! Returns the name of this game.
        inline const std::string& GetName() const
        {
            return name;
        }

        /* --- Game scenes --- */

        /**
        Creates a new game scene and sets this to the active one.
        \see activeGameScene
        */
        GameScene* CreateGameScene(const std::string& name = "gameScene");
        /**
        Deletes the specified game scene. If this was the active game scene it is set to null.
        \see activeGameScene
        */
        void DeleteGameScene(GameScene* gameScene);

        //! Returns the list of all game scenes.
        inline const GameSceneList& GetGameScenes() const
        {
            return gameScenes;
        }

        /* --- Game objects --- */

        //! Creates a new game object and adds it to all game scenes if 'addToAllScenes' is true.
        GameObject* CreateGameObject(bool addToAllScenes = true);
        //! Creates a copy of the specified game object and adds it to all game scenes if 'addToAllScenes' is true.
        GameObject* CopyGameObject(const GameObject* gameObject, bool addToAllScenes = true);
        //! Deletes the specified game object and removes it from all game scenes.
        void DeleteGameObject(GameObject* gameObject);

        //! Returns the list of all game objects.
        inline const GameObjectList& GetGameObjects() const
        {
            return gameObjects;
        }

        /* --- I/O system --- */

        //! Loads the entire game content from the specified file (*.forkgame files).
        bool LoadContent(IO::File& file);
        //! Saves the entire game content to the specified file (*.forkgame files).
        bool SaveContent(IO::File& file);

        //! Loads the entire game state (of all game objects) from the specified file (*.forkstate files).
        bool LoadState(IO::File& file);
        //! Saves the entire game state (of all game objects) to the specified file (*.forkstate files).
        bool SaveState(IO::File& file);

        //! Specifies the active game scene.
        GameScene* activeGameScene = nullptr;

    protected:
        
        /**
        Returns the component factory used to instantiate the game object components.
        If this is null, the default component factory is used. By default null.
        */
        virtual ComponentFactory* GetComponentFactory() const;

        void AddToAllScenes(GameObject* gameObject);
        void RemoveFromAllScenes(GameObject* gameObject);

        std::string         name;

        AssetLibrary        assetLibrary;   //!< Asset library.
        GameSceneList       gameScenes;     //!< List of all game scenes.
        GameObjectList      gameObjects;    //!< List of all game objects.

        //! Returns the ID factory for the game objects.
        inline const IdentityFactoryPtr& GetIDFactory() const
        {
            return idFactory_;
        }

    private:
        
        IdentityFactoryPtr idFactory_;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================