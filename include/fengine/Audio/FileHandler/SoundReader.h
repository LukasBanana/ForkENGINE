/*
 * Sound reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_READER_H__
#define __FORK_SOUND_READER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Audio/Core/WaveBuffer.h"

#include <string>


namespace Fork
{

namespace Audio
{


DECL_SHR_PTR(SoundReader);

//! Sound reader interface.
class FORK_EXPORT SoundReader
{
    
    public:
        
        virtual ~SoundReader()
        {
        }

        /**
        Reads the wave buffer from the specified sound file.
        \param[in] filename Specifies the filename of the sound which is to be read.
        */
        virtual WaveBufferPtr ReadSound(const std::string& filename) = 0;

    protected:

        SoundReader() = default;

};


} // /namespace Audio

} // /namespace Fork


#endif



// ========================