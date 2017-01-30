/*
 * Wave buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_WAVE_BUFFER_H__
#define __FORK_WAVE_BUFFER_H__


#include "Core/Export.h"
#include "Audio/Core/WaveFormat.h"
#include "Math/Core/Range.h"

#include <vector>
#include <queue>
#include <memory>


namespace Fork
{

namespace Audio
{


//! Raw audio PCM (Pulse Modulation Code) buffer type.
typedef std::vector<char> PCMBuffer;

/**
Type for a PCM buffer queue.
\see PCMBuffer
*/
typedef std::queue<PCMBuffer> PCMBufferQueue;

//! Value to play a sound in an infinite loop
static const unsigned int infiniteSoundLoop = unsigned(-1);

//! Data model structure for an audio wave buffer.
struct FORK_EXPORT WaveBuffer
{
    //! Reverses the raw buffer.
    void Reverse();
    //! Converts the raw buffer to mono sound (1 channel).
    void MakeMono();
    //! Converts the raw buffer to stereo sound (2 channels).
    void MakeStereo();

    //! Returns the number of samples (independently of the number of channels).
    size_t NumSamples() const;
    /**
    Samples the PCM buffer in the specified range.
    \param[in] sampleIndex Specifies the index of the first PCM sample.
    \param[in] numSamples Specifies the number of samples to take.
    \param[in] channel Specifies the channel which is to be sampled from.
    If this is 0, the range is computed by the average values of all channels.
    If this is 1, only the 1st channel is sampled,
    If this is 2, only the 2nd channel is sampled, and so forth ...
    \return Minimal and maximal values of the samples in the specified range.
    \remarks If an input parameter is invalid, the return value is an
    invalid range (i.e. 'min' is larger than the 'max' component).
    */
    Math::Rangei SampleRange(size_t sampleIndex, size_t numSamples, unsigned short channel = 0) const;

    //! Returns the total time (in seconds) which is required to play the entire wave buffer.
    double TotalTime() const;

    WaveFormat  format; //!< Wave buffer format.
    PCMBuffer   buffer; //!< Wave PCM (Pulse Modulation Code) buffer.
};

typedef std::shared_ptr<WaveBuffer> WaveBufferPtr;


} // /namespace Audio

} // /namespace Fork


#endif



// ========================