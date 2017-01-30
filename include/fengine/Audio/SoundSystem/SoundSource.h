/*
 * Sound source header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_SOURCE_H__
#define __FORK_SOUND_SOURCE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Math/Core/Vector3.h"


namespace Fork
{

namespace Audio
{


DECL_SHR_PTR(SoundSource);

//! Sound source interface.
class FORK_EXPORT SoundSource
{
    
    public:
        
        //! Sound source types.
        enum class Types
        {
            Buffer, //!< Buffer sound. \see BufferSoundSource
            Stream, //!< Stream sound. \see StreamSoundSource
        };

        virtual ~SoundSource()
        {
        }

        //! Returns the type of this sound source.
        virtual Types Type() const = 0;

        /**
        Starts the sound playback.
        \param[in] loopCount Specifies the number of loop iterations (i.e. number of repetitions). By default 0.
        */
        virtual void Play(unsigned int loopCount = 0) = 0;
        //! Pauses or resumes the sound playback.
        virtual void Pause(bool isPaused = true) = 0;
        //! Stops the sound playback.
        virtual void Stop() = 0;

        /**
        Sets the volume of the sound source.
        \param[in] volume Specifies sound volume. This must in the range [0.0 .. +inf). By default 1.0.
        \remarks Values over 1.0 are allowed as well. But a too high volume may result in lower sound quality.
        */
        virtual void SetupVolume(float volume) = 0;
        //! Returns the current sound volume.
        virtual float Volume() const = 0;

        /**
        Sets the balance of the sound source.
        \param[in] balance Specifies the sound balance between the channels.
        This must be in the range [-1.0 .. 1.0], where -1.0 is the left side
        and +1.0 is the right side. By default 0.0.
        \remarks This has no effect if the number of sound channels is 1.
        */
        virtual void SetupBalance(float balance) = 0;
        //! Returns the current sound balance.
        virtual float Balance() const = 0;

        /**
        Sets the frequency ratio of the sound source.
        \param[in] ratio Specifies the sound frequency ratio. This must be in the range (0.0 .. +inf).
        A value larger than 1.0 will cause the sound to play faster and
        a value smaller than 1.0 will cause the sound to play slower. By default 1.0.
        \remarks The actual ratio limits may depend on the active sound system.
        */
        virtual void SetupFrequencyRatio(float ratio) = 0;
        //! Returns the frequency ratio.
        virtual float FrequencyRatio() const = 0;

        /**
        Returns true if the sound is currently being played.
        \see Play
        \see Stop
        */
        virtual bool IsPlaying() const = 0;
        /**
        Returns true if the sound is currently paused.
        \see Pause
        */
        virtual bool IsPaused() const = 0;

        /**
        Returns the seek position since the sound source has been started to play.
        \return The current seek position in the range [0.0 .. 1.0].
        \see Play
        */
        virtual double SeekPosition() const = 0;
        //! Returns the total time (in seconds) of the sound source.
        virtual double TotalTime() const = 0;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================