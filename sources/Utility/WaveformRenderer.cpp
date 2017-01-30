/*
 * Waveform renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/WaveformRenderer.h"
#include "Core/Exception/NullPointerException.h"

#include <limits>


namespace Fork
{

namespace Utility
{

namespace WaveformRenderer
{


/*
 * Internal functions
 */

// Returns the maximal value for the specified bits-per-sample
static int MaxValueForBPS(unsigned short bps)
{
    return bps == 16 ?
        static_cast<int>(std::numeric_limits<short>::max()) :
        static_cast<int>(std::numeric_limits<char>::max());
}

/*
This is the actual "DrawWaveform" function implementation.
The global functions are used as interface.
*/
static void DrawWaveformImpl(
    Video::PrimitiveRenderer* renderer,
    const Audio::WaveBuffer& waveBuffer,
    const Math::Rectf& rect,
    const Video::ColorRGBAf& primaryColor,
    const Video::ColorRGBAf* secondaryColor,
    double sampleOffset,
    double sampleLength)
{
    /* Clamp input parameters */
    sampleOffset = Math::Clamp(sampleOffset, 0.0, 1.0);
    sampleLength = Math::Clamp(sampleLength, 0.0, 1.0 - sampleOffset);

    /* Initialize sampling parameters */
    const auto maxValue = MaxValueForBPS(waveBuffer.format.bitsPerSample);
    const auto scale    = (rect.Height() * 0.5f) / maxValue;
    
    const auto posX     = rect.left + 1.0f;
    const auto posY     = (rect.top + rect.bottom) * 0.5f;

    auto waveformWidth      = static_cast<int>(rect.Width());
    auto samples            = static_cast<double>(waveBuffer.NumSamples());
    auto samplesPerPixel    = (samples / waveformWidth) * sampleLength;

    /* Line plotting functions */
    auto PlotSampleLine = [&](int pos, const Math::Rangei& range)
    {
        /* Draw primary line */
        float min = scale * range.min;
        float max = scale * range.max;

        auto a = Math::Point2f(posX + pos, posY + std::floor(min       ));
        auto b = Math::Point2f(posX + pos, posY + std::floor(max + 1.0f));

        renderer->DrawLine(Math::Line2f{ a, b }, primaryColor);

        if (secondaryColor)
        {
            /* Draw secondary line */
            auto centerPosY = (a.y + b.y)/2.0f;
            auto height = b.y - a.y;

            auto c = Math::Point2f(a.x, centerPosY - height*0.25f);
            auto d = Math::Point2f(b.x, centerPosY + height*0.25f);

            renderer->DrawLine(Math::Line2f{ c, d }, *secondaryColor);
        }
    };

    auto PlotLineMinToMax = [&](int prevPos, int pos, const Math::Rangei& prevRange, const Math::Rangei& range)
    {
        float min = scale * prevRange.min;
        float max = scale * range.max;
        
        auto a = Math::Point2f(posX + prevPos, posY + std::floor(min       ));
        auto b = Math::Point2f(posX + pos    , posY + std::floor(max + 1.0f));

        renderer->DrawLine(Math::Line2f{ a, b }, primaryColor);
    };

    auto PlotLineMaxToMin = [&](int prevPos, int pos, const Math::Rangei& prevRange, const Math::Rangei& range)
    {
        float max = scale * prevRange.max;
        float min = scale * range.min;
        
        auto a = Math::Point2f(posX + prevPos, posY + std::floor(max + 1.0f));
        auto b = Math::Point2f(posX + pos    , posY + std::floor(min       ));

        renderer->DrawLine(Math::Line2f{ a, b }, primaryColor);
    };

    /* Setup lines from samples */
    sampleOffset *= samples;

    Math::Rangei prevRange;
    int prevPos = -1;

    for (int i = 0; i < waveformWidth; ++i)
    {
        /* Sample range from wave buffer */
        auto sampleIndex = static_cast<size_t>(sampleOffset);
        auto numSamples = static_cast<size_t>(sampleOffset + samplesPerPixel) - sampleIndex;

        if (numSamples > 0)
        {
            auto range = waveBuffer.SampleRange(sampleIndex, numSamples);

            if (!range.IsValid())
                break;
        
            /* Check if there is a gap between the previous range and this range */
            if (prevPos >= 0)
            {
                if (prevRange.max < range.min)
                    PlotLineMaxToMin(prevPos, i, prevRange, range);
                else if (prevRange.min > range.max)
                    PlotLineMinToMax(prevPos, i, prevRange, range);
            }

            /* Plot current sample line */
            PlotSampleLine(i, range);

            /* Store previous range and go to next sample offset */
            prevRange   = range;
            prevPos     = i;
        }

        sampleOffset += samplesPerPixel;
    }
}


/*
 * Global functions
 */

FORK_EXPORT void DrawWaveform(
    Video::PrimitiveRenderer* renderer, const Audio::WaveBuffer& waveBuffer, const Math::Rectf& rect,
    const Video::ColorRGBAf& primaryColor, const Video::ColorRGBAf& secondaryColor,
    double sampleOffset, double sampleLength)
{
    /* Validate input parameters */
    ASSERT_POINTER(renderer);
    DrawWaveformImpl(
        renderer, waveBuffer, rect, primaryColor,
        &secondaryColor, sampleOffset, sampleLength
    );
}

FORK_EXPORT void DrawWaveform(
    Video::PrimitiveRenderer* renderer,
    const Audio::WaveBuffer& waveBuffer,
    const Math::Rectf& rect,
    const Video::ColorRGBAf& primaryColor,
    double sampleOffset,
    double sampleLength)
{
    /* Validate input parameters */
    ASSERT_POINTER(renderer);
    DrawWaveformImpl(
        renderer, waveBuffer, rect, primaryColor,
        nullptr, sampleOffset, sampleLength
    );
}

FORK_EXPORT void DrawWaveform(
    Video::PrimitiveRenderer* renderer,
    const Audio::WaveBuffer& waveBuffer,
    const Math::Rectf& rect,
    const Video::ColorRGBAf& primaryColor)
{
    /* Validate input parameters */
    ASSERT_POINTER(renderer);
    DrawWaveformImpl(
        renderer, waveBuffer, rect, primaryColor,
        nullptr, 0.0, 1.0
    );
}


} // /namespace WaveformRenderer

} // /namespace Utility

} // /namespace Fork



// ========================