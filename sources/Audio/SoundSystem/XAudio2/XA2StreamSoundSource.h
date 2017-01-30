/*
 * XAudio2 stream sound source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XA2_STREAM_SOUND_SOURCE_H__
#define __FORK_XA2_STREAM_SOUND_SOURCE_H__


#include "Audio/SoundSystem/StreamSoundSource.h"
#include "XA2SoundSource.h"
#include "XA2SourceVoice.h"


namespace Fork
{

namespace Audio
{


//! Buffer sound source interface.
class XA2StreamSoundSource : public XA2SoundSource, public StreamSoundSource
{
    
    public:
        
        XA2StreamSoundSource(IXAudio2* device, const WaveFormat& waveFormat, double totalTime);
        ~XA2StreamSoundSource();

        void Play(unsigned int loopCount = 0) override;

        bool SubmitBuffer(
            const PCMBuffer& pcmBuffer, size_t bufferLimit = size_t(-1), unsigned int loopCount = 0
        ) override;

        unsigned int QueueSize() const override;
        unsigned int MaxQueueSize() const override;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================