/*
 * XAudio2 sound source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_XA2_SOUND_SOURCE_H__
#define __FORK_XA2_SOUND_SOURCE_H__


#include "Audio/SoundSystem/SoundSource.h"
#include "XA2SourceVoice.h"


namespace Fork
{

namespace Audio
{


//! XAudio2 sound source base class.
class XA2SoundSource : public virtual SoundSource
{
    
    public:
        
        virtual ~XA2SoundSource();

        void Pause(bool isPaused = true) override;
        void Stop() override;

        void SetupVolume(float volume) override;
        float Volume() const override;

        void SetupBalance(float balance) override;
        float Balance() const override;

        void SetupFrequencyRatio(float ratio) override;
        float FrequencyRatio() const override;

        bool IsPlaying() const override;
        bool IsPaused() const override;

        double SeekPosition() const override;
        double TotalTime() const override;

    protected:
        
        XA2SoundSource(IXAudio2* device, const WaveFormat& waveFormat);

        XA2SourceVoice  sourceVoice;

        float           volume          = 1.0f;
        float           balance         = 0.0f;
        float           frequencyRatio  = 1.0f;

        bool            isPaused        = false;

        double          totalTime       = 0.0;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================