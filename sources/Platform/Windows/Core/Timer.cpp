/*
 * WIN32: Timer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/Timer.h"
#include "Win32Timer.h"


namespace Fork
{

namespace Platform
{


Timer::~Timer()
{
}

TimerPtr Timer::Create()
{
    return std::make_shared<Win32Timer>();
}

void Timer::MeasureFrame()
{
    /* Measure frame rate */
    Stop();
    auto elapsed = GetElapsedTime();
    Start();

    /* Store delta time and frame rate */
    static const double frequency = 1000000.0;

    deltaTime_ = elapsed / frequency;
    frameRate_ = frequency / elapsed;

    /* Increase frame counter */
    ++frameCount_;
}

void Timer::ResetFrameCount()
{
    frameCount_ = 0;
}


} // /namespace Platform

} // /namespace Fork



// ========================