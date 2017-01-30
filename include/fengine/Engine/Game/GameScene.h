/*
 * Game scene header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GAME_SCENE_H__
#define __FORK_GAME_SCENE_H__


#include "Core/Export.h"
#include "Engine/Game/GameObject.h"
#include "Scene/Node/SceneNode.h"

#include <set>
#include <memory>


namespace Fork
{

namespace Engine
{


//! Main class for the game's scene.
class FORK_EXPORT GameScene
{
    
    public:
        
        GameScene() = default;
        virtual ~GameScene();

        GameScene(const GameScene&) = delete;
        GameScene& operator = (const GameScene&) = delete;

        /* --- Common --- */

        //! Changes the name of this scene.
        virtual void ChangeName(const std::string& gameName);

        //! Returns the name of this scene.
        inline const std::string& GetName() const
        {
            return sceneGraph.metaData.name;
        }

        /* --- Game objects --- */

        void AddGameObject(GameObject* gameObject);
        void RemoveGameObject(GameObject* gameObject);

        /**
        Specifies the scene graph root node.
        \todo Should not be public!
        */
        Scene::SceneNode sceneGraph;

    protected:
        
        std::set<GameObject*> gameObjects; //!< Set of all games object in this scene.

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================