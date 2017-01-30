/*
 * Wave format file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Audio/Core/WaveFormat.h"
#include "Audio/Core/WaveFormatTags.h"


namespace Fork
{

namespace Audio
{


FORK_EXPORT WaveFormat MakeWaveFormatPCM(
    unsigned short channels, unsigned long sampleRate, unsigned short bitsPerSample)
{
    WaveFormat waveFormat;

    waveFormat.formatTag        = WaveFormatTags::PCM;
    waveFormat.channels         = channels;
    waveFormat.sampleRate       = sampleRate;
    waveFormat.bitsPerSample    = bitsPerSample;
    waveFormat.blockAlign       = (waveFormat.channels * waveFormat.bitsPerSample) / 8;
    waveFormat.bytesPerSecond   = waveFormat.sampleRate * waveFormat.blockAlign;

    return waveFormat;
}


} // /namespace Audio

} // /namespace Fork



// ========================