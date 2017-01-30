/*
 * Wave buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Audio/Core/WaveBuffer.h"

#include <algorithm>


namespace Fork
{

namespace Audio
{


/*
 * Internal functions
 */

static void ChangeFormatChannels(WaveFormat& format, unsigned short channels)
{
    format.channels         = channels;
    format.bytesPerSecond   = format.sampleRate * format.channels * format.bitsPerSample / 8;
    format.blockAlign       = format.channels * format.bitsPerSample / 8;
}

/* --- Reverse --- */

template <typename T> void ReverseTmpl(const WaveFormat& format, PCMBuffer& buffer)
{
    auto size       = buffer.size();
    auto num        = size / sizeof(T);
    auto numHalf    = num / format.channels / 2;
    auto rawBuffer  = reinterpret_cast<T*>(buffer.data());

    for (size_t i = 0, j = 0; i < numHalf; ++i, j += format.channels)
    {
        for (unsigned short chn = 0; chn < format.channels; ++chn)
            std::swap(rawBuffer[chn + j], rawBuffer[num + chn - format.channels - j]);
    }
}

void WaveBuffer::Reverse()
{
    if (!buffer.empty())
    {
        switch (format.bitsPerSample)
        {
            case 8:
                ReverseTmpl<char>(format, buffer);
                break;
            case 16:
                ReverseTmpl<short>(format, buffer);
                break;
        }
    }
}

/* --- Mono --- */

template <typename T> void MakeMonoTmpl(WaveFormat& format, PCMBuffer& buffer)
{
    auto size = buffer.size()/2;
    auto num = size / sizeof(T);

    PCMBuffer tempBuffer(size);

    auto rawBufferSrc = reinterpret_cast<const T*>(buffer.data());
    auto rawBufferDst = reinterpret_cast<T*>(tempBuffer.data());

    long long sample;

    for (size_t i = 0; i < num; ++i)
    {
        /* Set mono sample to average of the two stereo samples */
        sample = rawBufferSrc[i*2];
        sample += rawBufferSrc[i*2 + 1];
        sample /= 2;

        rawBufferDst[i] = static_cast<T>(sample);
    }
    
    buffer = tempBuffer;
    ChangeFormatChannels(format, 1);
}

void WaveBuffer::MakeMono()
{
    /* Only allow conversion from stereo sound */
    if (format.channels == 2 && buffer.empty())
    {
        switch (format.bitsPerSample)
        {
            case 8:
                MakeMonoTmpl<char>(format, buffer);
                break;
            case 16:
                MakeMonoTmpl<short>(format, buffer);
                break;
        }
    }
}

/* --- Stereo --- */

template <typename T> void MakeStereoTmpl(WaveFormat& format, PCMBuffer& buffer)
{
    auto size = buffer.size();
    auto num = size / sizeof(T);

    PCMBuffer tempBuffer(size*2);

    auto rawBufferSrc = reinterpret_cast<const T*>(buffer.data());
    auto rawBufferDst = reinterpret_cast<T*>(tempBuffer.data());

    for (size_t i = 0; i < num; ++i)
    {
        /* Copy mono sample to the two stereo samples */
        rawBufferDst[i*2    ] = rawBufferSrc[i];
        rawBufferDst[i*2 + 1] = rawBufferSrc[i];
    }
    
    buffer = tempBuffer;
    ChangeFormatChannels(format, 2);
}

void WaveBuffer::MakeStereo()
{
    /* Only allow conversion from mono sound */
    if (format.channels == 1 && !buffer.empty())
    {
        switch (format.bitsPerSample)
        {
            case 8:
                MakeStereoTmpl<char>(format, buffer);
                break;
            case 16:
                MakeStereoTmpl<short>(format, buffer);
                break;
        }
    }
}

/* --- Samples --- */

size_t WaveBuffer::NumSamples() const
{
    return format.blockAlign > 0 ? buffer.size() / static_cast<size_t>(format.blockAlign) : 0;
}

template <typename T> Math::Rangei SampleRangeTmpl(
    const WaveFormat& format, const PCMBuffer& buffer,
    size_t sampleIndex, size_t numSamples, unsigned short channel)
{
    auto num = buffer.size() / sizeof(T);
    auto rawBuffer = reinterpret_cast<const T*>(buffer.data());

    Math::Rangei range;

    /* Get start offset for the specified sample index */
    size_t offset = sampleIndex*format.channels;

    if (channel > 0)
    {
        /* Get start offset for the specified sample index */
        --channel;
        offset += channel;

        for (size_t i = 0; i < numSamples; ++i)
        {
            /* Insert current sample into range */
            range.Insert(static_cast<int>(rawBuffer[offset]));

            /* Go to next sample */
            offset += format.channels;
        }
    }
    else
    {
        for (size_t i = 0; i < numSamples; ++i)
        {
            /* Compute average of current samples */
            long long int avg = 0;

            for (unsigned short c = 0; c < format.channels; ++c)
                avg += rawBuffer[offset++];

            avg /= format.channels;

            /* Insert average value into range */
            range.Insert(static_cast<int>(avg));
        }
    }

    return range;
}

Math::Rangei WaveBuffer::SampleRange(size_t sampleIndex, size_t numSamples, unsigned short channel) const
{
    if (channel <= format.channels && !buffer.empty() && sampleIndex + numSamples < NumSamples())
    {
        switch (format.bitsPerSample)
        {
            case 8:
                return SampleRangeTmpl<char>(format, buffer, sampleIndex, numSamples, channel);
            case 16:
                return SampleRangeTmpl<short>(format, buffer, sampleIndex, numSamples, channel);
        }
    }
    return {};
}

/* --- Misc --- */

double WaveBuffer::TotalTime() const
{
    if (format.bytesPerSecond > 0)
    {
        auto numBytes = static_cast<double>(buffer.size());
        return numBytes / static_cast<double>(format.bytesPerSecond);
    }
    return 0.0;
}


} // /namespace Audio

} // /namespace Fork



// ========================