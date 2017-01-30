/*
 * Renderer profiler output file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/RendererProfilerOutput.h"
#include "Math/Core/Vector2.h"
#include "Core/Exception/NullPointerException.h"

#include <string>


namespace Fork
{

namespace Utility
{

namespace RendererProfilerOutput
{


/* --- Internal members --- */

static const char* profilerInfoTexts[] =
{
    "Frame Rate:",
    "Draw Calls:",
    "Rendered Triangles:",
    "Render Target Bindings:",
    "Texture Bindings:",
    "Buffer Bindings:",
    "Shader Bindings:",
    "Texture Creations:",
    "Texture Updates:",
    "Buffer Creations:",
    "Buffer Updates:",
    "State Changes:",
    "Context Switches:",
    0
};

static float profilerInfoTextMaxWidth = -1.0f;


/* --- Global functions --- */

FORK_EXPORT void DrawInfo(
    Video::RendererProfilerModel profilerModel, Platform::Timer* timer,
    Video::PrimitiveRenderer* renderer, const Video::TextureFont* font,
    float textBorder, const Video::ColorRGBAf& color)
{
    ASSERT_POINTER(renderer);
    ASSERT_POINTER(font);

    if (profilerInfoTextMaxWidth <= 0.0f)
        InitInfoTextMaxWidth(font);

    Math::Point2f textPos { textBorder, textBorder };

    auto DrawProfilerText = [&](const Math::Point2f& position, const std::string& text)
    {
        renderer->DrawText2D(font, position, text, color);
    };

    auto AddProfilerInfo = [&](const std::string& text)
    {
        DrawProfilerText(textPos, text);
        textPos.y += font->GetDesc().size.height;
    };

    auto IncTextPosY = [&textPos]
    {
        textPos.y += 10.0f;
    };

    /* Draw profiler descriptions */
    if (timer)
    {
        AddProfilerInfo(profilerInfoTexts[0]); // <-- Frame rate
        IncTextPosY();
    }

    size_t i = 0;

    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Draw calls
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Rendered triangles
    IncTextPosY();
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Render target bindings
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Texture bindings
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Buffer bindings
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Shader bindings
    IncTextPosY();
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Texture creations
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Texture updates
    IncTextPosY();
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Buffer creations
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Buffer updates
    IncTextPosY();
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- State changes
    AddProfilerInfo(profilerInfoTexts[++i]); // <-- Context switches

    /* Draw profiler information */
    textPos = { textBorder*2 + profilerInfoTextMaxWidth, textBorder };

    if (timer)
    {
        AddProfilerInfo(ToStr(timer->GetFrameRate(), 2) + " Hz");
        IncTextPosY();
    }

    AddProfilerInfo(ToStr(profilerModel.NumDrawCalls            ()));
    AddProfilerInfo(ToStr(profilerModel.NumRenderedTriangles    ()));
    IncTextPosY();
    AddProfilerInfo(ToStr(profilerModel.NumRenderTargetBindings ()));
    AddProfilerInfo(ToStr(profilerModel.NumTextureBindings      ()));
    AddProfilerInfo(ToStr(profilerModel.NumBufferBindings       ()));
    AddProfilerInfo(ToStr(profilerModel.NumShaderBindings       ()));
    IncTextPosY();
    AddProfilerInfo(ToStr(profilerModel.NumTextureCreations     ()));
    AddProfilerInfo(ToStr(profilerModel.NumTextureUpdates       ()));
    IncTextPosY();
    AddProfilerInfo(ToStr(profilerModel.NumBufferCreations      ()));
    AddProfilerInfo(ToStr(profilerModel.NumBufferUpdates        ()));
    IncTextPosY();
    AddProfilerInfo(ToStr(profilerModel.NumStateChanges         ()));
    AddProfilerInfo(ToStr(profilerModel.NumContextSwitches      ()));
}

FORK_EXPORT void InitInfoTextMaxWidth(const Video::TextureFont* font)
{
    if (!font)
        return;

    const char** infoText = profilerInfoTexts;

    while (*infoText)
    {
        auto textWidth = static_cast<float>(font->TextWidth(*infoText));
        
        if (profilerInfoTextMaxWidth < textWidth)
            profilerInfoTextMaxWidth = textWidth;

        ++infoText;
    }
}


} // /namespace RendererProfilerOutput

} // /namespace Utility

} // /namespace Fork



// ========================