/*
 * Scoped timer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_SCOPED_TIMER_H__
#define __FORK_IO_SCOPED_TIMER_H__


#include "Platform/Core/Timer.h"


namespace Fork
{

namespace IO
{


/**
Scoped timer class. This is a comfort class to easily measure time sections.
\code
// Usage example:
doubel timerValue = 0.0;
{
    IO::ScopedTimer timer(*engine.GetTemporaryTimer(), timerValue);
    // Do something time intensive here ...
}
std::cout << "Elapsed Time: " << timerValue;
\endcode
\see Platform::Timer
\see LogScopedTimer
*/
class ScopedTimer
{
    
    public:
        
        inline ScopedTimer(Platform::Timer& timer, double& value) :
            timer_{ timer },
            value_{ value }
        {
            timer_.Start();
        }
        inline ~ScopedTimer()
        {
            timer_.Stop();
            value_ = timer_.GetElapsedTime();
        }

    private:
        
        Platform::Timer&    timer_;
        double&             value_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================