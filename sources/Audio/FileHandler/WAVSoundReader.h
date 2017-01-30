/*
 * WAV sound reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_WAV_SOUND_READER_H__
#define __FORK_WAV_SOUND_READER_H__


#include "Audio/FileHandler/SoundReader.h"

#include <fstream>


namespace Fork
{

namespace Audio
{


class WAVSoundReader : public SoundReader
{
    
    public:
        
        WaveBufferPtr ReadSound(const std::string& filename) override;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================