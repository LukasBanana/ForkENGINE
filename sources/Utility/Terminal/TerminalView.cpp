/*
 * Terminal view file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Terminal/TerminalView.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/StringModifier.h"
#include "Core/STLHelper.h"
#include "Video/RenderSystem/RenderSystem.h"


namespace Fork
{

namespace Utility
{


static const float cmdLineTextOffset    = 5;
static const float cmdLineSpace         = 4;

TerminalView::TerminalView(
    Video::PrimitiveRenderer* renderer, const TerminalModel* model, const Video::TextureFont* cmdLineFont) :
        model_      { model       },
        cmdLineFont_{ cmdLineFont }
{
    ASSERT_POINTER( renderer    );
    ASSERT_POINTER( model       );
    ASSERT_POINTER( cmdLineFont );

    /* Create rasterizer states to enable and disable scissoring */
    Video::RasterizerState::Description scissorStateDesc;

    scissorStateDesc.isScissorTestEnabled = true;
    drawStateBegin_ = renderer->GetRenderSystem()->CreateRasterizerState(scissorStateDesc);

    scissorStateDesc.isScissorTestEnabled = false;
    drawStateEnd_ = renderer->GetRenderSystem()->CreateRasterizerState(scissorStateDesc);
}
TerminalView::~TerminalView()
{
}

void TerminalView::Draw(
    Video::PrimitiveRenderer* renderer, const Math::Rectf& viewRect, bool showCursor)
{
    auto renderContext = Video::RenderContext::Active();

    ASSERT_POINTER(renderer);
    
    /* Draw background */
    DrawBackground(renderer, viewRect);

    /* Setup entry start position */
    const auto cmdLinePosY = viewRect.bottom - cmdLineFont_->FontHeight() - cmdLineSpace*2;
    Math::Point2f entryPos(cmdLineTextOffset + viewRect.left, cmdLinePosY - cmdLineTextOffset);

    /* Apply scroll bar displacement */
    entryPos.y += std::floor(model_->scrollBar.ScrollOffset());

    /* Draw visible entries */
    auto scissorRect = viewRect.Cast<int>();
    scissorRect.bottom = static_cast<int>(cmdLinePosY);

    renderContext->ChangeState(drawStateBegin_.get());
    renderContext->SetupScissor(
        { scissorRect.LTPoint(), scissorRect.Size() }
    );

    const auto& entryList = model_->GetEntries();
    const size_t numEntries = entryList.size();

    if (numEntries > 0)
    {
        int indexEnd = static_cast<int>(numEntries) - 1;

        for (int i = indexEnd; i >= 0; --i)
        {
            auto entry = entryList[static_cast<size_t>(i)];

            /* Find entry which indicates a new line */
            if (entry->IndicateNewLine() && i < indexEnd)
            {
                DrawEntryRange(
                    renderer, i, indexEnd, entryPos, viewRect
                );
                indexEnd = i;
            }

            /* Cancel drawing when the entry position is outside the screen */
            if (entryPos.y < 0.0f)
            {
                /* Invalidate end-index to skip draw the first line */
                indexEnd = -1;
                break;
            }
        }

        /* Draw first line (which has been dismissed inside the draw loop) */
        DrawEntryRange(
            renderer, -1, indexEnd, entryPos, viewRect
        );
    }

    renderContext->ChangeState(drawStateEnd_.get());

    /* Draw scroll bar */
    if (model_->scrollBar.CanScroll())
    {
        const Math::Rectf scrollBarRect(
            viewRect.right - scrollBarWidth, viewRect.top,
            viewRect.right, cmdLinePosY
        );
        DrawScrollBar(renderer, model_->scrollBar, scrollBarRect);
    }

    /* Draw command line */
    DrawCommandLine(
        renderer, viewRect, { 0, cmdLinePosY }, model_->GetText(), showCursor
    );
}

Math::Rectf TerminalView::ViewArea(const Math::Rectf& viewRect) const
{
    return
    {
        viewRect.left,
        viewRect.top,
        viewRect.right,
        viewRect.bottom - cmdLineFont_->FontHeight() - cmdLineSpace*2 - cmdLineTextOffset*2
    };
}


/*
 * ======= Protected: =======
 */

void TerminalView::DrawBackground(Video::PrimitiveRenderer* renderer, const Math::Rectf& viewRect)
{
    renderer->DrawRect(viewRect, groundColor);
}

void TerminalView::DrawCursor(Video::PrimitiveRenderer* renderer, const Math::Point2f& position)
{
    int cursorWidth = 2;
    bool isInsertionVisible = false;

    if (model_->insertionEnabled && model_->CharRight() != 0)
    {
        /* Resize cursor width to visualize insertion mode */
        cursorWidth = cmdLineFont_->TextWidth(std::string(1, model_->CharRight()));
        isInsertionVisible = true;
    }

    /* Draw cursor rectangle */
    renderer->DrawRect(
        { { position.x, position.y },
          Math::Size2i(cursorWidth, cmdLineFont_->FontHeight()).Cast<float>() },
        { 1, 1, 1, groundColor.a }
    );

    /* If insertion mode is visible, draw the active character again */
    if (isInsertionVisible)
    {
        renderer->DrawText2D(
            cmdLineFont_, position,
            std::string(1, model_->CharRight()), { 0, 0, 0, groundColor.a }
        );
    }
}

