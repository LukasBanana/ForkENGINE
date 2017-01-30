/*
 * Sound file formats file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Audio/FileHandler/SoundFileFormats.h"
#include "IO/Core/MagicNumber.h"

#include <fstream>


namespace Fork
{

namespace Audio
{


/*
Magic numbers has been taken from:
http://en.wikipedia.org/wiki/List_of_file_signatures
*/
FORK_EXPORT SoundFileFormats DetectFileFormat(const std::string& filename)
{
    /* Open file for reading */
    std::ifstream file(filename);
    if (!file.is_open())
        return SoundFileFormats::__Unknown__;

    /* Read magic number */
    IO::MagicNumber magicNumber;
    file.read(magicNumber.charSet, 4);

    /* Compare magic number to detect sound file format */
    static const unsigned long  magicNumberWAV = 0x46464952; // 'RIFF'
    static const unsigned short magicNumberMP3 = 0xbfff;
    static const unsigned long  magicNumberOGG = 0x5367674f; // 'OggS'
    static const unsigned long  magicNumberASF = 0x75b22630;
    static const unsigned long  magicNumberMID = 0x4D546864; // 'MThd'

    if (magicNumber.number32 == magicNumberWAV)
        return SoundFileFormats::WAV;
    if (magicNumber.number32 == magicNumberOGG)
        return SoundFileFormats::OGG;
    if (magicNumber.number16 == magicNumberMP3)
        return SoundFileFormats::MP3;
    if (magicNumber.number32 == magicNumberASF)
        return SoundFileFormats::ASF;
    if (magicNumber.number32 == magicNumberMID)
        return SoundFileFormats::MID;

    return SoundFileFormats::__Unknown__;
}


} // /namespace Audio

} // /namespace Fork



// ========================