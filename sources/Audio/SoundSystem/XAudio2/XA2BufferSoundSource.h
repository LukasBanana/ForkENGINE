/*
 * XAudio2 buffer sound source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XA2_BUFFER_SOUND_SOURCE_H__
#define __FORK_XA2_BUFFER_SOUND_SOURCE_H__


#include "Audio/SoundSystem/BufferSoundSource.h"
#include "XA2SoundSource.h"
#include "XA2SourceVoice.h"


namespace Fork
{

namespace Audio
{


//! Buffer sound source interface.
class XA2BufferSoundSource : public XA2SoundSource, public BufferSoundSource
{
    
    public:
        
        XA2BufferSoundSource(IXAudio2* device, const WaveBufferPtr& waveBuffer);
        ~XA2BufferSoundSource();

        void Play(unsigned int loopCount = 0) override;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================