void TerminalView::DrawCommandLine(
    Video::PrimitiveRenderer* renderer, const Math::Rectf& viewRect,
    const Math::Point2f& cmdLinePos, const std::string& cmdLineText, bool showCursor)
{
    float cmdLinePosX = cmdLineTextOffset;

    const Video::ColorRGBAf whiteColor { 1, 1, 1, groundColor.a };

    if (model_->mode == TerminalModel::Modes::Script)
    {
        static const wchar_t* scriptModeIdent = L">>>";
        
        renderer->DrawText2D(
            cmdLineFont_, { cmdLinePosX, cmdLinePos.y + cmdLineSpace },
            scriptModeIdent, whiteColor
        );

        cmdLinePosX += cmdLineFont_->TextWidth(scriptModeIdent) + cmdLineTextOffset;
    }

    renderer->DrawText2D(
        cmdLineFont_, { cmdLinePosX, cmdLinePos.y + cmdLineSpace }, cmdLineText, whiteColor
    );

    renderer->DrawLine(
        { { viewRect.left, cmdLinePos.y }, { viewRect.right, cmdLinePos.y } },
        whiteColor
    );

    /* Draw cursor */
    if (showCursor)
    {
        if (cursorBlinker.IsVisible())
        {
            const auto textWidth = cmdLineFont_->TextWidth(cmdLineText, 0, model_->GetCursorPosition());
            DrawCursor(
                renderer,
                { cmdLinePosX + viewRect.left + textWidth, cmdLinePos.y + cmdLineSpace }
            );
        }
    }
}

void TerminalView::DrawEntry(
    Video::PrimitiveRenderer* renderer, TerminalModel::Entry* entry, const Math::Point2f& position)
{
    switch (entry->Type())
    {
        case TerminalModel::Entry::Types::Text:
            DrawTextEntry(renderer, static_cast<TerminalModel::TextEntry*>(entry), position);
            break;

        case TerminalModel::Entry::Types::SourceCode:
            DrawSourceCodeEntry(renderer, static_cast<TerminalModel::SourceCodeEntry*>(entry), position);
            break;

        case TerminalModel::Entry::Types::Texture:
            DrawTextureEntry(renderer, static_cast<TerminalModel::TextureEntry*>(entry), position);
            break;

        default:
            break;
    }
}

void TerminalView::DrawTextEntry(
    Video::PrimitiveRenderer* renderer, TerminalModel::TextEntry* entry, const Math::Point2f& position)
{
    renderer->DrawText2D(
        entry->GetFont(), position, entry->GetText(),
        { entry->GetColor(), groundColor.a }
    );
}

void TerminalView::DrawSourceCodeEntry(
    Video::PrimitiveRenderer* renderer, TerminalModel::SourceCodeEntry* entry, const Math::Point2f& position)
{
    renderer->DrawText2D(
        cmdLineFont_, position, "] " + entry->GetCodeLine(),
        { 0, 1, 1, groundColor.a }
    );
}

void TerminalView::DrawTextureEntry(
    Video::PrimitiveRenderer* renderer, TerminalModel::TextureEntry* entry, const Math::Point2f& position)
{
    switch (entry->GetTexture()->Type())
    {
        case Video::Texture::Types::Tex2D:
        {
            auto tex = static_cast<const Video::Texture2D*>(entry->GetTexture());
            renderer->DrawImage(tex, position, { 1, 1, 1, groundColor.a });
        }
        break;

        default:
        break;
    }
}

void TerminalView::DrawScrollBar(
    Video::PrimitiveRenderer* renderer, const ScrollBarModel& scrollBar, const Math::Rectf& scrollBarRect)
{
    renderer->DrawRect(
        scrollBarRect, { 1, 1, 1, groundColor.a * 0.5f }
    );

    const auto scrollBarPosY    = std::floor(scrollBarRect.top + scrollBar.BarPosition(true, scrollBarRect.Height()));
    const auto scrollBarHeight  = std::floor(scrollBar.BarSize(scrollBarRect.Height()));

    renderer->DrawRect(
        {
            scrollBarRect.left + scrollBarBorder,
            scrollBarPosY + scrollBarBorder,
            scrollBarRect.right - scrollBarBorder,
            scrollBarPosY + scrollBarHeight - scrollBarBorder
        },
        { 1, 1, 1, groundColor.a }
    );
}


/*
 * ======= Private: =======
 */

void TerminalView::DrawEntryRange(
    Video::PrimitiveRenderer* renderer,
    int indexBegin, int indexEnd,
    Math::Point2f& entryPos, const Math::Rectf& viewRect)
{
    if (indexBegin >= indexEnd)
        return;

    float maxHeight = 0.0f;

    auto pos = entryPos;

    for (++indexBegin; indexBegin <= indexEnd; ++indexBegin)
    {
        const size_t index = static_cast<size_t>(indexBegin);
        const auto& entry = model_->GetEntries()[index].get();

        /* Store maximal entry height for this range */
        const auto size = entry->Size().Cast<float>();
        if (maxHeight < size.height)
            maxHeight = size.height;

        pos.y = entryPos.y - size.height;

        if (pos.y < viewRect.bottom)
        {
            /* Draw current entry */
            DrawEntry(renderer, entry, pos);
        }

        /* Increase entry position on X axis to append further entries */
        pos.x += size.width;
    }

    /* Move entry position by largest entry height */
    entryPos.y -= maxHeight;
}


} // /namespace Utility

} // /namespace Fork



// ========================