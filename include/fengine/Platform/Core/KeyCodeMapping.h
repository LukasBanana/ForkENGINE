/*
 * Key code mapping header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_KEYCODE_MAPPING_H__
#define __FORK_PLATFORM_KEYCODE_MAPPING_H__


#include "Core/Export.h"
#include "IO/InputDevice/KeyCodes.h"


namespace Fork
{

namespace Platform
{

namespace KeyCodeMapping
{


/**
Returns the engine key code for the specified respective system key code.
\param[in] sysKeyCode Specifies the system key code.
On MS/Windows for instance the key code 48 (or rather the ASCII character '0') will result in IO::KeyCodes::Key0.
*/
FORK_EXPORT IO::KeyCodes MapKeyCode(unsigned char sysKeyCode);


} // /namespace KeyCodeMapping

} // /namespace Platform

} // /namespace Fork


#endif



// ========================