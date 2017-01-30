/*
 * Default playback event handlers file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Animation/Core/DefaultPlaybackEventHandlers.h"

#include <algorithm>


namespace Fork
{

namespace Anim
{

//! Namespace for all default playback event handlers.
namespace DefaultPlayback
{


void OneShot::OnNextFrame(Playback& playback)
{
    if (playback.HasEndReached())
        playback.Stop();
    else
        playback.SetupNextFrame();
}

void Loop::OnNextFrame(Playback& playback)
{
    if (playback.HasEndReached())
        playback.SetupNextFrame(playback.IsForward() ? playback.firstFrame : playback.lastFrame);
    else
        playback.SetupNextFrame();
}

void PingPongLoop::OnNextFrame(Playback& playback)
{
    if (playback.HasEndReached())
        std::swap(playback.firstFrame, playback.lastFrame);
    playback.SetupNextFrame();
}

void ListLoop::OnNextFrame(Playback& playback)
{
    const auto numFrames = frames.size();

    if (numFrames >= 2)
    {
        if (playback.IsForward())
        {
            if (iterator >= (numFrames - 1))
                iterator = 0;
            else
                ++iterator;
        }
        else
        {
            if (iterator == 0)
                iterator = (numFrames - 1);
            else
                --iterator;
        }

        playback.SetupNextFrame(frames[iterator]);
    }
}


} // /namespace DefaultPlayback

} // /namespace Anim

} // /namespace Fork



// ========================