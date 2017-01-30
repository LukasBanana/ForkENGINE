/*
 * Sound file formats header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_FILE_FORMATS_H__
#define __FORK_SOUND_FILE_FORMATS_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Audio
{


//! Sound file formats.
enum class SoundFileFormats
{
    __Unknown__,    //!< Unknown file format.
    WAV,            //!< Windows RIFF (Resource Information File Format) WAVE sounds. Uncompressed PCM sound data. File extensions are: .wav.
    OGG,            //!< Ogg Vorbis sound streams. Compressed audio stream. File extensions are: .ogg, .oga, .ogv.
    MP3,            //!< MPEG Layer-3 sound streams. Compressed audio stream. File extensions are: .mp3. This format is not supported!
    ASF,            //!< Advanced Systems Format sounds. Compressed audio stream. File extensions are: .asf, .wma, .wmv. This format is not supported!
    MID,            //!< Musical Instrument Digital Interface sounds. Compressed sound data. File extensions are: .mid.
};


/**
Returns the format type of the specified sound file.
\see SoundFileFormats
*/
FORK_EXPORT SoundFileFormats DetectFileFormat(const std::string& filename);


} // /namespace Audio

} // /namespace Fork


#endif



// ========================