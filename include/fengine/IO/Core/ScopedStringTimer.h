/*
 * Scoped string timer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_SCOPED_STRING_TIMER_H__
#define __FORK_IO_SCOPED_STRING_TIMER_H__


#include "Core/Export.h"
#include "Platform/Core/Timer.h"

#include <string>


namespace Fork
{

namespace IO
{


/**
Scoped string timer class. This is a comfort class to easily measure time sections and convert the result to a string.
\code
// Usage example:
std::string timerValue;
{
    IO::ScopedStringTimer timer(*engine.GetTemporaryTimer(), timerValue);
    // Do something time intensive here ...
}
std::cout << "Elapsed Time: " << timerValue;
\endcode
\see Platform::Timer
\see ScopedTimer
*/
class FORK_EXPORT ScopedStringTimer
{
    
    public:
        
        //! Time scale units.
        enum class Units
        {
            MicroSecs,  //!< Microseconds.
            MilliSecs,  //!< Milliseconds.
            Secs,       //!< Seconds.
        };

        ScopedStringTimer(Platform::Timer& timer, std::string& value, const Units unit = Units::MilliSecs);
        ~ScopedStringTimer();

    private:
        
        Platform::Timer&    timer_;
        std::string&        value_;
        Units               unit_   = Units::MilliSecs;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================