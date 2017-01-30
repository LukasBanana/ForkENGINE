/*
 * Sound file handler file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Audio/FileHandler/SoundFileHandler.h"
#include "Audio/FileHandler/SoundFileFormats.h"
#include "WAVSoundReader.h"
#include "OGGSoundStreamReader.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Audio
{

namespace SoundFileHandler
{


FORK_EXPORT WaveBufferPtr ReadSound(const std::string& filename)
{
    /* Choose sound reader by file format */
    auto fileFormat = Audio::DetectFileFormat(filename);

    try
    {
        std::unique_ptr<SoundReader> soundReader;

        switch (fileFormat)
        {
            case SoundFileFormats::__Unknown__:
                IO::Log::Error("File \"" + filename + "\" not found");
                break;
            case SoundFileFormats::WAV:
                soundReader = std::make_unique<WAVSoundReader>();
                break;
            default:
                IO::Log::Error("Unsupported sound file format");
                break;
        }

        /* Read sound data */
        return soundReader != nullptr ? soundReader->ReadSound(filename) : nullptr;
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
    }

    return nullptr;
}

FORK_EXPORT SoundStreamReaderPtr OpenSoundStream(const std::string& filename)
{
    /* Choose sound stream reader by file format */
    auto fileFormat = Audio::DetectFileFormat(filename);

    try
    {
        switch (fileFormat)
        {
            case SoundFileFormats::__Unknown__:
                IO::Log::Error("File \"" + filename + "\" not found");
                break;
            case SoundFileFormats::OGG:
                return std::make_shared<OGGSoundStreamReader>(filename);
            default:
                IO::Log::Error("Unsupported sound stream file format");
                break;
        }
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
    }

    return nullptr;
}


} // /namespace SoundFileHandler

} // /namespace Audio

} // /namespace Fork



// ========================