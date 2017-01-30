/*
 * GUI hardware draw context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "HWDrawContext.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace GUI
{


/*
 * Internal macros
 */

#define FONT_TO_HANDLE(obj) reinterpret_cast<Font>(obj)
#define HANDLE_TO_FONT(obj) reinterpret_cast<Video::TextureFont*>(obj)


/*
 * Internal functions
 */

static inline Math::Point2f ToF32(const Point& point)
{
    return point.Cast<float>();
}

static inline Video::ColorRGBAf ToF32(const Color& color)
{
    return color.Cast<float>();
}

static inline Math::Rectf ToF32(const Rect& rect)
{
    return rect.Cast<float>();
}

static inline Video::BezierCurve2f ToF32(const BezierCurve& curve)
{
    Video::BezierCurve2f curveF32;
    for (size_t i = 0; i < 4; ++i)
        curveF32.points[i] = curve.points[i].Cast<float>();
    return curveF32;
}


/*
 * HWDrawContext class
 */

static const size_t numPrimitiveVertices = 8;

HWDrawContext::HWDrawContext(
    const Video::PrimitiveRendererPtr& renderer, Video::RenderSystem* renderSystem, Video::RenderContext* renderContext) :
        renderer_           { renderer             },
        renderContext_      { renderContext        },
        primitiveCoords_    { numPrimitiveVertices },
        primitiveTexCoords_ { numPrimitiveVertices },
        primitiveColors_    { numPrimitiveVertices }
{
    /* Validate input arguments */
    ASSERT_POINTER( renderer      );
    ASSERT_POINTER( renderSystem  );
    ASSERT_POINTER( renderContext );

    /* Create default rasterizer state for this context */
    Video::RasterizerState::Description stateDesc;
    {
        stateDesc.cullMode              = Video::RasterizerState::CullModes::None;
        stateDesc.isScissorTestEnabled  = true;
    }
    defaultRasterState_ = renderSystem->CreateRasterizerState(stateDesc);

    /* Create default font */
    PushFont(LoadFont(L"Calibri", 20));
}

void HWDrawContext::Begin()
{
    renderer_->BeginDrawing2D();
    
    renderContext_->SetupViewport();
    renderContext_->ChangeState(defaultRasterState_.get());

    #ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_
    UpdateAdjustLineOrigin();
    #endif
}

void HWDrawContext::End()
{
    renderer_->EndDrawing2D();
}

void HWDrawContext::DrawPoint(const Point& point, const Color& color)
{
    renderer_->DrawPoint(ToF32(point), ToF32(color));
}

void HWDrawContext::DrawLine(const Point& startPoint, const Point& endPoint, const Color& color)
{
    renderer_->DrawLine({ ToF32(startPoint), ToF32(endPoint) }, ToF32(color));
}

void HWDrawContext::DrawLine(const Point& startPoint, const Point& endPoint, const Color& startColor, const Color& endColor)
{
    renderer_->DrawLine({ ToF32(startPoint), ToF32(endPoint) }, ToF32(startColor), ToF32(endColor));
}

void HWDrawContext::DrawRect(const Rect& rect, const Color& color)
{
    renderer_->DrawRect(ToF32(rect), ToF32(color));
}

void HWDrawContext::DrawRect(const Rect& rect, const Color& colorLT, const Color& colorRT, const Color& colorRB, const Color& colorLB)
{
    renderer_->DrawRect(ToF32(rect), ToF32(colorLT), ToF32(colorRT), ToF32(colorRB), ToF32(colorLB));
}

void HWDrawContext::DrawFrame(const Rect& rect, const Color& color)
{
    auto rectF32 = rect.Cast<float>();

    /* Setup vertex coordinates */
    #ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_
    if (adjustLineOriginLT_)
        rectF32.top += 1.0f;
    else
        rectF32.bottom -= 1.0f;
    #endif

    primitiveCoords_[0] = { rectF32.left , rectF32.top   , 0, 1 };
    primitiveCoords_[1] = { rectF32.right, rectF32.top   , 0, 1 };

    primitiveCoords_[2] = { rectF32.right, rectF32.bottom, 0, 1 };
    primitiveCoords_[3] = { rectF32.left, rectF32.bottom, 0, 1 };

    #ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_
    rectF32.left += 1.0f;
    #endif

    primitiveCoords_[4] = { rectF32.right, rectF32.top   , 0, 1 };
    primitiveCoords_[5] = { rectF32.right, rectF32.bottom, 0, 1 };

    primitiveCoords_[6] = { rectF32.left , rectF32.bottom, 0, 1 };
    primitiveCoords_[7] = { rectF32.left , rectF32.top   , 0, 1 };

    /* Draw frame */
    renderer_->Draw(Video::GeometryPrimitives::Lines, primitiveCoords_, ToF32(color));
}

