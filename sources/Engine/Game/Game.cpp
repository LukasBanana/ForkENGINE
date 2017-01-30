/*
 * Game file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Game/Game.h"
#include "Core/STLHelper.h"
#include "../FileHandler/GameReader.h"
#include "../FileHandler/GameWriter.h"


namespace Fork
{

namespace Engine
{


Game::Game() :
    idFactory_{ std::make_shared<IdentityFactory>() }
{
}
Game::~Game()
{
}

/* --- Common --- */

void Game::ChangeName(const std::string& gameName)
{
    name = gameName;
}

/* --- Game scenes --- */

GameScene* Game::CreateGameScene(const std::string& name)
{
    /* Create new game scene and return reference */
    auto gameScene = std::make_unique<GameScene>();
    gameScene->ChangeName(name);
    
    auto gameSceneRef = gameScene.get();
    gameScenes.push_back(std::move(gameScene));
    
    /* Set this to the active game scene */
    activeGameScene = gameSceneRef;

    return gameSceneRef;
}

void Game::DeleteGameScene(GameScene* gameScene)
{
    /* Release active game scene if this was the active one */
    if (activeGameScene == gameScene)
        activeGameScene = nullptr;

    /* Remove game scene from global list */
    RemoveFromListIf(
        gameScenes,
        [&gameScene](const std::unique_ptr<GameScene>& obj)
        {
            return obj.get() == gameScene;
        }
    );
}

/* --- Game objects --- */

GameObject* Game::CreateGameObject(bool addToAllScenes)
{
    /* Create new game object and return reference */
    auto gameObject = std::make_unique<GameObject>(idFactory_);

    auto gameObjectRef = gameObject.get();
    gameObjects.push_back(std::move(gameObject));

    if (addToAllScenes)
        AddToAllScenes(gameObjectRef);

    return gameObjectRef;
}

GameObject* Game::CopyGameObject(const GameObject* gameObject, bool addToAllScenes)
{
    /* Create new game object and return reference */
    auto copy = gameObject->InstantiateCopy(idFactory_, GetComponentFactory());

    auto copyRef = copy.get();
    gameObjects.push_back(std::move(copy));

    if (addToAllScenes)
        AddToAllScenes(copyRef);

    return copyRef;
}

void Game::DeleteGameObject(GameObject* gameObject)
{
    /* Remove game object form all game scenes */
    RemoveFromAllScenes(gameObject);

    /* Remove game object from global list */
    RemoveFromListIf(
        gameObjects,
        [&gameObject](const std::unique_ptr<GameObject>& obj)
        {
            return obj.get() == gameObject;
        }
    );
}

/* --- I/O system --- */

bool Game::LoadContent(IO::File& file)
{
    Format::GameReader reader(file, *this);
    return reader.ReadGame();
}

bool Game::SaveContent(IO::File& file)
{
    Format::GameWriter writer(file, *this);
    return writer.WriteGame();
}

bool Game::LoadState(IO::File& file)
{
    //todo
    return false;
}

bool Game::SaveState(IO::File& file)
{
    //todo
    return false;
}


/*
 * ======= Protected: =======
 */

ComponentFactory* Game::GetComponentFactory() const
{
    /* Return null -> use the default component factory */
    return nullptr;
}

void Game::AddToAllScenes(GameObject* gameObject)
{
    for (auto& scene : gameScenes)
        scene->AddGameObject(gameObject);
}

void Game::RemoveFromAllScenes(GameObject* gameObject)
{
    for (auto& scene : gameScenes)
        scene->RemoveGameObject(gameObject);
}


} // /namespace Engine

} // /namespace Fork



// ========================