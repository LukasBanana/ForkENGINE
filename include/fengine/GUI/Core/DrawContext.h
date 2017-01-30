/*
 * GUI draw context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_DRAW_CONTEXT_H__
#define __FORK_GUI_DRAW_CONTEXT_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "GUI/Core/BaseGUITypes.h"
#include "GUI/Core/ColorScheme.h"

#include <stack>


namespace Fork
{

namespace GUI
{


DECL_SHR_PTR(DrawContext);

//! Abstract font type.
typedef void* Font;

/**
The draw context interface is used to abstract the basic
drawing operations used when drawing all GUI elements.
*/
class FORK_EXPORT DrawContext
{
    
    public:
        
        virtual ~DrawContext()
        {
        }

        //! Begins with drawing.
        virtual void Begin() = 0;
        //! Ends with drawing.
        virtual void End() = 0;

        //! Draws a single 2D point.
        virtual void DrawPoint(const Point& point, const Color& color) = 0;

        //! Draws a 2D line.
        virtual void DrawLine(const Point& startPoint, const Point& endPoint, const Color& color) = 0;
        //! Draws a multi-colored 2D line.
        virtual void DrawLine(const Point& startPoint, const Point& endPoint, const Color& startColor, const Color& endColor) = 0;

        //! Draws a filled 2D rectangle.
        virtual void DrawRect(const Rect& rect, const Color& color) = 0;
        //! Draws a filled, multi-colored 2D rectangle.
        virtual void DrawRect(const Rect& rect, const Color& colorLT, const Color& colorRT, const Color& colorRB, const Color& colorLB) = 0;

        //! Draws a wired 2D rectangle frame.
        virtual void DrawFrame(const Rect& rect, const Color& color) = 0;
        //! Draws a wired, multi-colored 2D rectangle frame.
        virtual void DrawFrame(const Rect& rect, const Color& colorLT, const Color& colorRT, const Color& colorRB, const Color& colorLB) = 0;

        //! Draws a 2D bezier curve.
        virtual void DrawBezier(const BezierCurve& curve, const Color& color) = 0;
        //! Draws a multi-colored 2D bezier curve.
        virtual void DrawBezier(const BezierCurve& curve, const Color& startColor, const Color& endColor) = 0;

        //! Loads a font with the specified configuration.
        virtual Font LoadFont(
            const String& fontName, int fontSize,
            bool isBold = false, bool isItalic = false, bool isUnderlined = false, bool isStrikeOut = false
        ) = 0;

        //! Pushes the specified font onto the stack. Text drawing will be done with the top level font. There is always a default font.
        virtual void PushFont(Font font) = 0;
        //! Pops the previous font from the stack.
        virtual void PopFont() = 0;

        //! Draws the specified text with the current font.
        virtual void DrawFontText(
            const Point& position, const String& text, const Color& color,
            bool isCenteredHorz = false, bool isCenteredVert = false
        ) = 0;

        /**
        Pushes the specified scissor onto the stack.
        New scissors will be clamped to get smaller or equal to the previous scissor.
        They can not become larger than the previous one.
        */
        void PushScissor(Rect rect);
        //! Pops the previous scissor from the stack.
        void PopScissor();

    protected:
        
        DrawContext() = default;

        //! Sets the scissor to draw only inside the specified area.
        virtual void Scissor(const Rect& rect) = 0;
        //! Sets the scissor to the entire screen.
        virtual void Scissor() = 0;

    private:
        
        std::stack<Rect> scissorStack_;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================