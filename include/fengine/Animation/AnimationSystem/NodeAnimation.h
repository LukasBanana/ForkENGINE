/*
 * Node animation header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NODE_ANIMATION_H__
#define __FORK_NODE_ANIMATION_H__


#include "Animation/AnimationSystem/Animation.h"
#include "Animation/Core/KeyframeSequence.h"
#include "Scene/Node/DynamicSceneNode.h"


namespace Fork
{

namespace Anim
{


DECL_SHR_PTR(NodeAnimation);

/**
Scene node animation implementation.
\ingroup animation
*/
class FORK_EXPORT NodeAnimation : public Animation
{

    public:
        
        Types Type() const override;

        //! Updates the playback and transforms the scene node.
        void Update(double deltaTime = 1.0/60.0) override;

        //! Animation keyframe sequence.
        KeyframeSequence keyframeSequence;

        /**
        Reference to the transformation which is to be animated.
        This is just a raw pointer since an animation must not own a scene node's transformation.
        */
        Scene::Transform* transform = nullptr;

};


} // /namespace Anim

} // /namespace Fork


#endif



// ========================