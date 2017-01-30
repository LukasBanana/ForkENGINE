/*
 * Skeletal animation header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SKELETAL_ANIMATION_H__
#define __FORK_SKELETAL_ANIMATION_H__


#include "Animation/AnimationSystem/Animation.h"
#include "Animation/Core/KeyframeSequence.h"
#include "Scene/Geometry/Skeleton.h"

#include <string>
#include <vector>
#include <map>


namespace Fork
{

namespace Anim
{


DECL_SHR_PTR(SkeletalAnimation);

/**
Skeletal animation implementation.
\ingroup animation
\todo Incomplete
*/
class FORK_EXPORT SkeletalAnimation : public Animation
{

    public:
        
        //! Skeletal animation joint type.
        typedef Scene::Skeleton::Joint Joint;

        //! Structure to store joint reference and its keyframe sequence.
        class FORK_EXPORT KeyframeJoint
        {
            
            public:
                
                //! \throws NullPointerException If 'joint' is null.
                KeyframeJoint(Joint* joint);

                /**
                Updates the interpolation of all keyframe joints.
                \see KeyframeSequence::Interpolate
                */
                static void Update(std::vector<KeyframeJoint>& keyframeJoints, const Playback& playback);

                //! Returns the joint reference.
                inline Joint* GetJoint() const
                {
                    return joint_;
                }

                //! Animation keyframe sequence for this joint.
                KeyframeSequence keyframeSequence;

            private:
                
                Joint* joint_ = nullptr; //!< Reference to the joint node.

        };

        //! Skeletal joint group structure.
        struct FORK_EXPORT JointGroup
        {
            std::string                 name;           //!< Name of this joint group.
            Playback                    playback;       //!< Animation playback for this joint group.
            std::vector<KeyframeJoint>  keyframeJoints; //!< Keyframe joints in this group.
        };

        //! \throws NullPointerException If 'skeletojn' is null.
        SkeletalAnimation(const Scene::SkeletonPtr& skeleton);

        //! Returns Animation::Types::Skeletal.
        Types Type() const override;

        /**
        Updates the playback and transforms the keyframe joints.
        The behavior of this function can be controlled by the "animateJointGroup" member.
        \remarks You can also update the keyframe joints by yourself:
        \code
        skeletalAnim->playback.Update(deltaTime);
        for (const auto& keyJoint : skeletalAnim->keyframeJoints)
            keyJoint.keyframeSequence.Interpolate(keyJoint.GetJoint()->transform, skeletalAnim->playback);
        \endcode
        \see animateJointGroup
        */
        void Update(double deltaTime = 1.0/60.0) override;

        //! Returns the mesh skeleton.
        inline Scene::Skeleton* GetSkeleton() const
        {
            return skeleton_.get();
        }

        //! Keyframe joint list.
        std::vector<KeyframeJoint> keyframeJoints;

        //! Joint group list.
        std::vector<JointGroup> jointGroups;

        /**
        Specifies whether the joint groups will be animated or all joints together.
        If this is true, each joint group can be animated individually. By default false.
        */
        bool animateJointGroup = false;

    private:
        
        Scene::SkeletonPtr skeleton_;

};


} // /namespace Anim

} // /namespace Fork


#endif



// ========================