/*
 * WIN32 Win32 timer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_WIN32TIMER_H__
#define __FORK_PLATFORM_WIN32TIMER_H__


#include "Platform/Core/Timer.h"
#include "Core/StaticConfig.h"

#include <Windows.h>


namespace Fork
{

namespace Platform
{


class Win32Timer : public Timer
{
    
    public:
        
        Win32Timer();

        void Start() override;
        void Stop() override;

    private:
        
        LARGE_INTEGER   clockFrequency_;
        LARGE_INTEGER   t0_;
        LARGE_INTEGER   t1_;

        DWORD           startTick_          = 0;
        LONGLONG        prevElapsedTime_    = 0;

};


} // /namespace Platform

} // /namespace Fork


#endif



// ========================