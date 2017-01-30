/*
 * XAudio2 stream sound source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2StreamSoundSource.h"
#include "Math/Core/BaseMath.h"

#include <algorithm>


namespace Fork
{

namespace Audio
{


XA2StreamSoundSource::XA2StreamSoundSource(IXAudio2* device, const WaveFormat& waveFormat, double totalTime) :
    StreamSoundSource   { waveFormat         },
    XA2SoundSource      { device, waveFormat }
{
    this->totalTime = totalTime;
}
XA2StreamSoundSource::~XA2StreamSoundSource()
{
    Stop();
}

void XA2StreamSoundSource::Play(unsigned int loopCount)
{
    Stop();
    sourceVoice.Start();
}

bool XA2StreamSoundSource::SubmitBuffer(const PCMBuffer& pcmBuffer, size_t bufferLimit, unsigned int loopCount)
{
    return sourceVoice.SubmitBuffer(pcmBuffer.data(), std::min(pcmBuffer.size(), bufferLimit), loopCount);
}

unsigned int XA2StreamSoundSource::QueueSize() const
{
    return sourceVoice.State().BuffersQueued;
}

unsigned int XA2StreamSoundSource::MaxQueueSize() const
{
    return XAUDIO2_MAX_QUEUED_BUFFERS;
}


} // /namespace Audio

} // /namespace Fork



// ========================