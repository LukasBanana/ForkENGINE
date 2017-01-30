/*
 * Waveform renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_WAVEFORM_RENDERER_H__
#define __FORK_UTILITY_WAVEFORM_RENDERER_H__


#include "Core/Export.h"
#include "Audio/Core/WaveBuffer.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"


namespace Fork
{

namespace Utility
{

namespace WaveformRenderer
{


/**
Renders the specified waveform.
\param[in] renderer Specifies the primitive renderer to draw the waveform.
\param[in] waveBuffer Specifies the wave buffer which is to be drawn as waveform.
\param[in] rect Specifies the rectangle where the waveform is to be drawn.
\param[in] primaryColor Specifies the primary color of the waveform.
\param[in] secondaryColor Specifies the secondary color of the waveform.
This is the inner color as additional visualization, similar to the free audio editor software "Audacity".
\param[in] sampleOffset Specifies the offset to sample the wave buffer. This will be clamped to the range [0.0 .. 1.0].
\param[in] sampleLength Specifies the length to sample the wave buffer. This will be clamped to the range [0.0 .. 1.0 - sampleOffset].
\throws NullPointerException If 'renderer' is null.
*/
FORK_EXPORT void DrawWaveform(
    Video::PrimitiveRenderer* renderer,
    const Audio::WaveBuffer& waveBuffer,
    const Math::Rectf& rect,
    const Video::ColorRGBAf& primaryColor,
    const Video::ColorRGBAf& secondaryColor,
    double sampleOffset,
    double sampleLength
);

/**
Same as primary "DrawWaveform" function but without a secondary color.
\see DrawWaveform(Video::PrimitiveRenderer*, const Audio::WaveBuffer&, const Math::Rectf&, const Video::ColorRGBAf&, const Video::ColorRGBAf&, double, double)
*/
FORK_EXPORT void DrawWaveform(
    Video::PrimitiveRenderer* renderer,
    const Audio::WaveBuffer& waveBuffer,
    const Math::Rectf& rect,
    const Video::ColorRGBAf& primaryColor,
    double sampleOffset,
    double sampleLength
);

/**
Same as primary "DrawWaveform" function but with 'sampleOffset = 0.0' and 'sampleLength = 1.0' as default values.
\see DrawWaveform(Video::PrimitiveRenderer*, const Audio::WaveBuffer&, const Math::Rectf&, const Video::ColorRGBAf&, const Video::ColorRGBAf&, double, double)
*/
FORK_EXPORT void DrawWaveform(
    Video::PrimitiveRenderer* renderer,
    const Audio::WaveBuffer& waveBuffer,
    const Math::Rectf& rect,
    const Video::ColorRGBAf& primaryColor
);


} // /namespace WaveformRenderer

} // /namespace Utility

} // /namespace Fork


#endif



// ========================