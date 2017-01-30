/*
 * WIN32: Cursor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/Cursor.h"
#include "Platform/Core/Desktop.h"
#include "Win32Frame.h"

#include <Windows.h>


namespace Fork
{

namespace Platform
{

namespace Cursor
{


FORK_EXPORT void SetupPosition(const Math::Point2i& position)
{
    SetCursorPos(position.x, position.y);
}

FORK_EXPORT void SetupPosition(const Math::Point2i& position, const Frame* frame, bool useClientAreaOrigin)
{
    auto relativePosition = position;

    if (frame)
    {
        /* Make position relative to frame position */
        relativePosition += frame->Position();

        /* Make position relative to frame's client area origin */
        if (useClientAreaOrigin)
            relativePosition += frame->ClientAreaOffset();
    }

    SetupPosition(relativePosition);
}

FORK_EXPORT Math::Point2i Position()
{
    POINT position;
    GetCursorPos(&position);
    return { position.x, position.y };
}

FORK_EXPORT Math::Point2i Position(const Frame* frame, bool useClientAreaOrigin)
{
    auto position = Position();

    if (frame)
    {
        /* Make position relative to frame position */
        position -= frame->Position();

        /* Make position relative to frame's client area origin */
        if (useClientAreaOrigin)
            position -= frame->ClientAreaOffset();
    }

    return position;
}

FORK_EXPORT void Clamp(const Math::Recti& rect)
{
    RECT rc;
    {
        rc.left     = rect.left;
        rc.top      = rect.top;
        rc.right    = rect.right;
        rc.bottom   = rect.bottom;
    }
    ClipCursor(&rc);
}

FORK_EXPORT void Unclamp()
{
    ClipCursor(nullptr);
}

FORK_EXPORT void Show(bool show)
{
    static bool isVisible = true;
    if (isVisible != show)
    {
        isVisible = show;
        ShowCursor(isVisible);
    }
}


} // /namespace Cursor

} // /namespace Platform

} // /namespace Fork



// ========================