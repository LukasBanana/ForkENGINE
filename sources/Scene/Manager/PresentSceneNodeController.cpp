/*
 * Present scene node controller file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Manager/PresentSceneNodeController.h"
#include "IO/InputDevice/Mouse.h"
#include "Math/Core/MathConstants.h"


namespace Fork
{

namespace Scene
{


void PresentSceneNodeController::ProcessRotation(DynamicSceneNode* sceneNode)
{
    SceneNodeController::ProcessRotation(sceneNode);
}

void PresentSceneNodeController::ProcessRotation(DynamicSceneNode* sceneNode, const Math::Vector2f& motion)
{
    if (!sceneNode)
        return;

    /* Update rotation state */
    Math::Matrix3f rotation;
    rotation.RotateX(-motion.y);
    rotation.RotateY(-motion.x);

    /* Setup new scene node rotation transform */
    auto prevRotation = sceneNode->transform.GetRotation();
    prevRotation *= Math::Quaternionf(rotation);

    sceneNode->transform.SetRotation(prevRotation);
}


} // /namespace Scene

} // /namespace Fork



// ========================