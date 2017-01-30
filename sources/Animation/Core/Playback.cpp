/*
 * Playback file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Animation/Core/Playback.h"


namespace Fork
{

namespace Anim
{


Playback::Playback(const EventHandlerPtr& initialEventHandler) :
    eventHandler_{ initialEventHandler }
{
}

void Playback::Play(size_t firstFrameIndex, size_t lastFrameIndex, double newSpeed, const EventHandlerPtr& newEventHandler)
{
    /* Setup frame indices */
    firstFrame      = firstFrameIndex;
    lastFrame       = lastFrameIndex;
    frame           = firstFrame;

    /* Setup interpolator and speed */
    interpolator    = 0.0;
    speed           = newSpeed;
    state_          = States::Playing;

    /* Setup event handler */
    if (newEventHandler)
        eventHandler_ = newEventHandler;

    /* Initialize next frame */
    if (eventHandler_)
    {
        eventHandler_->OnPlay(*this);
        if (IsPlaying())
            eventHandler_->OnNextFrame(*this);
    }
    else
        SetupNextFrame();
}

void Playback::Play(size_t firstFrameIndex, size_t lastFrameIndex, const EventHandlerPtr& newEventHandler)
{
    Play(firstFrameIndex, lastFrameIndex, speed, newEventHandler);
}

void Playback::Play(const EventHandlerPtr& newEventHandler)
{
    Play(firstFrame, lastFrame, speed, newEventHandler);
}

void Playback::Pause(bool isPaused)
{
    if (isPaused)
    {
        /* Pause playback */
        if (state_ == States::Playing)
            ChangePauseState(States::Paused);
    }
    else
    {
        /* Resume playback */
        if (state_ == States::Paused)
            ChangePauseState(States::Playing);
    }
}

void Playback::Stop()
{
    if (state_ != States::Stopped)
    {
        /* Stop playback */
        state_ = States::Stopped;

        /* Emit 'play' event */
        if (eventHandler_)
            eventHandler_->OnPlay(*this);
    }
}

void Playback::Update(double deltaTime)
{
    /* Check if playback is currently being played */
    if (state_ != States::Playing || deltaTime <= 0.0)
        return;

    /* Increase frame interpolator */
    interpolator += deltaTime * speed;

    if (IsForward())
    {
        if (interpolator < 1.0)
            return;
    
        /* Check if event handler is used for 'next frame' process */
        if (eventHandler_)
        {
            do
            {
                /* Set current frame to the next frame */
                interpolator -= 1.0;
                frame = nextFrame;

                /* Get next frame by event handler */
                eventHandler_->OnNextFrame(*this);
            }
            while (interpolator >= 1.0);
        }
        else
        {
            do
            {
                /* Set current frame to the next frame */
                interpolator -= 1.0;
                frame = nextFrame;

                /* Get next frame by default playback behavior */
                if (HasEndReached())
                    Stop();
                SetupNextFrame();
            }
            while (interpolator >= 1.0);
        }
    }
    else
    {
        if (interpolator > 0.0)
            return;
    
        /* Check if event handler is used for 'next frame' process */
        if (eventHandler_)
        {
            do
            {
                /* Set current frame to the next frame */
                interpolator += 1.0;
                nextFrame = frame;

                /* Get next frame by event handler */
                eventHandler_->OnNextFrame(*this);
            }
            while (interpolator <= 0.0);
        }
        else
        {
            do
            {
                /* Set current frame to the next frame */
                interpolator += 1.0;
                nextFrame = frame;

                /* Get next frame by default playback behavior */
                if (HasEndReached())
                    Stop();
                SetupNextFrame();
            }
            while (interpolator <= 0.0);
        }
    }
}

void Playback::SetupNextFrame(size_t nextFrameIndex)
{
    if (IsForward())
        nextFrame = nextFrameIndex;
    else
        frame = nextFrameIndex;
}

void Playback::SetupNextFrame()
{
    /* Set next frame index depending on playback direction (forward or backward) and chronology */
    if (IsForward())
        nextFrame = (AreFramesChrono() ? frame + 1 : frame - 1);
    else
        frame = (AreFramesChrono() ? frame - 1 : frame + 1);
}

bool Playback::HasEndReached() const
{
    if (IsForward())
        return AreFramesChrono() ? frame >= lastFrame : frame <= lastFrame;
    else
        return AreFramesChrono() ? frame <= firstFrame : frame >= firstFrame;
}


/*
 * ======= Private: =======
 */

void Playback::ChangePauseState(const States state)
{
    /* Change playback state */
    state_ = state;

    /* Emit 'pause' event */
    if (eventHandler_)
        eventHandler_->OnPause(*this);
}


} // /namespace Anim

} // /namespace Fork



// ========================