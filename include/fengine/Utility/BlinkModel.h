/*
 * Blink model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_BLINK_MODEL_H__
#define __FORK_UTILITY_BLINK_MODEL_H__


#include "Core/Export.h"

#include <chrono>


namespace Fork
{

namespace Utility
{


/**
Base class for blinking objects in a user interface.
This is used by the TerminalView for the cursor blinking.
\see TerminalView
\note This can not be used for multi-threading because of mutable members!
*/
class FORK_EXPORT BlinkModel
{

    public:
        
        BlinkModel();

        /**
        Returns true if the blinking model is currently visible.
        \remarks This will be true for a time interval specified by 'timeInterval' and false for the next interval.
        E.g. if 'timeInterval' is 1000, the blinking model will be visible for one second and invisible
        for the next second, then visible again and so on ...
        \see timeInterval
        */
        bool IsVisible() const;

        //! Refreshes the last blink time.
        void Refresh();

        /**
        Cursor blink time interval (in milliseconds).
        If this <= 0, the cursor will not blink. By default 500.
        */
        long long timeInterval = 500;

    private:
        
        mutable std::chrono::system_clock::time_point lastTime_; //!< Last time point the cursor has blinked.

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================