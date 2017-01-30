/*
 * Morph-target animation header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MORPH_TARGET_ANIMATION_H__
#define __FORK_MORPH_TARGET_ANIMATION_H__


#include "Animation/AnimationSystem/Animation.h"
#include "Scene/Geometry/Node/MeshGeometry.h"


namespace Fork
{

namespace Anim
{


DECL_SHR_PTR(MorphTargetAnimation);

/**
Morph-target animation implementation.
\ingroup animation
\todo Incomplete
*/
class FORK_EXPORT MorphTargetAnimation : public Animation
{

    public:
        
        Types Type() const override;

        /**
        Updates the playback and transforms the mesh geometry.
        \todo Not yet implemented!
        */
        void Update(double deltaTime = 1.0/60.0) override;

        /**
        Mesh geometry reference.
        \note This is just a raw pointer since an animation must not own a geometry.
        */
        Scene::MeshGeometry* geometry = nullptr;

};


} // /namespace Anim

} // /namespace Fork


#endif



// ========================