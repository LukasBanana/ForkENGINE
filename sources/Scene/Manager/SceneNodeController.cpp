/*
 * Scene node controller file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Manager/SceneNodeController.h"
#include "IO/InputDevice/Mouse.h"


namespace Fork
{

namespace Scene
{


SceneNodeController::~SceneNodeController()
{
}

void SceneNodeController::ProcessRotation(DynamicSceneNode* sceneNode)
{
    ProcessRotation(sceneNode, Rotation(rotateFactor));
}

void SceneNodeController::ProcessMovement(DynamicSceneNode* sceneNode, const MoveKeyGroups moveKeyGroup)
{
    /* Dummy */
}

void SceneNodeController::Process(DynamicSceneNode* sceneNode, const MoveKeyGroups moveKeyGroup)
{
    ProcessRotation(sceneNode);
    ProcessMovement(sceneNode, moveKeyGroup);
}


/*
 * ======= Private: =======
 */

Math::Vector2f SceneNodeController::Rotation(float rotateFactor) const
{
    return IO::Mouse::Instance()->GetMotion().Cast<float>() * (Math::deg2rad * rotateFactor);
}


} // /namespace Scene

} // /namespace Fork



// ========================