void HWDrawContext::DrawFrame(const Rect& rect, const Color& colorLT, const Color& colorRT, const Color& colorRB, const Color& colorLB)
{
    auto rectF32 = rect.Cast<float>();

    /* Setup vertex coordinates */
    #ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_
    if (adjustLineOriginLT_)
        rectF32.top += 1.0f;
    else
        rectF32.bottom -= 1.0f;
    #endif

    primitiveCoords_[0] = { rectF32.left , rectF32.top   , 0, 1 };
    primitiveCoords_[1] = { rectF32.right, rectF32.top   , 0, 1 };

    primitiveCoords_[2] = { rectF32.right, rectF32.bottom, 0, 1 };
    primitiveCoords_[3] = { rectF32.left, rectF32.bottom, 0, 1 };

    #ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_
    rectF32.left += 1.0f;
    #endif

    primitiveCoords_[4] = { rectF32.right, rectF32.top   , 0, 1 };
    primitiveCoords_[5] = { rectF32.right, rectF32.bottom, 0, 1 };

    primitiveCoords_[6] = { rectF32.left , rectF32.bottom, 0, 1 };
    primitiveCoords_[7] = { rectF32.left , rectF32.top   , 0, 1 };

    /* Setup vertex colors */
    primitiveColors_[0] = ToF32(colorLT);
    primitiveColors_[1] = ToF32(colorRT);

    primitiveColors_[2] = ToF32(colorRB);
    primitiveColors_[3] = ToF32(colorLB);

    primitiveColors_[4] = ToF32(colorRT);
    primitiveColors_[5] = ToF32(colorRB);

    primitiveColors_[6] = ToF32(colorLB);
    primitiveColors_[7] = ToF32(colorLT);

    /* Draw frame */
    renderer_->Draw(Video::GeometryPrimitives::Lines, primitiveCoords_, primitiveTexCoords_, primitiveColors_);
}

void HWDrawContext::DrawBezier(const BezierCurve& curve, const Color& color)
{
    renderer_->DrawBezier(ToF32(curve), ToF32(color));
}

void HWDrawContext::DrawBezier(const BezierCurve& curve, const Color& startColor, const Color& endColor)
{
    renderer_->DrawBezier(ToF32(curve), ToF32(startColor), ToF32(endColor));
}

Font HWDrawContext::LoadFont(
    const String& fontName, int fontSize, bool isBold, bool isItalic, bool isUnderlined, bool isStrikeOut)
{
    /* Setup font description */
    typedef Video::Font::Flags Flags;

    Video::Font::Description fontDesc;

    fontDesc.name = fontName;
    fontDesc.size = { 0, fontSize };

    if (isBold)
        fontDesc.flags |= Flags::Bold;
    if (isItalic)
        fontDesc.flags |= Flags::Italic;
    if (isUnderlined)
        fontDesc.flags |= Flags::Underlined;
    if (isStrikeOut)
        fontDesc.flags |= Flags::StrikeOut;

    /* Create texture font */
    auto font = renderer_->CreateTextureFont(fontDesc);
    fonts_.push_back(font);

    return FONT_TO_HANDLE(font.get());
}

void HWDrawContext::PushFont(Font font)
{
    fontStack_.push(HANDLE_TO_FONT(font));
}

void HWDrawContext::PopFont()
{
    if (fontStack_.size() > 1)
        fontStack_.pop();
}

void HWDrawContext::DrawFontText(
    const Point& position, const String& text, const Color& color, bool isCenteredHorz, bool isCenteredVert)
{
    if (!fontStack_.empty())
    {
        typedef Video::PrimitiveRenderer::FontDrawFlags DrawFlags;

        DrawFlags::DataType flags = 0;

        if (isCenteredHorz)
            flags |= DrawFlags::CenterX;
        if (isCenteredVert)
            flags |= DrawFlags::CenterY;

        renderer_->DrawText2D(fontStack_.top(), ToF32(position), text, ToF32(color), flags);
    }
}


/*
 * ======= Private: =======
 */

void HWDrawContext::Scissor(const Rect& rect)
{
    renderContext_->SetupScissor({ rect.LTPoint(), rect.Size() });
}

void HWDrawContext::Scissor()
{
    renderContext_->SetupScissor({ {}, renderContext_->Resolution().Cast<int>() });
}

#ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_

void HWDrawContext::UpdateAdjustLineOrigin()
{
    /* Initialize line rasterization adjustment */
    adjustLineOriginLT_ = renderContext_->QueryRendererDetails().hasOriginLeftTop;
}

#endif


} // /namespace GUI

} // /namespace Fork



// ========================