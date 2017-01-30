/*
 * System clock header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_SYSTEM_CLOCK_H__
#define __FORK_IO_SYSTEM_CLOCK_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace IO
{

//! System clock namespace for time and date information.
namespace SystemClock
{


//! Returns the current time point (including the current date) as string.
FORK_EXPORT std::string CurrentTimePoint();

//! Resets the elapsed time (in seconds) since the program has started.
FORK_EXPORT void ResetElapsedTime();

//! Returns the elapsed time (in seconds) since the program has started.
FORK_EXPORT long long ElapsedTime();

/**
Converts the specified time (in seconds) into a timepoint as string.
This string has the format: "hh:mm:ss" if 'areHoursOptional' is false or the hours are greater than zero, "mm:ss" otherwise.
*/
FORK_EXPORT std::string SecondsToTimePoint(long long time, bool areHoursOptional = true);


} // /namespace Console

} // /namespace IO

} // /namespace Fork


#endif



// ========================