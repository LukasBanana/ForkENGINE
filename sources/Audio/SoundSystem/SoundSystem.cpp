/*
 * Sound system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Audio/SoundSystem/SoundSystem.h"
#include "Audio/SoundSystem/SoundSystemException.h"
#include "Audio/FileHandler/SoundFileHandler.h"
#include "Audio/FileHandler/SoundFileFormats.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/STLHelper.h"
#include "Platform/Core/OS.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Audio
{


SoundSystem::~SoundSystem()
{
}

SoundSystemPtr SoundSystem::Create(const Platform::DynamicLibraryPtr& library)
{
    ASSERT_POINTER(library);

    const std::string procName = "AllocSoundSystem";

    /* Load render system allocation procedure from library */
    DEFINE_PROC_INTERFACE(void*, PFNALLOCSOUNDSYSTEMPROC, (void));

    auto AllocSoundSystem = reinterpret_cast<PFNALLOCSOUNDSYSTEMPROC>(library->LoadProc(procName));
    if (!AllocSoundSystem)
    {
        throw SoundSystemException(
            "Procedure \"" + procName + "\" not found in sound system library \"" + ToStr(library->GetName()) + "\""
        );
    }

    /* Allocate render system via library function */
    auto soundSystem = reinterpret_cast<SoundSystem*>(AllocSoundSystem());
    if (!soundSystem)
        throw SoundSystemException("Allocating sound system from library \"" + ToStr(library->GetName()) + "\" failed"); 

    return std::shared_ptr<SoundSystem>(soundSystem);
}

static void SetupSoundSource(SoundSource* sound, float volume, float balance, float frequencyRatio)
{
    sound->SetupVolume(volume);
    sound->SetupBalance(balance);
    sound->SetupFrequencyRatio(frequencyRatio);
}

bool SoundSystem::PlayFromFile(
    const std::string& filename, unsigned int loopCount, float volume, float balance, float frequencyRatio)
{
    /* Detect file format to determine whether a buffer- or stream sound must be loaded */
    auto fileFormat = DetectFileFormat(filename);

    switch (fileFormat)
    {
        case SoundFileFormats::WAV:
        {
            /* Load buffer sound from file */
            auto sound = LoadBufferSound(filename);
            if (!sound)
                return false;

            /* Setup sound configuration and play sound */
            SetupSoundSource(sound.get(), volume, balance, frequencyRatio);
            sound->Play(loopCount);

            bufferSoundSources_.push_back(sound);
        }
        return true;

        case SoundFileFormats::OGG:
        {
            /* Load stream sound from file */
            auto sound = LoadStreamSound(filename);
            if (!sound.reader || !sound.stream)
                return false;

            /* Setup sound configuration and play sound */
            SetupSoundSource(sound.stream.get(), volume, balance, frequencyRatio);
            sound.loopCount = loopCount;
            sound.stream->Play();

            streamSounds_.push_back(sound);
        }
        return true;
    }

    return false;
}

void SoundSystem::StopAll()
{
    /* Stop all buffer sounds */
    for (auto& sound : bufferSoundSources_)
        sound->Stop();
    bufferSoundSources_.clear();

    /* Stop all stream sounds */
    for (auto& sound : streamSounds_)
        sound.stream->Stop();
    streamSounds_.clear();
}

void SoundSystem::UpdateStreamSound(StreamSound& sound, unsigned int queueLength)
{
    static PCMBuffer pcmBuffer(1024);

    if (sound.stream && sound.reader)
    {
        while (sound.stream->QueueSize() < queueLength)
        {
            /* Read next stream chunk */
            auto chunkSize = sound.reader->ReadStream(pcmBuffer);

            if (chunkSize > 0)
            {
                /* Submit stream chunk buffer */
                sound.stream->SubmitBuffer(pcmBuffer, chunkSize);
            }
            else if (sound.loopCount > 0)
            {
                /* Start stream from the beginning */
                sound.reader->TimeSeek(0.0);
                --sound.loopCount;
            }
            else
            {
                /* Stop with stream playback */
                break;
            }
        }
    }
}

void SoundSystem::Update()
{
    /* Remove all buffer sounds from the list which are no longer being played */
    if (!bufferSoundSources_.empty())
    {
        RemoveAllFromListIf(
            bufferSoundSources_,
            [](BufferSoundSourcePtr& sound)
            {
                return !sound->IsPlaying();
            }
        );
    }

    if (!streamSounds_.empty())
    {
        /* Update streaming */
        for (auto& sound : streamSounds_)
            UpdateStreamSound(sound);

        /* Remove all stream sounds from the list which are no longer beaing played */
        RemoveAllFromListIf(
            streamSounds_,
            [](StreamSound& sound)
            {
                return !sound.stream->IsPlaying();
            }
        );
    }
}


/*
 * ======= Protected: =======
 */

WaveBufferPtr SoundSystem::LoadWaveBuffer(const std::string& filename)
{
    /* Try to find file in wave-buffer map */
    auto it = waveBufferMap_.find(filename);
    if (it != waveBufferMap_.end())
        return it->second;

    /* Load wave-buffer from file */
    IO::Log::Message("Load sound: \"" + filename + "\"");
    IO::Log::ScopedIndent unused;

    auto waveBuffer = SoundFileHandler::ReadSound(filename);
    if (!waveBuffer)
        return nullptr;
    
    /* Ensure to have a stereo wave buffer */
    waveBuffer->MakeStereo();

    /* Store wave-buffer in map */
    waveBufferMap_[filename] = waveBuffer;
    return waveBuffer;
}


} // /namespace Audio

} // /namespace Fork



// ========================