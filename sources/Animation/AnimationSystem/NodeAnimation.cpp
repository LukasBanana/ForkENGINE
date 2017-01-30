/*
 * Node animation file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Animation/AnimationSystem/NodeAnimation.h"


namespace Fork
{

namespace Anim
{


Animation::Types NodeAnimation::Type() const
{
    return Types::Node;
}

void NodeAnimation::Update(double deltaTime)
{
    playback.Update(deltaTime);
    if (transform)
        keyframeSequence.Interpolate(*transform, playback);
}


} // /namespace Anim

} // /namespace Fork



// ========================