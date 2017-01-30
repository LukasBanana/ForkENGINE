/*
 * Scoped log timer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/ScopedLogTimer.h"
#include "IO/Core/Log.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace IO
{


ScopedLogTimer::ScopedLogTimer(Platform::Timer& timer, const std::string& info) :
    timer_  { timer },
    info_   { info  }
{
    timer_.Start();
}
ScopedLogTimer::~ScopedLogTimer()
{
    timer_.Stop();
    auto duration = static_cast<long long>(timer_.GetElapsedTime());
    IO::Log::Message(
        info_ + ToStr(duration / 1000) + "." +
        NumberOffset(static_cast<size_t>(duration % 1000), 999, '0') + "ms"
    );
}


} // /namespace IO

} // /namespace Fork



// ========================