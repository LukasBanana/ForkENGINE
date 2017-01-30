/*
 * Desktop header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_DESKTOP_H__
#define __FORK_PLATFORM_DESKTOP_H__


#include "Math/Core/Size2.h"
#include "Video/Core/VideoMode.h"
#include "Core/Export.h"


namespace Fork
{

namespace Platform
{

namespace Desktop
{


//! Returns the desktop resolution.
FORK_EXPORT Math::Size2ui QueryResolution();

//! Returns the desktop color depth (bits per pixel).
FORK_EXPORT int QueryColorDepth();

//! Returns the number of display monitors on the desktop.
FORK_EXPORT unsigned int QueryNumMonitors();

//! Sets the new specified video mode for the desktop (resolution and fullscreen mode).
FORK_EXPORT bool ChangeVideoMode(const Video::VideoMode& videoMode);

//! Resets the original video mode for the desktop.
FORK_EXPORT bool ResetVideoMode();


} // /namespace Desktop

} // /namespace Platform

} // /namespace Fork


#endif



// ========================