/*
 * GUI frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/ViewController/Frame.h"
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

#define HAS_FLAG(bitmask, flag) (((bitmask) & Frame::Flags::flag) != 0)

#define FLAG_TITLE_BAR(bitmask)     HAS_FLAG( bitmask, TitleBar     )
#define FLAG_CLOSE_BOX(bitmask)     HAS_FLAG( bitmask, CloseBox     )
#define FLAG_MIN_BOX(bitmask)       HAS_FLAG( bitmask, MinimizeBox  )
#define FLAG_MAX_BOX(bitmask)       HAS_FLAG( bitmask, MaxmimizeBox )
#define FLAG_SIZE_BOX(bitmask)      HAS_FLAG( bitmask, SizeBox      )
#define FLAG_BORDERLESS(bitmask)    HAS_FLAG( bitmask, Borderless   )
#define FLAG_SCROLL_AREA(bitmask)   HAS_FLAG( bitmask, ScrollArea   )


/*
 * ViewEventHandler class
 */

Frame::ViewEventHandler::ViewEventHandler(Frame* owner) :
    owner_{ owner }
{
    ASSERT_POINTER(owner);
}

void Frame::ViewEventHandler::OnMouseDown(MouseEvent& event)
{
    auto owner = GetOwner();

    const auto frameSize = owner->GetSize();
    const auto mousePos = event.GetLocalPosition();
    const auto flags = owner->GetFlags();

    if (event.GetKeyCode() == MouseKeyCodes::MouseLeft)
    {
        switch (GetState())
        {
            case States::Idle:
            {
                /* Initializer trigger rectangles */
                const auto statusBarSize = owner->GetConfig().statusBarSize;
                const Rect titleBarRect{ {}, Size{ frameSize.width, GetOwner()->GetConfig().captionSize } };
                const Rect sizeBoxRect{ frameSize.width - statusBarSize, frameSize.height - statusBarSize, frameSize.width, frameSize.height };

                /* Check if mouse is over title bar */
                if ( ( !FLAG_TITLE_BAR(flags) || titleBarRect.Overlap(mousePos) ) && LockMouseFocus(owner))
                {
                    /* Clamp mouse position */
                    if (owner->GetParent())
                        ClampMouse(owner->GetParent()->GlobalClientArea());

                    /* Start moving frame */
                    ChangeState(States::MoveFrame);
                }
                else if (FLAG_SIZE_BOX(flags) && sizeBoxRect.Overlap(mousePos) && LockMouseFocus(owner))
                {
                    /* Clamp mouse position */
                    auto globalPosition = owner->GlobalPosition();
                    auto minSize = owner->GetMinSize();
                    auto maxSize = owner->GetMaxSize();

                    ClampMouse({
                        globalPosition.x + minSize.width,
                        globalPosition.y + minSize.height,
                        std::max(globalPosition.x + maxSize.width , maxSize.width ),
                        std::max(globalPosition.y + maxSize.height, maxSize.height)
                    });

                    /* Start resizing frame */
                    ChangeState(States::ResizeFrame);
                }
            }
            break;
        }
    }
}

void Frame::ViewEventHandler::OnMouseUp(MouseEvent& event)
{
    if (event.GetKeyCode() == MouseKeyCodes::MouseLeft)
    {
        switch (GetState())
        {
            case States::MoveFrame:
            case States::ResizeFrame:
                ChangeState(States::Idle);
                break;
        }
    }
}

void Frame::ViewEventHandler::OnMouseMove(MouseEvent& event)
{
    auto owner = event.GetOwner();
    auto mouseMotion = event.GetMotion();

    switch (GetState())
    {
        case States::MoveFrame:
            owner->Move(mouseMotion);
            break;
        case States::ResizeFrame:
            owner->Resize(owner->GetSize() + Size(mouseMotion.x, mouseMotion.y));
            break;
    }
}

static void DrawFrameTitleBar(
    DrawContext* context, const Rect& titleBarRect, const String& title, const ColorScheme::Box& frameColors)
{
    context->DrawRect(
        titleBarRect,
        frameColors.top, frameColors.top,
        frameColors.bottom, frameColors.bottom
    );
    context->DrawFontText(titleBarRect.Center(), title, frameColors.title, true, true);
}

