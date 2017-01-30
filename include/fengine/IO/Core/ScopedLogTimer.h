/*
 * Scoped log timer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_SCOPED_LOG_TIMER_H__
#define __FORK_IO_SCOPED_LOG_TIMER_H__


#include "Core/Export.h"
#include "Platform/Core/Timer.h"

#include <string>


namespace Fork
{

namespace IO
{


/**
Scoped log timer class. This is a comfort class to easily measure time sections and immediately output the result.
\code
// Usage example:
{
    IO::ScopedLogTimer timer(*engine.GetTemporaryTimer());
    // Do something time intensive here ...
    // At the end of this scope, the duration is printed to the log
}
\endcode
\see Platform::Timer
\see ScopedTimer
*/
class FORK_EXPORT ScopedLogTimer
{
    
    public:
        
        ScopedLogTimer(Platform::Timer& timer, const std::string& info = "Duration: ");
        ~ScopedLogTimer();

    private:
        
        Platform::Timer&    timer_;
        std::string         info_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================