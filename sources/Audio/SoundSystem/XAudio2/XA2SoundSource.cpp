/*
 * XAudio2 sound source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2SoundSource.h"
#include "Math/Core/BaseMath.h"


namespace Fork
{

namespace Audio
{


XA2SoundSource::XA2SoundSource(IXAudio2* device, const WaveFormat& waveFormat) :
    sourceVoice{ device, XA2SourceVoice::ConvertWaveFormat(waveFormat) }
{
}
XA2SoundSource::~XA2SoundSource()
{
}

void XA2SoundSource::Pause(bool isPaused)
{
    if (this->isPaused != isPaused && IsPlaying())
    {
        if (isPaused)
            sourceVoice.Stop();
        else
            sourceVoice.Start();
        this->isPaused = isPaused;
    }
}

void XA2SoundSource::Stop()
{
    sourceVoice.Stop();
    sourceVoice.ClearBuffers();
    isPaused = false;
}

void XA2SoundSource::SetupVolume(float volume)
{
    sourceVoice.SetupVolume(volume);
    this->volume = volume;
}

float XA2SoundSource::Volume() const
{
    return volume;
}

void XA2SoundSource::SetupBalance(float balance)
{
    balance = Math::Clamp(balance, -1.0f, 1.0f);
    sourceVoice.SetupBalance(balance);
    this->balance = balance;
}

float XA2SoundSource::Balance() const
{
    return balance;
}

void XA2SoundSource::SetupFrequencyRatio(float ratio)
{
    ratio = Math::Clamp(ratio, XAUDIO2_MIN_FREQ_RATIO, XAUDIO2_MAX_FREQ_RATIO);
    sourceVoice.SetupFrequencyRatio(ratio);
    frequencyRatio = ratio;
}

float XA2SoundSource::FrequencyRatio() const
{
    return frequencyRatio;
}

bool XA2SoundSource::IsPlaying() const
{
    return sourceVoice.IsPlaying();
}

bool XA2SoundSource::IsPaused() const
{
    return isPaused;
}

double XA2SoundSource::SeekPosition() const
{
    /*
    Compute elapsed time since source voice was started,
    by the played samples and the sample rate.
    */
    if (totalTime > 0.0)
    {
        auto samplesPlayed  = sourceVoice.State().SamplesPlayed;
        auto sampleRate     = sourceVoice.Details().InputSampleRate;
        auto time           = static_cast<double>(samplesPlayed) / static_cast<double>(sampleRate);
        return time / totalTime;
    }
    return 0.0;
}

double XA2SoundSource::TotalTime() const
{
    return totalTime;
}


} // /namespace Audio

} // /namespace Fork



// ========================