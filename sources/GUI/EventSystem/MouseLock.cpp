/*
 * GUI mouse lock file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MouseLock.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace GUI
{


MouseLock::MouseLock()
{
    Unclamp();
}

void MouseLock::Clamp(const Rect& rect)
{
    clampRect = rect;
}

void MouseLock::Unclamp()
{
    clampRect = { 0, 0, -1, -1 };
}

void MouseLock::ClampPoint(Point& point)
{
    if (clampRect.left <= clampRect.right && clampRect.top <= clampRect.bottom)
    {
        point.x = Math::Clamp(point.x, clampRect.left, clampRect.right);
        point.y = Math::Clamp(point.y, clampRect.top, clampRect.bottom);
    }
}

MouseLock& GetMouseLock()
{
    static MouseLock mouseLock;
    return mouseLock;
}


} // /namespace GUI

} // /namespace Fork



// ========================