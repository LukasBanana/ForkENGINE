/*
 * Sound file handler header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_FILE_HANDLER_H__
#define __FORK_SOUND_FILE_HANDLER_H__


#include "Core/Export.h"
#include "Audio/Core/WaveBuffer.h"
#include "Audio/FileHandler/SoundStreamReader.h"

#include <string>


namespace Fork
{

namespace Audio
{

namespace SoundFileHandler
{


/**
Reads the specified wave sound from file.
\see SoundReader::ReadSound
*/
FORK_EXPORT WaveBufferPtr ReadSound(const std::string& filename);

/**
Opens the specified sound stream.
\see SoundStreamReader::ReadStream
*/
FORK_EXPORT SoundStreamReaderPtr OpenSoundStream(const std::string& filename);


} // /namespace SoundFileHandler

} // /namespace Audio

} // /namespace Fork


#endif



// ========================