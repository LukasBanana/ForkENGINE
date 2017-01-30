/*
 * XAudio2 buffer sound source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2BufferSoundSource.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace Audio
{


XA2BufferSoundSource::XA2BufferSoundSource(IXAudio2* device, const WaveBufferPtr& waveBuffer) :
    XA2SoundSource      { device, waveBuffer->format },
    BufferSoundSource   { waveBuffer                 }
{
    totalTime = waveBuffer->TotalTime();
}
XA2BufferSoundSource::~XA2BufferSoundSource()
{
    Stop();
}

void XA2BufferSoundSource::Play(unsigned int loopCount)
{
    Stop();
    sourceVoice.SubmitBuffer(GetWaveBuffer()->buffer, loopCount);
    sourceVoice.Start();
}


} // /namespace Audio

} // /namespace Fork



// ========================