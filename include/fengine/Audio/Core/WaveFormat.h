/*
 * Wave format header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_WAVE_FORMAT_H__
#define __FORK_WAVE_FORMAT_H__


#include "Core/Export.h"
#include "Audio/Core/WaveFormatTags.h"


namespace Fork
{

namespace Audio
{


#include "Core/PackPush.h"

//! Audio wave buffer format structure.
struct WaveFormat
{
    unsigned short  formatTag;      //!< Encoding format tag. \see WaveFormatTags
    unsigned short  channels;       //!< Number of channels. 1 for mono and 2 for stereo.
    unsigned long   sampleRate;     //!< Number samples per second (in Herz). The commonly used sample rates are: 8.0kHz, 11.025kHz, 22.05kHz and 44.1kHz.
    unsigned long   bytesPerSecond; //!< Number of bytes per second. Must be equal to (sampleRate * blockAlign).
    unsigned short  blockAlign;     //!< Channel block alignment. Must be equal to ((channels * bitsPerSample) / 8).
    unsigned short  bitsPerSample;  //!< Number of bits per sample.
}
PACK_STRUCT;

#include "Core/PackPop.h"


/**
Generates a PCM wave format.
\param[in] channels Specifies the number of channels.
This is commonly 1 (for mono) or 2 (for stereo) sounds.
\param[in] sampleRate Specifies the sample rate. Common values are 8000 (8 KHz) or 44100 (44.1 KHz).
\param[in] bitsPerSample Specifies the number of bits per sample. This is commonly 8 or 16.
\return The generated wave format. All members are initialized with
the respective values depending on the input parameters.
\see WaveFormat
\see WaveFormatTags::PCM
*/
FORK_EXPORT WaveFormat MakeWaveFormatPCM(
    unsigned short channels, unsigned long sampleRate, unsigned short bitsPerSample
);


} // /namespace Audio

} // /namespace Fork


#endif



// ========================