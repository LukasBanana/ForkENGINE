/*
 * Present scene node controller header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PRESENT_SCENE_NODE_CONTROLLER_H__
#define __FORK_PRESENT_SCENE_NODE_CONTROLLER_H__


#include "Scene/Manager/SceneNodeController.h"


namespace Fork
{

namespace Scene
{


//! Scene node controller class to present a scene node.
class FORK_EXPORT PresentSceneNodeController : public SceneNodeController
{
    
    public:
        
        void ProcessRotation(DynamicSceneNode* sceneNode);
        void ProcessRotation(DynamicSceneNode* sceneNode, const Math::Vector2f& motion);

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================