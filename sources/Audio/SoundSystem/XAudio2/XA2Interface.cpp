/*
 * XAudio2 sound system library interface file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Interface.h"
#include "XA2SoundSystem.h"


extern "C"
{

/*
Allocate the XAudio2 sound systems and return the raw pointer.
-> This is an exception in memory management of this engine.
   All other memory objects must be created with managed memory (i.e. std::shared_ptr)!
*/

FORK_EXPORT void* AllocSoundSystem()
{
    return new Fork::Audio::XA2SoundSystem();
}

FORK_EXPORT const char* GetSoundSystemName()
{
    return "XAudio2";
}

}



// ========================