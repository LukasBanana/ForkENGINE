/*
 * XAudio2 sound system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XA2_SOUND_SYSTEM_H__
#define __FORK_XA2_SOUND_SYSTEM_H__


#include "Audio/SoundSystem/SoundSystem.h"

#include <xaudio2.h>


namespace Fork
{

namespace Audio
{


class XA2SourceVoice;

//! XAudio2 sound system implementation.
class XA2SoundSystem : public SoundSystem
{
    
    public:
        
        XA2SoundSystem(const XA2SoundSystem&) = delete;
        XA2SoundSystem& operator = (const XA2SoundSystem&) = delete;

        XA2SoundSystem();
        ~XA2SoundSystem();

        BufferSoundSourcePtr LoadBufferSound(const std::string& filename) override;
        StreamSound LoadStreamSound(const std::string& filename) override;

    private:
        
        IXAudio2*               device_         = nullptr;
        IXAudio2MasteringVoice* masteringVoice_ = nullptr;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================