/*
 * Sound system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_SYSTEM_H__
#define __FORK_SOUND_SYSTEM_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Platform/Core/DynamicLibrary.h"
#include "Audio/Core/WaveBuffer.h"
#include "Audio/SoundSystem/BufferSoundSource.h"
#include "Audio/SoundSystem/StreamSoundSource.h"
#include "Audio/FileHandler/SoundStreamReader.h"

#include <map>
#include <vector>
#include <memory>


namespace Fork
{

namespace Audio
{


//! Simple structure to store a stream sound source and the respective stream reader.
struct StreamSound
{
    StreamSoundSourcePtr    stream;         //!< \see StreamSoundSource
    SoundStreamReaderPtr    reader;         //!< \see SoundStreamReader
    unsigned int            loopCount = 0;  //!< Loop repetition counter. By default 0.
};

DECL_SHR_PTR(SoundSystem);

//! Sound system interface.
class FORK_EXPORT SoundSystem
{
    
    public:
        
        /* === Static functions === */

        /**
        Creates a sound system from the specified library.
        \param[in] library Specifies the library from which the sound system is to be created (e.g. "ForkAudioAL" or "ForkAudioXA2").
        \throws NullPointerException If 'library' is null.
        \throws SounodSystemException If an error occured during creation.
        \see Platform::DynamicLibrary
        */
        static SoundSystemPtr Create(const Platform::DynamicLibraryPtr& library);

        /* === Interface === */

        virtual ~SoundSystem();

        /**
        Loads a buffer sound source from the specified file.
        \see BufferSoundSource
        */
        virtual BufferSoundSourcePtr LoadBufferSound(const std::string& filename) = 0;
        /**
        Loads (or rather opens) a stream sound source from the specified file.
        \see StreamSound
        */
        virtual StreamSound LoadStreamSound(const std::string& filename) = 0;

        /* === Functions === */

        /**
        Plays a sound directly from the specified file.
        \param[in] filename Specifies the sound filename. This can be either a buffer (WAV) or stream (OGG) sound file.
        \param[in] loopCount Specifies the number of loop iterations (repetitions of the sound).
        To play the sound in an infinite loop, set it to 'infiniteSoundLoop'.
        0 means the sound is played only once (i.e. with 0 repetitions). By default 0.
        \param[in] volume Specifies the sound volume to play. Must be in the range [0.0 .. inf). By default 1.0.
        \param[in] balance Specifies the sound balance to paly. Must be in the range [-1.0 .. 1.0]
        where -1.0 is left and +1.0 is right. By default 0.0.
        \param[in] frequencyRatio Specifies the sound frequency ratio to play. Must be in the range (0.0 .. inf). By default 1.0.
        \return True if the sound has been loaded successful. Otherwise the sound could not be loaded.
        \remarks This is a helper function to easily play sound files directly, without holding an instance of a sound source.
        The disadvantage of this high level function is, that you can not control the sound after playing started.
        You can only stop the sound by calling "StopAll", which stops all sounds currently being played.
        \note The XAudio2 sound system only allows a maximum loop count of 254 or an infinite loop.
        \see StopAll
        \see infiniteSoundLoop
        */
        bool PlayFromFile(
            const std::string& filename, unsigned int loopCount = 0,
            float volume = 1.0f, float balance = 0.0f, float frequencyRatio = 1.0f
        );

        /**
        Stops all sounds which are currently being played.
        \remarks This is the only way to stop sounds which are played with the "PlayFromFile" function.
        \see PlayFromFile
        */
        void StopAll();

        /**
        Updates the streaming process for the specified stream sound.
        \param[in,out] sound Specifies the sound (with its stream and reader) which is to be updated.
        \param[in] queueLength Specifies the buffer queue length. This should be at least larger than 1.
        Otherwise the sound may 'stutter', because the streaming does not provide enough
        audio buffers in a certain time interval. By default 10.
        \remarks If the stream has reached its end and the "loopCount" member of the 'sound' parameter is greater than 0,
        the stream starts from the beginning and the "loopCount" member will be decremented by 1.
        \note This function must not be used in a multi-threaded manner.
        */
        void UpdateStreamSound(StreamSound& sound, unsigned int queueLength = 10);

        /**
        Updates the sound system. This should be called to updated all temporary sound buffers,
        which have been created by the "PlayFromFile" function.
        \see PlayFromFile
        */
        void Update();

    protected:
        
        /**
        Returns a wave buffer, loaded from the specified file.
        \return Pointer to the wave buffer or null if reading the specified file failed.
        */
        WaveBufferPtr LoadWaveBuffer(const std::string& filename);

    private:

        std::map<std::string, WaveBufferPtr> waveBufferMap_;

        /**
        Buffer sound source list for the temporary sound playback.
        \see PlayFromFile
        */
        std::vector<BufferSoundSourcePtr> bufferSoundSources_;
        /**
        Stream sound list for the temporary sound playback.
        \see PlayFromFile
        */
        std::vector<StreamSound> streamSounds_;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================