/*
 * Buffer sound source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Audio/SoundSystem/BufferSoundSource.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Audio
{


BufferSoundSource::BufferSoundSource(const WaveBufferPtr& waveBuffer) :
    waveBuffer_{ waveBuffer }
{
    ASSERT_POINTER(waveBuffer);
}

SoundSource::Types BufferSoundSource::Type() const
{
    return Types::Buffer;
}


} // /namespace Audio

} // /namespace Fork



// ========================