/*
 * Sound system library interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUNDSYSTEM_LIB_INTERFACE_H__
#define __FORK_SOUNDSYSTEM_LIB_INTERFACE_H__


#include "Core/Export.h"


extern "C"
{

/**
Returns a raw-pointer to the allocated sound system.
This should be wrapped into a smart pointer:
\code
auto soundSystem = std::shared_ptr<SoundSystem>(AllocSoundSystem());
\endcode
*/
FORK_EXPORT void* AllocSoundSystem();

/**
Returns a null terminated UTF-8 string
which describes the name of the sound system.
*/
FORK_EXPORT const char* GetSoundSystemName();

}


#endif



// ========================