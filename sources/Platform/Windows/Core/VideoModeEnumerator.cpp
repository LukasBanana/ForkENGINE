/*
 * WIN32: Video mode enumerator file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/VideoModeEnumerator.h"
#include "Core/StringModifier.h"
#include "Core/STLHelper.h"

#include <Windows.h>
#include <algorithm>


namespace Fork
{

namespace Platform
{


VideoModeEnumerator::VideoModeEnumerator()
{
    /* Enumerate supported display settings */
    DEVMODE devMode;

    /* Enumerate connected display devices */
    DISPLAY_DEVICE device, monitor;
    
    InitRawMemory(device);
    device.cb = sizeof(DISPLAY_DEVICE);
    
    InitRawMemory(monitor);
    monitor.cb = sizeof(DISPLAY_DEVICE);
    
    for (int i = 0; EnumDisplayDevices(0, i, &device, 0); ++i)
    {
        for (int j = 0; EnumDisplaySettings(device.DeviceName, j, &devMode); ++j)
        {
            Video::VideoMode vidMode;
            {
                vidMode.resolution.width    = devMode.dmPelsWidth;
                vidMode.resolution.height   = devMode.dmPelsHeight;
                vidMode.colorBitDepth       = devMode.dmBitsPerPel;
            }
            videoModes_.push_back(vidMode);
        }

        for (int j = 0; EnumDisplayDevices(device.DeviceName, j, &monitor, 0); ++j)
        {
            Video::DisplayDevice displayDev;
            {
                displayDev.videoControllerID    = std::wstring(device.DeviceString);
                displayDev.monitorID            = std::wstring(monitor.DeviceString);
            }
            displayDevices_.push_back(displayDev);
        }
    }

    /* Copy video modes into the vector for faster access */
    std::sort(videoModes_.begin(), videoModes_.end());
    std::unique(videoModes_.begin(), videoModes_.end());
}

Video::VideoMode VideoModeEnumerator::FindSmallestVideoMode() const
{
    return videoModes_.empty() ? Video::VideoMode() : videoModes_.front();
}

Video::VideoMode VideoModeEnumerator::FindLargestVideoMode() const
{
    return videoModes_.empty() ? Video::VideoMode() : videoModes_.back();
}


} // /namespace Platform

} // /namespace Fork



// ========================