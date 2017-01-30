/*
 * WIN32: Desktop file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/Desktop.h"

#include <Windows.h>


namespace Fork
{

namespace Platform
{

namespace Desktop
{


static bool isFullscreen = false;

FORK_EXPORT Math::Size2ui QueryResolution()
{
    return { static_cast<unsigned int>(GetSystemMetrics(SM_CXSCREEN)),
             static_cast<unsigned int>(GetSystemMetrics(SM_CYSCREEN)) };
}

FORK_EXPORT int QueryColorDepth()
{
    HWND desktopWindowHandle = GetDesktopWindow();
    HDC dc = GetDC(desktopWindowHandle);

    int bpp = GetDeviceCaps(dc, BITSPIXEL);
    int planes = GetDeviceCaps(dc, PLANES);

    if (planes > 1)
        bpp = (1 << planes);

    return bpp;
}

FORK_EXPORT unsigned int QueryNumMonitors()
{
    return static_cast<unsigned int>(GetSystemMetrics(SM_CMONITORS));
}

FORK_EXPORT bool ChangeVideoMode(const Video::VideoMode& videoMode)
{
    LONG result = 0;

    if (videoMode.isFullscreen)
    {
        DEVMODE config;
        memset(&config, 0, sizeof(DEVMODE));
        {
            config.dmSize       = sizeof(DEVMODE);
            config.dmPelsWidth  = videoMode.resolution.width;
            config.dmPelsHeight = videoMode.resolution.height;
            config.dmBitsPerPel = videoMode.colorBitDepth;
            config.dmFields     = (DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT);
        }
        result = ChangeDisplaySettings(&config, CDS_FULLSCREEN);
    }
    else if (isFullscreen)
        result = ChangeDisplaySettings(nullptr, 0);

    isFullscreen = videoMode.isFullscreen;

    return (result == DISP_CHANGE_SUCCESSFUL);
}

FORK_EXPORT bool ResetVideoMode()
{
    return ChangeDisplaySettings(nullptr, 0) == DISP_CHANGE_SUCCESSFUL;
}


} // /namespace Desktop

} // /namespace Platform

} // /namespace Fork



// ========================