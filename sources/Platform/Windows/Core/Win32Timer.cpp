/*
 * WIN32: Win32 timer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32Timer.h"

#include <algorithm>


namespace Fork
{

namespace Platform
{


/*
Specifies whether to enable the adjustment for unexpected leaps in the Win32 performance counter.
This is caused by unexpected data across the PCI to ISA bridge, aka south bridge. See Microsoft KB274323.
*/
#define ENABLE_LEAP_FORWARD_ADJUSTMENT

Win32Timer::Win32Timer()
{
    QueryPerformanceFrequency(&clockFrequency_);
}

void Win32Timer::Start()
{
    /* Query current performance counter ticks */
    QueryPerformanceCounter(&t0_);

    #ifdef ENABLE_LEAP_FORWARD_ADJUSTMENT
    startTick_ = GetTickCount();
    #endif
}

void Win32Timer::Stop()
{
    /* Querry elapsed ticks */
    QueryPerformanceCounter(&t1_);
    auto elapsedTime = t1_.QuadPart - t0_.QuadPart;
    
    #ifdef ENABLE_LEAP_FORWARD_ADJUSTMENT

    /* Compute the number of millisecond ticks elapsed */
    long long msecTicks = static_cast<long long>(1000 * elapsedTime / clockFrequency_.QuadPart);
    
    /* Check for unexpected leaps */
    long long elapsedLowTicks = static_cast<long long>(GetTickCount() - startTick_);
    auto msecOff = msecTicks - elapsedLowTicks;
    
    if (std::abs(msecOff) > 100)
    {
        /* Adjust the starting time forwards */
        LONGLONG msecAdjustment = std::min<LONGLONG>(
            ( msecOff * clockFrequency_.QuadPart / 1000 ),
            ( elapsedTime - prevElapsedTime_ )
        );
        elapsedTime -= msecAdjustment;
    }
    
    /* Store the current elapsed time for adjustments next time */
    prevElapsedTime_ = elapsedTime;

    #endif

    /* Store final elapsed time */
    SetElapsedTime(1000000.0 * elapsedTime / clockFrequency_.QuadPart);
}


} // /namespace Platform

} // /namespace Fork



// ========================