/*
 * XAudio2 source voice header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XA2_SOURCE_VOICE_H__
#define __FORK_XA2_SOURCE_VOICE_H__


#include "Audio/Core/WaveBuffer.h"
#include "XA2VoiceCallback.h"

#include <xaudio2.h>
#include <queue>
#include <memory>


namespace Fork
{

namespace Audio
{


//! XAudio2 source voice wrapper class.
class XA2SourceVoice
{
    
    public:
        
        XA2SourceVoice(const XA2SourceVoice&) = delete;
        XA2SourceVoice& operator = (const XA2SourceVoice&) = delete;

        /**
        Constructs the XAudio2 source voice.
        \throws NullPointerException If 'device' is null.
        \throws std::runtime_error If the source voice creation failed.
        */
        XA2SourceVoice(IXAudio2* device, const WAVEFORMATEX& waveFormat);
        ~XA2SourceVoice();

        //! Converts the engine's wave format into a WAVEFORMATEX structure.
        static WAVEFORMATEX ConvertWaveFormat(const WaveFormat& waveFormat);

        bool SubmitBuffer(const PCMBuffer& buffer, unsigned int loopCount = 0);
        bool SubmitBuffer(const char* buffer, size_t bufferSize, unsigned int loopCount = 0);
        void ClearBuffers();

        void SetupVolume(float volume);
        void SetupBalance(float balance);
        void SetupFrequencyRatio(float ratio);

        XAUDIO2_VOICE_STATE State() const;
        XAUDIO2_VOICE_DETAILS Details() const;

        bool IsPlaying() const;

        inline void Start()
        {
            sourceVoice_->Start();
        }
        inline void Stop()
        {
            sourceVoice_->Stop();
        }

    private:
        
        PCMBufferQueue          bufferQueue_;

        IXAudio2SourceVoice*    sourceVoice_    = nullptr;
        XA2VoiceCallback        voiceCallback_;
        WORD                    channels_       = 1;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================