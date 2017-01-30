/*
 * Renderer profiler output header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_RENDERER_PROFILER_OUTPUT_H__
#define __FORK_UTILITY_RENDERER_PROFILER_OUTPUT_H__


#include "Core/Export.h"
#include "Video/RenderSystem/RendererProfilerModel.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/Font/TextureFont.h"
#include "Platform/Core/Timer.h"


namespace Fork
{

namespace Utility
{

namespace RendererProfilerOutput
{


/**
Draws the specified profiler model information.
\param[in] profilerModel Specifies the renderer profiler model data which is to be drawn.
\param[in] timer Optional raw-pointer to the timer. If a timer is passed as argument,
the frame rate will drawn, too.
\param[in] renderer Specifies the primitive renderer which is to be used to draw the text.
\param[in] font Specifies the font which is to be used to draw the text.
\param[in] textBorder Specifies the border between the text lines. By default 15.0.
\param[in] color Specifies the color which is to be used to draw the text. By default (1, 1, 1, 1).
\throws NullPointerException If 'renderer' is null or 'font' is null.
\see Video::RendererProfilerModel
*/
FORK_EXPORT void DrawInfo(
    Video::RendererProfilerModel profilerModel, Platform::Timer* timer,
    Video::PrimitiveRenderer* renderer, const Video::TextureFont* font,
    float textBorder = 15.0f, const Video::ColorRGBAf& color = {}
);

/**
Initializes the maximal width of the information text lines.
This should be called once at program start up, but it is not required.
If you omit this function call, it will be called automatically every
time you call "RendererProfilerOutput::DrawInfo".
*/
FORK_EXPORT void InitInfoTextMaxWidth(const Video::TextureFont* font);


} // /namespace RendererProfilerOutput

} // /namespace Utility

} // /namespace Fork


#endif



// ========================