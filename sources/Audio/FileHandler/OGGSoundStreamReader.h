/*
 * OGG sound stream reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_OGG_SOUND_STREAM_READER_H__
#define __FORK_OGG_SOUND_STREAM_READER_H__


#include "Audio/FileHandler/SoundStreamReader.h"

#include <fstream>
#include <cstdio>//!!!
#include <vorbis/vorbisfile.h>


namespace Fork
{

namespace Audio
{


#define _FORKAUDIO_USE_ANSI_C_FILES_ // !!!

class OGGSoundStreamReader : public SoundStreamReader
{
    
    public:
        
        //! \throws std::runtime_error
        OGGSoundStreamReader(const std::string& filename);
        ~OGGSoundStreamReader();

        size_t ReadStream(PCMBuffer& buffer) override;

        bool TimeSeek(double time) override;

    private:
        
        #ifdef _FORKAUDIO_USE_ANSI_C_FILES_
        FILE*           file_ = nullptr;
        #else
        std::ifstream   file_;
        #endif
        vorbis_info*    info_ = nullptr;
        OggVorbis_File  oggFile_;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================