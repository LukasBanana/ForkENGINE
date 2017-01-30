/*
 * Scene node controller header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_NODE_CONTROLLER_H__
#define __FORK_SCENE_NODE_CONTROLLER_H__


#include "Scene/Node/DynamicSceneNode.h"


namespace Fork
{

namespace Scene
{


/**
Scene node controller interface (for dynamic scene nodes only).
This can be used to easily rotation and move a camera scene node for instance.
\see DynamicSceneNode
*/
class FORK_EXPORT SceneNodeController
{
    
    public:
        
        //! Groups of movement key codes.
        enum class MoveKeyGroups
        {
            WASD,   //!< Move the scene node with W/A/S/D keys.
            Arrows, //!< Move the scene node with the arrow keys.
        };

        virtual ~SceneNodeController();

        //! Processes the user's input, to control the rotation of the scene node with the standard mouse motion.
        virtual void ProcessRotation(DynamicSceneNode* sceneNode);
        //! Processes the user's input, to control the rotation of the scene node.
        virtual void ProcessRotation(DynamicSceneNode* sceneNode, const Math::Vector2f& motion) = 0;
        //! Processes the user's input, to control the movement of the scene node.
        virtual void ProcessMovement(DynamicSceneNode* sceneNode, const MoveKeyGroups moveKeyGroup = MoveKeyGroups::WASD);

        //! Processes the user's input, to control borth rotation and movement of the scene node.
        virtual void Process(DynamicSceneNode* sceneNode, const MoveKeyGroups moveKeyGroup = MoveKeyGroups::WASD);

        //! Rotation factor. By default 0.5.
        float rotateFactor = 0.5f;

    protected:
        
        SceneNodeController() = default;

        //! Returns the rotation via the user-mouse interaction.
        Math::Vector2f Rotation(float rotateFactor) const;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================