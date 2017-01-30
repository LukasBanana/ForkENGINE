/*
 * GUI scroll area file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/ViewController/ScrollArea.h"
#include "GUI/Core/ColorScheme.h"
#include "GUI/Core/DrawContext.h"
#include "Core/Exception/NullPointerException.h"
#include "../EventSystem/ImportEvents.h"


namespace Fork
{

namespace GUI
{


/*
 * Internal functions
 */

#define HAS_FLAG(bitmask, flag) (((bitmask) & ScrollArea::Flags::flag) != 0)

#define FLAG_VERT_LEFT(bitmask) HAS_FLAG( bitmask, VertLeft     )
#define FLAG_HORZ_TOP(bitmask)  HAS_FLAG( bitmask, HorzTop     )


/*
 * ScrollBarEventHandler class
 */

ScrollArea::ScrollBarEventHandler::ScrollBarEventHandler(ScrollBar* owner, ViewController* virtualArea) :
    ScrollBar::ViewEventHandler { owner       },
    virtualArea_                { virtualArea }
{
    ASSERT_POINTER(virtualArea);
}

void ScrollArea::ScrollBarEventHandler::OnResize(SizeEvent& event)
{
    /* Update scroll position */
    Scroll(GetOwner()->VirtualScrollPosition());
}

void ScrollArea::ScrollBarEventHandler::Scroll(int position)
{
    if (GetOwner()->IsVertical())
        GetVirtualArea()->scrollOffset.y = -position;
    else
        GetVirtualArea()->scrollOffset.x = -position;
}


/*
 * ScrollArea class
 */

ScrollArea::ScrollArea() :
    vertScrollBar_{ true  },
    horzScrollBar_{ false }
{
    /* Initialize scroll bars */
    vertScrollBar_.SetEventHandler(std::make_shared<ScrollBarEventHandler>(&vertScrollBar_, &virtualArea));
    horzScrollBar_.SetEventHandler(std::make_shared<ScrollBarEventHandler>(&horzScrollBar_, &virtualArea));

    vertScrollBar_.SetupParent(this);
    horzScrollBar_.SetupParent(this);
    virtualArea.SetupParent(this);

    vertScrollBar_.ChangeFlags(ScrollBar::Flags::HideOnFullSize);
    horzScrollBar_.ChangeFlags(ScrollBar::Flags::HideOnFullSize);

    Finalize();
}

void ScrollArea::Resize(Size size)
{
    /* Call standard resize procedure and update scroll bars */
    ViewController::Resize(size);
    UpdateScrollBars();
}

void ScrollArea::ChangeFlags(const Flags::DataType flags)
{
    flags_ = flags;
    Finalize();
    UpdateScrollBars();
}

void ScrollArea::SetupVirtualSize(const Size& size)
{
    vertScrollBar_.model.SetupAreaSize(static_cast<float>(size.height));
    horzScrollBar_.model.SetupAreaSize(static_cast<float>(size.width));
    UpdateScrollBars();
}


/*
 * ======= Private: =======
 */

void ScrollArea::UpdateScrollBars()
{
    /* Update scroll bar location */
    static const int scrollBarSize = 15;
    auto clientArea = ClientArea();

    vertScrollBar_.Locate({ clientArea.Width() - scrollBarSize, 0 });
    vertScrollBar_.Resize({ scrollBarSize, clientArea.Height() - scrollBarSize });

    horzScrollBar_.Locate({ 0, clientArea.Height() - scrollBarSize });
    horzScrollBar_.Resize({ clientArea.Width() - scrollBarSize, scrollBarSize });

    /* Check if a scroll bar is hidden */
    if (horzScrollBar_.model.CanScroll())
    {
        if (!vertScrollBar_.model.CanScroll())
            horzScrollBar_.Resize({ clientArea.Width(), scrollBarSize });
    }
    else
    {
        if (vertScrollBar_.model.CanScroll())
            vertScrollBar_.Resize({ scrollBarSize, clientArea.Height() });
    }

    /* Update virutal area */
    Rect virtualRect = clientArea;

    if (horzScrollBar_.model.CanScroll())
    {
        if (FLAG_HORZ_TOP(GetFlags()))
            virtualRect.top += scrollBarSize;
        else
            virtualRect.bottom -= scrollBarSize;
    }

    if (vertScrollBar_.model.CanScroll())
    {
        if (FLAG_VERT_LEFT(GetFlags()))
            virtualRect.left += scrollBarSize;
        else
            virtualRect.right -= scrollBarSize;
    }

    virtualArea.Locate(virtualRect.LTPoint());
    virtualArea.Resize(virtualRect.Size());
}


} // /namespace GUI

} // /namespace Fork



// ========================