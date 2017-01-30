/*
 * System clock file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/SystemClock.h"
#include "Core/StringModifier.h"

#include <chrono>
#include <ctime>


namespace Fork
{

namespace IO
{

namespace SystemClock
{


static auto programStartTime = std::chrono::steady_clock::now();

FORK_EXPORT std::string CurrentTimePoint()
{
    /* Determine current time point */
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch());
    const auto date = duration.count();
    
    /* Get time point as string */
    auto timePoint = std::string(std::ctime(&date));

    /* Remove new-line character at the end */
    if (!timePoint.empty() && timePoint.back() == '\n')
        timePoint.resize(timePoint.size() - 1);

    return timePoint;
}

FORK_EXPORT void ResetElapsedTime()
{
    programStartTime = std::chrono::steady_clock::now();
}

FORK_EXPORT long long ElapsedTime()
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - programStartTime).count();
}

FORK_EXPORT std::string SecondsToTimePoint(long long time, bool areHoursOptional)
{
    auto sec = time % 60;
    auto min = (time / 60) % 60;
    auto hour = (time / 60 / 60);

    std::string str;

    if (!areHoursOptional || hour > 0)
        str = NumberOffset(static_cast<size_t>(hour), 99, '0') + ':';

    str += NumberOffset(static_cast<size_t>(min), 99, '0') + ':';
    str += NumberOffset(static_cast<size_t>(sec), 99, '0');

    return str;
}


} // /namespace Console

} // /namespace IO

} // /namespace Fork



// ========================