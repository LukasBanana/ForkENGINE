/*
 * Morph-target animation file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Animation/AnimationSystem/MorphTargetAnimation.h"


namespace Fork
{

namespace Anim
{


Animation::Types MorphTargetAnimation::Type() const
{
    return Types::MorphTarget;
}

void MorphTargetAnimation::Update(double deltaTime)
{
    playback.Update(deltaTime);
    //todo...
}


} // /namespace Anim

} // /namespace Fork



// ========================