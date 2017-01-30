/*
 * Free view scene node controller file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Manager/FreeViewSceneNodeController.h"
#include "IO/InputDevice/Keyboard.h"
#include "Math/Core/BaseMath.h"
#include "Math/Core/MathConstants.h"
#include "Core/Exception/IndexOutOfBoundsException.h"


namespace Fork
{

namespace Scene
{


void FreeViewSceneNodeController::ProcessRotation(DynamicSceneNode* sceneNode)
{
    SceneNodeController::ProcessRotation(sceneNode);
}

void FreeViewSceneNodeController::ProcessRotation(DynamicSceneNode* sceneNode, const Math::Vector2f& motion)
{
    if (!sceneNode)
        return;

    /* Update rotation state */
    pitch += motion.y;
    pitch = Math::Clamp(pitch, -Math::pi*0.5f, Math::pi*0.5f);

    yaw += motion.x;

    /* Setup new scene node rotation transform */
    Math::Matrix3f rotation;
    rotation.RotateY(yaw);
    rotation.RotateX(pitch);

    sceneNode->transform.SetRotation(Math::Quaternionf(rotation));
}

void FreeViewSceneNodeController::ProcessMovement(DynamicSceneNode* sceneNode, const MoveKeyGroups moveKeyGroup)
{
    static const IO::KeyCodes keyCodes[2][4] =
    {
        { IO::KeyCodes::KeyW, IO::KeyCodes::KeyA, IO::KeyCodes::KeyS, IO::KeyCodes::KeyD },
        { IO::KeyCodes::KeyUp, IO::KeyCodes::KeyLeft, IO::KeyCodes::KeyDown, IO::KeyCodes::KeyRight }
    };

    if (!sceneNode)
        return;

    /* Get index from movement key group */
    const size_t groupIndex = static_cast<size_t>(moveKeyGroup);
    if (groupIndex >= 2)
        throw IndexOutOfBoundsException(__FUNCTION__, groupIndex);

    auto selectedKeyCodes = keyCodes[groupIndex];

    /* Check for key states */
    auto keyboard = IO::Keyboard::Instance();

    auto moveSpeed = moveFactor;
    if (keyboard->KeyDown(IO::KeyCodes::KeyShift))
        moveSpeed *= 3.0f;

    if (keyboard->KeyDown(selectedKeyCodes[0]))
        sceneNode->transform.MoveLocal({ 0, 0, moveSpeed });
    if (keyboard->KeyDown(selectedKeyCodes[1]))
        sceneNode->transform.MoveLocal({ -moveSpeed, 0, 0 });
    if (keyboard->KeyDown(selectedKeyCodes[2]))
        sceneNode->transform.MoveLocal({ 0, 0, -moveSpeed });
    if (keyboard->KeyDown(selectedKeyCodes[3]))
        sceneNode->transform.MoveLocal({ moveSpeed, 0, 0 });
}

void FreeViewSceneNodeController::SetupRotation(DynamicSceneNode* sceneNode)
{
    if (!sceneNode)
        return;
    
    /* Get rotation and convert to euler rotation */
    auto rotation = sceneNode->transform.GetRotation();
    auto eulerRotation = rotation.EulerRotation();

    /* Store new rotation angles */
    pitch   = eulerRotation.x;
    yaw     = eulerRotation.y;
}

void FreeViewSceneNodeController::SetupRotation(const Math::Quaternionf& rotation)
{
    /* Get rotation and convert to euler rotation */
    auto eulerRotation = rotation.EulerRotation();
    pitch   = eulerRotation.x;
    yaw     = eulerRotation.y;
}


} // /namespace Scene

} // /namespace Fork



// ========================