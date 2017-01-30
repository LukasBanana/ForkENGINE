/*
 * Free view scene node controller header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FREE_VIEW_SCENE_NODE_CONTROLLER_H__
#define __FORK_FREE_VIEW_SCENE_NODE_CONTROLLER_H__


#include "Scene/Manager/SceneNodeController.h"


namespace Fork
{

namespace Scene
{


/**
Scene node controller class for a free view rotation and movement behaviour.
This can be useful for editor cameras.
*/
class FORK_EXPORT FreeViewSceneNodeController : public SceneNodeController
{
    
    public:
        
        void ProcessRotation(DynamicSceneNode* sceneNode);
        void ProcessRotation(DynamicSceneNode* sceneNode, const Math::Vector2f& motion);
        void ProcessMovement(DynamicSceneNode* sceneNode, const MoveKeyGroups moveKeyGroup = MoveKeyGroups::WASD);
        
        //! Initialize pitch and yaw angles with the specified scene node rotation.
        void SetupRotation(DynamicSceneNode* sceneNode);
        /**
        Initialize pitch and yaw angles with the specified rotation (this is converted to euler rotations).
        \see Math::Quaternion::EulerRotation
        */
        void SetupRotation(const Math::Quaternionf& rotation);

        //! Movement factor. By default 0.1.
        float moveFactor    = 0.1f;

        //! Pitch angle (X axis) in radian. By default 0.0.
        float pitch         = 0.0f;

        //! Yaw itch angle (Y axis) in radian. By default 0.0.
        float yaw           = 0.0f;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================