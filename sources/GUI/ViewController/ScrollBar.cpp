/*
 * GUI scroll bar file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/ViewController/ScrollBar.h"
#include "GUI/Core/DrawContext.h"
#include "Core/Exception/NullPointerException.h"
#include "../EventSystem/ImportEvents.h"
#include "DrawHelper.h"


namespace Fork
{

namespace GUI
{


/*
 * Internal functions
 */

#define HAS_FLAG(bitmask, flag) (((bitmask) & ScrollBar::Flags::flag) != 0)

#define FLAG_SINGLE_STEP_BUTTONS(bitmask)   HAS_FLAG( bitmask, SingleStepButtons )
#define FLAG_HIDE_ON_FULL_SIZE(bitmask)     HAS_FLAG( bitmask, HideOnFullSize    )


/*
 * ViewEventHandler class
 */

ScrollBar::ViewEventHandler::ViewEventHandler(ScrollBar* owner) :
    owner_{ owner }
{
    ASSERT_POINTER(owner);
}

void ScrollBar::ViewEventHandler::OnMouseDown(MouseEvent& event)
{
    auto owner = GetOwner();

    if (event.GetKeyCode() == MouseKeyCodes::MouseLeft)
    {
        switch (GetState())
        {
            case States::Idle:
            {
                if (forwardButtonState_.isMouseOver && LockMouseFocus(owner))
                    ChangeState(States::Forwards);
                else if (backwardButtonState_.isMouseOver && LockMouseFocus(owner))
                    ChangeState(States::Backwards);
                else if (dragButtonState_.isMouseOver && LockMouseFocus(owner))
                {
                    if (owner->IsVertical())
                        dragOffset_ = event.GetLocalPosition().y - owner->ScrollPosition();
                    else
                        dragOffset_ = event.GetLocalPosition().x - owner->ScrollPosition();
                    ChangeState(States::Drag);
                }
            }
            break;
        }
    }
}

void ScrollBar::ViewEventHandler::OnMouseUp(MouseEvent& event)
{
    if (event.GetKeyCode() == MouseKeyCodes::MouseLeft)
    {
        switch (GetState())
        {
            case States::Forwards:
            case States::Backwards:
            case States::Drag:
                ChangeState(States::Idle);
                break;
        }
    }
}

void ScrollBar::ViewEventHandler::OnMouseMove(MouseEvent& event)
{
    auto owner          = GetOwner();
    auto rect           = Rect({}, owner->GetSize());
    auto localMousePos  = event.GetLocalPosition();

    switch (GetState())
    {
        case States::Idle:
            /* Get mouse over state */
            forwardButtonState_ .isMouseOver = owner->ForwardButtonRect (rect).Overlap(localMousePos);
            backwardButtonState_.isMouseOver = owner->BackwardButtonRect(rect).Overlap(localMousePos);
            dragButtonState_    .isMouseOver = owner->DragButtonRect    (rect).Overlap(localMousePos);
            break;

        case States::Drag:
            /* Update virutal scroll bar position */
            if (owner->IsVertical())
                owner->SetupScrollPosition(localMousePos.y - dragOffset_);
            else
                owner->SetupScrollPosition(localMousePos.x - dragOffset_);
            Scroll(owner->VirtualScrollPosition());
            break;
    }
}

void ScrollBar::ViewEventHandler::OnWheelMotion(MouseEvent& event)
{
    if (GetState() == States::Idle)
    {
        auto motion = event.GetWheelMotion();
        
        if (motion != 0)
        {
            auto owner = GetOwner();
            owner->model.Scroll(static_cast<float>(-owner->scrollWheelSpeed * motion));
            Scroll(owner->VirtualScrollPosition());
        }
    }
}

static void DrawButton(DrawContext* context, const Rect& rect, const ColorScheme::Box& buttonColor, const Color& borderColor)
{
    context->DrawRect(
        rect,
        buttonColor.top, buttonColor.top,
        buttonColor.bottom, buttonColor.bottom
    );
    context->DrawFrame(rect, borderColor);
}

static void DrawButtonFliped(DrawContext* context, const Rect& rect, const ColorScheme::Box& buttonColor, const Color& borderColor)
{
    context->DrawRect(
        rect,
        buttonColor.top, buttonColor.bottom,
        buttonColor.bottom, buttonColor.top
    );
    context->DrawFrame(rect, borderColor);
}

void ScrollBar::ViewEventHandler::OnUpdate(UpdateEvent& event)
{
    auto owner = GetOwner();

    switch (GetState())
    {
        case States::Forwards:
            owner->model.Scroll(static_cast<float>(owner->scrollSpeed));
            Scroll(owner->VirtualScrollPosition());
            break;
        case States::Backwards:
            owner->model.Scroll(static_cast<float>(-owner->scrollSpeed));
            Scroll(owner->VirtualScrollPosition());
            break;
    }
}

