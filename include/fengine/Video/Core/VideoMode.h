/*
 * Video mode header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VIDEO_MODE_H__
#define __FORK_VIDEO_MODE_H__


#include "Math/Core/Size2.h"
#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Video
{


/**
Video mode structure. For less- or greater comparisions the order
of preferential treatment is: 'resolution', 'colorBitDepth', 'isFullscreen'.
*/
struct FORK_EXPORT VideoMode
{
    VideoMode() = default;
    VideoMode(const Math::Size2ui& vmResolution, int vmColorDepth = 32, bool vmIsFullscreen = false);

    bool operator == (const VideoMode& other) const;
    bool operator != (const VideoMode& other) const;
    
    //! Compares the video modes by resolution, colorBitDepth and isFullscreen.
    bool operator < (const VideoMode& other) const;
    bool operator <= (const VideoMode& other) const;

    bool operator > (const VideoMode& other) const;
    bool operator >= (const VideoMode& other) const;

    //! Returns a resolution of 1280 x 720.
    static Math::Size2ui HDReady();
    //! Returns a resolution of 1920 x 1080.
    static Math::Size2ui FullHD();
    //! Returns a resolution of 3840 x 2160.
    static Math::Size2ui UltraHD();

    Math::Size2ui   resolution;                 //!< Screen resolution.
    int             colorBitDepth   = 0;        //!< Color bit depth. Should be 16, 24 or 32.
    bool            isFullscreen    = false;    //!< Specifies whether fullscreen- or window mode is enabled.
};


//! Display device identification structure.
struct FORK_EXPORT DisplayDevice
{
    DisplayDevice() = default;
    DisplayDevice(const std::wstring& videoController, const std::wstring& monitor);

    std::wstring videoControllerID; //!< Video controll identification string (Graphics card or on-board graphics).
    std::wstring monitorID;         //!< Monitor identification string.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================