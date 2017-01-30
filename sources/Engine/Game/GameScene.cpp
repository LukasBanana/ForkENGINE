/*
 * Game scene file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Game/GameScene.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Engine
{


GameScene::~GameScene()
{
}

/* --- Common --- */

void GameScene::ChangeName(const std::string& gameName)
{
    sceneGraph.metaData.name = gameName;
}

/* --- Game objects --- */

void GameScene::AddGameObject(GameObject* gameObject)
{
    if (gameObject)
    {
        gameObject->ForEachComponentWithSceneNode(
            [&](Component* component, const Scene::DynamicSceneNodePtr& sceneNode)
            {
                sceneGraph.AddChild(sceneNode);
            }
        );
        gameObjects.insert(gameObject);
    }
}

void GameScene::RemoveGameObject(GameObject* gameObject)
{
    if (gameObject)
    {
        gameObject->ForEachComponentWithSceneNode(
            [&](Component* component, const Scene::DynamicSceneNodePtr& sceneNode)
            {
                sceneGraph.RemoveChild(sceneNode);
            }
        );
        RemoveFromList(gameObjects, gameObject);
    }
}


} // /namespace Engine

} // /namespace Fork



// ========================