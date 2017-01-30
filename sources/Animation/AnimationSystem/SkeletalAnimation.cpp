/*
 * Skeletal animation file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Animation/AnimationSystem/SkeletalAnimation.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Anim
{


/*
 * KeyframeJoint class
 */

SkeletalAnimation::KeyframeJoint::KeyframeJoint(Joint* joint) :
    joint_{ joint }
{
    ASSERT_POINTER(joint);
}

void SkeletalAnimation::KeyframeJoint::Update(
    std::vector<KeyframeJoint>& keyframeJoints, const Playback& playback)
{
    for (const auto& keyJoint : keyframeJoints)
        keyJoint.keyframeSequence.Interpolate(keyJoint.GetJoint()->transform, playback);
}


/*
 * SkeletalAnimation class
 */

SkeletalAnimation::SkeletalAnimation(const Scene::SkeletonPtr& skeleton) :
    skeleton_{ skeleton }
{
    ASSERT_POINTER(skeleton);
}

Animation::Types SkeletalAnimation::Type() const
{
    return Types::Skeletal;
}

void SkeletalAnimation::Update(double deltaTime)
{
    if (animateJointGroup)
    {
        /* Animate each joint group individually */
        for (auto& group : jointGroups)
        {
            /* Update animation playback for current group */
            group.playback.Update(deltaTime);
            KeyframeJoint::Update(group.keyframeJoints, group.playback);
        }
    }
    else
    {
        /* Update animation playback */
        playback.Update(deltaTime);
        KeyframeJoint::Update(keyframeJoints, playback);
    }
}


} // /namespace Anim

} // /namespace Fork



// ========================