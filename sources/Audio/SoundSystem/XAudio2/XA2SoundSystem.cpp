/*
 * XAudio2 sound system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "XA2SoundSystem.h"
#include "XA2SourceVoice.h"
#include "XA2Core.h"
#include "XA2BufferSoundSource.h"
#include "XA2StreamSoundSource.h"

#include "Audio/SoundSystem/SoundSystemException.h"
#include "Audio/FileHandler/SoundFileHandler.h"
#include "Platform/Core/OS.h"
#include "IO/Core/Log.h"
#include "Core/STLHelper.h"

#include <algorithm>


namespace Fork
{

namespace Audio
{


XA2SoundSystem::XA2SoundSystem()
{
    try
    {
        #ifndef FORK_XBOX
        /* Initialize COM (Component Object Model) system */
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        #endif

        /* Create sound device */
        if (FAILED(XAudio2Create(&device_)))
            throw std::runtime_error("Creating XAudio2 device failed");

        /* Create mastering voice */
        if (FAILED(device_->CreateMasteringVoice(&masteringVoice_)))
            throw std::runtime_error("Creating XAudio2 mastering voice failed");
    }
    catch (const std::exception& err)
    {
        throw SoundSystemException(err.what());
    }
}
XA2SoundSystem::~XA2SoundSystem()
{
    StopAll();
    masteringVoice_->DestroyVoice();
    
    DXSafeRelease(device_);

    #ifndef FORK_XBOX
    CoUninitialize();
    #endif
}

BufferSoundSourcePtr XA2SoundSystem::LoadBufferSound(const std::string& filename)
{
    /* Load wave buffer */
    auto waveBuffer = LoadWaveBuffer(filename);
    if (waveBuffer)
    {
        try
        {
            /* Create buffer sound source */
            return std::make_shared<XA2BufferSoundSource>(device_, waveBuffer);
        }
        catch (const std::exception& err)
        {
            IO::Log::Error(err.what());
        }
    }
    return nullptr;
}

StreamSound XA2SoundSystem::LoadStreamSound(const std::string& filename)
{
    StreamSound sound;

    /* Open sound stream */
    sound.reader = SoundFileHandler::OpenSoundStream(filename);

    if (sound.reader)
    {
        /* Create stream sound source */
        sound.stream = std::make_shared<XA2StreamSoundSource>(
            device_, sound.reader->GetWaveFormat(), sound.reader->GetTotalTime()
        );
    }

    return sound;
}


} // /namespace Audio

} // /namespace Fork



// ========================