void ScrollBar::ViewEventHandler::OnDraw(DrawEvent& event)
{
    /* Get object references */
    auto context = event.GetContext();

    auto owner          = GetOwner();
    auto rect           = owner->GlobalArea();
    auto flags          = owner->GetFlags();
    auto size           = owner->GetSize();

    /* Get color scheme */
    const auto& scheme      = owner->colorScheme;
    const auto& borderColor = scheme.buttons.border.outer;

    /* Check if drawing can be omitted */
    if (FLAG_HIDE_ON_FULL_SIZE(flags) && !owner->model.CanScroll())
    {
        event.Veto();
        return;
    }
    
    /* Draw scroll bar */
    context->DrawRect(rect, owner->colorScheme.ground);

    if (owner->IsVertical())
    {
        DrawButtonFliped(context, owner->DragButtonRect(rect), ButtonColor(scheme.buttons, GetDragButtonState()), borderColor);
        DrawButtonFliped(context, owner->ForwardButtonRect(rect), ButtonColor(scheme.buttons, GetForwardButtonState()), borderColor);
        DrawButtonFliped(context, owner->BackwardButtonRect(rect), ButtonColor(scheme.buttons, GetBackwardButtonState()), borderColor);
    }
    else
    {
        DrawButton(context, owner->DragButtonRect(rect), ButtonColor(scheme.buttons, GetDragButtonState()), borderColor);
        DrawButton(context, owner->ForwardButtonRect(rect), ButtonColor(scheme.buttons, GetForwardButtonState()), borderColor);
        DrawButton(context, owner->BackwardButtonRect(rect), ButtonColor(scheme.buttons, GetBackwardButtonState()), borderColor);
    }
}


/*
 * ======= Protected: =======
 */

void ScrollBar::ViewEventHandler::ChangeState(const States state)
{
    switch (state)
    {
        case States::Idle:
            UnlockMouseFocus();
            forwardButtonState_ .isPushed = false;
            backwardButtonState_.isPushed = false;
            dragButtonState_    .isPushed = false;
            break;
        case States::Forwards:
            forwardButtonState_ .isPushed = true;
            break;
        case States::Backwards:
            backwardButtonState_.isPushed = true;
            break;
        case States::Drag:
            dragButtonState_    .isPushed = true;
            break;
        default:
            break;
    }
    state_ = state;
}

const ColorScheme::Box& ScrollBar::ViewEventHandler::ButtonColor(
    const ColorScheme::Button& buttonColors, const ButtonState& buttonState) const
{
    if (GetOwner()->IsEnabled())
    {
        if (HasFocus())
        {
            if (buttonState.isPushed)
                return buttonColors.pushed;
            else if (buttonState.isMouseOver)
                return buttonColors.selected;
        }
        return buttonColors.normal;
    }
    return buttonColors.disabled;
}

void ScrollBar::ViewEventHandler::Scroll(int direction)
{
    // dummy
}


/*
 * ScrollBar class
 */

ScrollBar::ScrollBar(bool isVertical) :
    colorScheme{ GetActiveColorScheme().scrollBar },
    isVertical_{ isVertical                       }
{
    MakeEventHandler<ViewEventHandler>(this);

    /* Initialize buttons */
    model.SetupMinBarSize(10.0f);

    Finalize();
}

void ScrollBar::Resize(Size size)
{
    /* Call standard resize procedure and update bar size */
    ViewController::Resize(size);
    UpdateBarSize();
}

void ScrollBar::ChangeFlags(const Flags::DataType flags)
{
    flags_ = flags;
    Finalize();
}

int ScrollBar::ScrollPosition() const
{
    return static_cast<int>(model.BarPosition());
}

void ScrollBar::SetupScrollPosition(int position)
{
    model.SetupBarPosition(static_cast<float>(position));
}

int ScrollBar::ScrollBarSize() const
{
    return static_cast<int>(std::ceil(model.BarSize()));
}

int ScrollBar::VirtualScrollPosition() const
{
    return static_cast<int>(std::ceil(model.ScrollOffset()));
}


/*
 * ======= Protected: =======
 */

Rect ScrollBar::ForwardButtonRect(const Rect& globalArea) const
{
    return IsVertical() ?
        Rect(globalArea.left, globalArea.bottom - GetSize().width, globalArea.right, globalArea.bottom) :
        Rect(globalArea.right - GetSize().height, globalArea.top, globalArea.right, globalArea.bottom);
}

Rect ScrollBar::BackwardButtonRect(const Rect& globalArea) const
{
    return IsVertical() ?
        Rect(globalArea.left, globalArea.top, globalArea.right, globalArea.top + GetSize().width) :
        Rect(globalArea.left, globalArea.top, globalArea.left + GetSize().height, globalArea.bottom);
}

Rect ScrollBar::DragButtonRect(const Rect& globalArea) const
{
    auto barSize = ScrollBarSize();
    auto barPos = ScrollPosition();

    if (IsVertical())
    {
        barPos += globalArea.top + GetSize().width;
        return Rect(globalArea.left, barPos, globalArea.right, barPos + barSize);
    }

    barPos += globalArea.left + GetSize().height;
    return Rect(barPos, globalArea.top, barPos + barSize, globalArea.bottom);
}


/*
 * ======= Private: =======
 */

void ScrollBar::UpdateBarSize()
{
    auto size = GetSize();
    if (IsVertical())
    {
        auto buttonSize = size.width;
        model.SetupSize(static_cast<float>(size.height - buttonSize*2));
    }
    else
    {
        auto buttonSize = size.height;
        model.SetupSize(static_cast<float>(size.width - buttonSize*2));
    }
}



} // /namespace GUI

} // /namespace Fork



// ========================