static void DrawFrameSizeBox(DrawContext* context, const Point& cornerRB, const Color& borderColor)
{
    for (int i = 1; i <= 3; ++i)
    {
        auto size = i*5;
        context->DrawLine(
            { cornerRB.x - size, cornerRB.y },
            { cornerRB.x, cornerRB.y - size },
            borderColor
        );
    }
}

static void DrawFrameStatusBar(DrawContext* context, const Rect& statusBarRect, const ColorScheme::Box& statusBar)
{
    context->DrawRect(
        statusBarRect,
        statusBar.top, statusBar.top,
        statusBar.bottom, statusBar.bottom
    );
}

void Frame::ViewEventHandler::OnDraw(DrawEvent& event)
{
    /* Get object references */
    auto context = event.GetContext();

    auto owner          = GetOwner();
    auto rect           = owner->GlobalArea();
    auto flags          = owner->GetFlags();
    const auto& scheme  = owner->colorScheme;

    const auto& config = owner->GetConfig();

    /* Draw frame */
    context->DrawRect(rect, scheme.ground);

    if (!FLAG_BORDERLESS(flags))
    {
        if (FLAG_SIZE_BOX(flags))
        {
            DrawFrameStatusBar(
                context,
                { rect.left, rect.bottom - config.statusBarSize - 1, rect.right, rect.bottom },
                GetActiveColorScheme().statusBar
            );
            DrawFrameSizeBox(context, { rect.right, rect.bottom }, scheme.border);
        }
        
        if (FLAG_TITLE_BAR(flags))
        {
            DrawFrameTitleBar(
                context,
                { rect.left, rect.top, rect.right, rect.top + config.captionSize },
                owner->GetTitle(),
                GetOwner()->TileBarColorScheme()
            );
        }

        context->DrawFrame(rect, scheme.border);
    }
}


/*
 * ======= Protected: =======
 */

void Frame::ViewEventHandler::ChangeState(const States state)
{
    switch (state)
    {
        case States::Idle:
            UnclampMouse();
            UnlockMouseFocus();
            break;
        case States::MoveFrame:
        case States::ResizeFrame:
            GetOwner()->MakeTopLevel();
            break;
        default:
            break;
    }
    state_ = state;
}


/*
 * Frame class
 */

Frame::Frame() :
    colorScheme{ GetActiveColorScheme().frame }
{
    MakeEventHandler<ViewEventHandler>(this);
    Finalize();
    UpdateScrollArea();
}

void Frame::Resize(Size size)
{
    /* Resize this frame and update scroll area */
    ViewController::Resize(size);
    UpdateScrollArea();
}

void Frame::ChangeFlags(const Flags::DataType flags)
{
    if (flags_ == flags)
        return;

    flags_ = flags;
    Finalize();

    /* Udpate scroll area configuartion */
    if (FLAG_SCROLL_AREA(GetFlags()))
    {
        /* Set scroll area as root parent */
        redirectParent = nullptr;
        scrollArea_.SetupParent(this);
        redirectParent = &(scrollArea_.virtualArea);

        /* Transfer all children to root parnet */
        auto children = GetChildren();
        for (auto child : children)
        {
            if (child != &scrollArea_)
                child->SetupParent(this);
        }

        /* Update scroll area */
        UpdateScrollArea();
    }
    else if (redirectParent)
    {
        auto children = redirectParent->GetChildren();

        /* Remove scroll area as root parent */
        redirectParent = nullptr;
        scrollArea_.SetupParent(nullptr);

        /* Transfer all children from root parnet */
        for (auto child : children)
            child->SetupParent(this);
    }
}

void Frame::SetupVirtualSize(const Size& size)
{
    scrollArea_.SetupVirtualSize(size);
}

const ColorScheme::Box& Frame::TileBarColorScheme() const
{
    if (!IsEnabled())
        return colorScheme.disabled;
    else if (IsTopLevel())
        return colorScheme.active;
    return colorScheme.inactive;
}


/*
 * ======= Private: =======
 */

Rect Frame::ClientAreaBorder() const
{
    Rect border{ 1, 1, 1, 1 };

    if (!FLAG_BORDERLESS(GetFlags()))
    {
        border.top = GetConfig().captionSize;
        if (FLAG_SIZE_BOX(GetFlags()))
            border.bottom += GetConfig().statusBarSize;
    }

    return border;
}

void Frame::UpdateScrollArea()
{
    scrollArea_.Resize(ClientArea().Size());
}


} // /namespace GUI

} // /namespace Fork



// ========================