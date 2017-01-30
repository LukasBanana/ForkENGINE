/*
 * Blink model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/BlinkModel.h"


namespace Fork
{

namespace Utility
{


BlinkModel::BlinkModel()
{
    Refresh();
}

bool BlinkModel::IsVisible() const
{
    const auto timePoint = std::chrono::system_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint - lastTime_).count();

    if (duration < timeInterval || timeInterval <= 0)
        return true;
    else if (duration > timeInterval*2)
        lastTime_ = timePoint;

    return false;
}

void BlinkModel::Refresh()
{
    lastTime_ = std::chrono::system_clock::now();
}


} // /namespace Utility

} // /namespace Fork



// ========================