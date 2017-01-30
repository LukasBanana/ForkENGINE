/*
 * Terminal view header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_TERMINAL_VIEW_H__
#define __FORK_UTILITY_TERMINAL_VIEW_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/RenderSystem/RenderState/RasterizerState.h"
#include "Utility/Terminal/TerminalModel.h"
#include "Utility/BlinkModel.h"

#include <chrono>


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(TerminalView);

//! Terminal view class.
class FORK_EXPORT TerminalView
{

    public:

        //! \throws NullPointerException
        TerminalView(
            Video::PrimitiveRenderer* renderer, const TerminalModel* model, const Video::TextureFont* cmdLineFont
        );
        virtual ~TerminalView();

        /**
        Draws the terminal model. This should be called between
        "Video::PrimitiveRenderer::Begin/ End" function calls.
        \param[in] renderer Raw-pointer to the primitive renderer.
        \param[in] viewRect Specifies the view rectangle for the terminal.
        \param[in] showCursor Specifies whether the cursor is to be shown or not.
        This can be disable, to show that the terminal is inactive. By default true.
        \throws NullPointerException If renderer is a null pointer.
        */
        void Draw(Video::PrimitiveRenderer* renderer, const Math::Rectf& viewRect, bool showCursor = true);

        /**
        Returns the final view area rectangle.
        \param[in] viewRect Specifies the entire terminal view rectangle. This should be the same as for the 'Draw' function.
        \see Draw
        */
        Math::Rectf ViewArea(const Math::Rectf& viewRect) const;

        //! Background color. By default (0, 0, 0, 1).
        Video::ColorRGBAf groundColor = { 0, 0, 0, 1 };

        //! Scroll bar width. By default 10.0. This should not be negative.
        float scrollBarWidth = 10.0f;

        //! Scroll bar border size. By default 2.0. This should not be negative.
        float scrollBarBorder = 2.0f;

        //! Cursor blinkder model.
        BlinkModel cursorBlinker;

    protected:
        
        /* === Functions === */

        virtual void DrawBackground(Video::PrimitiveRenderer* renderer, const Math::Rectf& viewRect);
        virtual void DrawCursor(Video::PrimitiveRenderer* renderer, const Math::Point2f& position);

        virtual void DrawCommandLine(
            Video::PrimitiveRenderer* renderer, const Math::Rectf& viewRect,
            const Math::Point2f& cmdLinePos, const std::string& cmdLineText, bool showCursor
        );

        void DrawEntry(
            Video::PrimitiveRenderer* renderer, TerminalModel::Entry* entry, const Math::Point2f& position
        );

        virtual void DrawTextEntry(
            Video::PrimitiveRenderer* renderer, TerminalModel::TextEntry* entry, const Math::Point2f& position
        );
        virtual void DrawSourceCodeEntry(
            Video::PrimitiveRenderer* renderer, TerminalModel::SourceCodeEntry* entry, const Math::Point2f& position
        );
        virtual void DrawTextureEntry(
            Video::PrimitiveRenderer* renderer, TerminalModel::TextureEntry* entry, const Math::Point2f& position
        );

        virtual void DrawScrollBar(
            Video::PrimitiveRenderer* renderer, const ScrollBarModel& scrollBar, const Math::Rectf& scrollBarRect
        );

        inline const TerminalModel* GetModel() const
        {
            return model_;
        }
        inline const Video::TextureFont* GetCommandLineFont() const
        {
            return cmdLineFont_;
        }

    private:
        
        /* === Functions === */

        void DrawEntryRange(
            Video::PrimitiveRenderer* renderer,
            int indexBegin, int indexEnd,
            Math::Point2f& entryPos, const Math::Rectf& viewRect
        );

        /* === Members === */

        const TerminalModel* model_ = nullptr;
        const Video::TextureFont* cmdLineFont_ = nullptr;

        std::chrono::system_clock::time_point lastCursorBlinkTime_; //!< Last time point the cursor has blinked.

        Video::RasterizerStatePtr drawStateBegin_, drawStateEnd_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================