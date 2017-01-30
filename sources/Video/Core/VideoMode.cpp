/*
 * Video mode file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Core/VideoMode.h"


namespace Fork
{

namespace Video
{


/* --- VideoMode structure --- */

VideoMode::VideoMode(const Math::Size2ui& vmResolution, int vmColorDepth, bool vmIsFullscreen) :
    resolution      { vmResolution   },
    colorBitDepth   { vmColorDepth   },
    isFullscreen    { vmIsFullscreen }
{
}

bool VideoMode::operator == (const VideoMode& other) const
{
    return
        resolution      == other.resolution     &&
        colorBitDepth   == other.colorBitDepth  &&
        isFullscreen    == other.isFullscreen;
}

bool VideoMode::operator != (const VideoMode& other) const
{
    return !(*this == other);
}

bool VideoMode::operator < (const VideoMode& other) const
{
    if (resolution < other.resolution)
        return true;
    if (resolution > other.resolution)
        return false;

    if (colorBitDepth < other.colorBitDepth)
        return true;
    if (colorBitDepth > other.colorBitDepth)
        return false;

    return !isFullscreen && other.isFullscreen;
}

bool VideoMode::operator <= (const VideoMode& other) const
{
    return *this < other || *this == other;
}

bool VideoMode::operator > (const VideoMode& other) const
{
    if (resolution > other.resolution)
        return true;
    if (resolution < other.resolution)
        return false;

    if (colorBitDepth > other.colorBitDepth)
        return true;
    if (colorBitDepth < other.colorBitDepth)
        return false;

    return isFullscreen && !other.isFullscreen;
}

bool VideoMode::operator >= (const VideoMode& other) const
{
    return *this > other || *this == other;
}

Math::Size2ui VideoMode::HDReady()
{
    return { 1280, 720 };
}

Math::Size2ui VideoMode::FullHD()
{
    return { 1920, 1080 };
}

Math::Size2ui VideoMode::UltraHD()
{
    return { 3840, 2160 };
}


/* --- DisplayDevice structure --- */

DisplayDevice::DisplayDevice(const std::wstring& videoController, const std::wstring& monitor) :
    videoControllerID   { videoController },
    monitorID           { monitor         }
{
}


} // /namespace Video

} // /namespace Fork



// ========================