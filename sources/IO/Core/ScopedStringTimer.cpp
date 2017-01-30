/*
 * Scoped string timer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/ScopedStringTimer.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace IO
{


ScopedStringTimer::ScopedStringTimer(Platform::Timer& timer, std::string& value, const Units unit) :
    timer_  { timer },
    value_  { value },
    unit_   { unit  }
{
    timer_.Start();
}
ScopedStringTimer::~ScopedStringTimer()
{
    timer_.Stop();
    
    auto duration = timer_.GetElapsedTime();

    switch (unit_)
    {
        case Units::MicroSecs:
            value_ = ToStr(duration);
            break;
        case Units::MilliSecs:
            value_ = ToStr(duration / 1000.0, 4);
            break;
        case Units::Secs:
            value_ = ToStr(duration / 1000000.0, 8);
            break;
    }
}


} // /namespace IO

} // /namespace Fork



// ========================