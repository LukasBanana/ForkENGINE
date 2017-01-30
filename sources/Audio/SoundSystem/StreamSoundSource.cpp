/*
 * Stream sound source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Audio/SoundSystem/StreamSoundSource.h"


namespace Fork
{

namespace Audio
{


StreamSoundSource::StreamSoundSource(const WaveFormat& waveFormat) :
    waveFormat_{ waveFormat }
{
}

SoundSource::Types StreamSoundSource::Type() const
{
    return Types::Stream;
}


} // /namespace Audio

} // /namespace Fork



// ========================