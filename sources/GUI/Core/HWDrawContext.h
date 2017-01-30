/*
 * GUI hardware draw context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_HW_DRAW_CONTEXT_H__
#define __FORK_GUI_HW_DRAW_CONTEXT_H__


#include "GUI/Core/DrawContext.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/RenderSystem/RenderState/RasterizerState.h"


namespace Fork
{

// Forward declaration
namespace Video
{

class RenderSystem;
class RenderContext;

}

namespace GUI
{


/*
Enable this for rasterization adjustment.
This may be required for line rasterization.
*/
#define _GUI_ENABLE_RASTERIZER_ADJUSTMENT_


//! Draw context implementation with hardware rendering support.
class HWDrawContext : public DrawContext
{
    
    public:
        
        //! \throws NullPointerException If any pointer is null.
        HWDrawContext(
            const Video::PrimitiveRendererPtr& renderer,
            Video::RenderSystem* renderSystem,
            Video::RenderContext* renderContext
        );

        void Begin() override;
        void End() override;

        void DrawPoint(const Point& point, const Color& color) override;

        void DrawLine(const Point& startPoint, const Point& endPoint, const Color& color) override;
        void DrawLine(const Point& startPoint, const Point& endPoint, const Color& startColor, const Color& endColor) override;

        void DrawRect(const Rect& rect, const Color& color) override;
        void DrawRect(const Rect& rect, const Color& colorLT, const Color& colorRT, const Color& colorRB, const Color& colorLB) override;

        void DrawFrame(const Rect& rect, const Color& color) override;
        void DrawFrame(const Rect& rect, const Color& colorLT, const Color& colorRT, const Color& colorRB, const Color& colorLB) override;

        void DrawBezier(const BezierCurve& curve, const Color& color) override;
        void DrawBezier(const BezierCurve& curve, const Color& startColor, const Color& endColor) override;

        Font LoadFont(
            const String& fontName, int fontSize,
            bool isBold = false, bool isItalic = false, bool isUnderlined = false, bool isStrikeOut = false
        ) override;

        void PushFont(Font font) override;
        void PopFont() override;

        virtual void DrawFontText(
            const Point& position, const String& text, const Color& color,
            bool isCenteredHorz = false, bool isCenteredVert = false
        ) override;

    private:
        
        void Scissor(const Rect& rect) override;
        void Scissor() override;

        #ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_
        void UpdateAdjustLineOrigin();
        #endif

        Video::PrimitiveRendererPtr         renderer_;
        Video::RenderContext*               renderContext_ = nullptr;

        Video::RasterizerStatePtr           defaultRasterState_;

        std::vector<Math::Point4f>          primitiveCoords_;
        std::vector<Math::Point4f>          primitiveTexCoords_;
        std::vector<Video::ColorRGBAf>      primitiveColors_;

        std::vector<Video::TextureFontPtr>  fonts_;
        std::stack<Video::TextureFont*>     fontStack_;

        #ifdef _GUI_ENABLE_RASTERIZER_ADJUSTMENT_
        bool                                adjustLineOriginLT_ = true;
        #endif

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================