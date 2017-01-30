/*
 * XAudio2 source voice file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2SourceVoice.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/STLHelper.h"
#include "Math/Core/BaseMath.h"

#include <algorithm>
#include <exception>


namespace Fork
{

namespace Audio
{


XA2SourceVoice::XA2SourceVoice(IXAudio2* device, const WAVEFORMATEX& waveFormat) :
    voiceCallback_  { bufferQueue_         },
    channels_       { waveFormat.nChannels }
{
    ASSERT_POINTER(device);

    /* Create source voice with wave format */
    if (FAILED(device->CreateSourceVoice(&sourceVoice_, &waveFormat, 0, 2.0f, &voiceCallback_)))
        throw std::runtime_error("Creating XAudio2 source voice failed");
}
XA2SourceVoice::~XA2SourceVoice()
{
    sourceVoice_->DestroyVoice();
}

WAVEFORMATEX XA2SourceVoice::ConvertWaveFormat(const WaveFormat& waveFormat)
{
    /* Setup wave format structure */
    WAVEFORMATEX result;
    InitRawMemory(result);
    CopyRawMemory(result, waveFormat);
    return result;
}

bool XA2SourceVoice::SubmitBuffer(const PCMBuffer& buffer, unsigned int loopCount)
{
    return SubmitBuffer(buffer.data(), buffer.size(), loopCount);
}

bool XA2SourceVoice::SubmitBuffer(const char* buffer, size_t bufferSize, unsigned int loopCount)
{
    /* Check if there are currently too many buffers queued */
    if (State().BuffersQueued >= XAUDIO2_MAX_QUEUED_BUFFERS)
        return false;

    /* Copy buffer to internal queued buffer */
    PCMBuffer pcmBuffer;
    pcmBuffer.insert(pcmBuffer.end(), buffer, buffer + bufferSize);

    /* Setup XAudio2 buffer description */
    XAUDIO2_BUFFER bufferDesc;
    InitRawMemory(bufferDesc);

    bufferDesc.AudioBytes = bufferSize;
    bufferDesc.pAudioData = reinterpret_cast<const BYTE*>(pcmBuffer.data());

    if (loopCount > 0)
    {
        /* Setup loop playback */
        if (loopCount == infiniteSoundLoop)
            bufferDesc.LoopCount = XAUDIO2_LOOP_INFINITE;
        else
            bufferDesc.LoopCount = std::min(loopCount, unsigned(XAUDIO2_MAX_LOOP_COUNT));
    }

    /* Push buffer onto queue */
    bufferQueue_.push(std::move(pcmBuffer));

    /* Submit buffer to source voice */
    sourceVoice_->SubmitSourceBuffer(&bufferDesc);

    return true;
}

void XA2SourceVoice::ClearBuffers()
{
    sourceVoice_->FlushSourceBuffers();
}

void XA2SourceVoice::SetupVolume(float volume)
{
    sourceVoice_->SetVolume(std::max(0.0f, volume));
}

void XA2SourceVoice::SetupBalance(float balance)
{
    if (channels_ == 2)
    {
        balance = Math::Clamp(balance, -1.0f, 1.0f);

        float volumes[2] = { 1.0f - std::max(0.0f, balance), std::min(1.0f, balance + 1.0f) };
        sourceVoice_->SetChannelVolumes(2, volumes);
    }
}

void XA2SourceVoice::SetupFrequencyRatio(float ratio)
{
    sourceVoice_->SetFrequencyRatio(Math::Clamp(ratio, XAUDIO2_MIN_FREQ_RATIO, XAUDIO2_MAX_FREQ_RATIO));
}

XAUDIO2_VOICE_STATE XA2SourceVoice::State() const
{
    XAUDIO2_VOICE_STATE state;
    sourceVoice_->GetState(&state);
    return state;
}

XAUDIO2_VOICE_DETAILS XA2SourceVoice::Details() const
{
    XAUDIO2_VOICE_DETAILS details;
    sourceVoice_->GetVoiceDetails(&details);
    return details;
}

bool XA2SourceVoice::IsPlaying() const
{
    return State().BuffersQueued > 0;
}


} // /namespace Audio

} // /namespace Fork



// ========================