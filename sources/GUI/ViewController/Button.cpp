/*
 * GUI button file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/ViewController/Button.h"
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

#define HAS_FLAG(bitmask, flag) (((bitmask) & Button::Flags::flag) != 0)

#define FLAG_TOGGLE(bitmask)        HAS_FLAG( bitmask, Toggle      )
#define FLAG_SINGLE_FRAME(bitmask)  HAS_FLAG( bitmask, SingleFrame )
#define FLAG_DOUBLE_FRAME(bitmask)  HAS_FLAG( bitmask, DoubleFrame )
#define FLAG_PUSH_CAPTURE(bitmask)  HAS_FLAG( bitmask, PushCapture )


/*
 * ViewEventHandler class
 */

Button::ViewEventHandler::ViewEventHandler(Button* owner) :
    owner_{ owner }
{
    ASSERT_POINTER(owner);
}

void Button::ViewEventHandler::OnMouseDown(MouseEvent& event)
{
    auto owner = GetOwner();

    if (event.GetKeyCode() == MouseKeyCodes::MouseLeft)
    {
        switch (GetState())
        {
            case States::Idle:
            {
                /* Change state to <pushed> */
                if (LockMouseFocus(owner))
                    ChangeState(States::Pushed);
            }
            break;
        }
    }
}

void Button::ViewEventHandler::OnMouseUp(MouseEvent& event)
{
    if (event.GetKeyCode() == MouseKeyCodes::MouseLeft)
    {
        switch (GetState())
        {
            case States::Pushed:
                if (IsMouseOver())
                    event.GetOwner()->PostTrigger(0);
                ChangeState(States::Idle);
                break;
        }
    }
}

void Button::ViewEventHandler::OnMouseMove(MouseEvent& event)
{
    auto owner = event.GetOwner();
    isMouseOver_ = Rect({}, owner->GetSize()).Overlap(event.GetLocalPosition());
}

void Button::ViewEventHandler::OnDraw(DrawEvent& event)
{
    /* Get object references */
    auto context = event.GetContext();

    auto owner          = GetOwner();
    auto rect           = owner->GlobalArea();
    auto flags          = owner->GetFlags();
    const auto& scheme  = owner->colorScheme;

    /* Choose color scheme by state */
    const ColorScheme::Box* boxColors = &(scheme.normal);

    if (owner->IsEnabled())
    {
        if (HasFocus())
        {
            if ( GetState() == States::Pushed && ( IsMouseOver() || FLAG_PUSH_CAPTURE(flags) ) )
                boxColors = &(scheme.pushed);
            else
                boxColors = &(scheme.selected);
        }
    }
    else
        boxColors = &(scheme.disabled);

    /* Draw button */
    context->DrawRect(
        rect,
        boxColors->top, boxColors->top,
        boxColors->bottom, boxColors->bottom
    );

    context->DrawFontText(rect.Center(), owner->GetTitle(), boxColors->title, true, true);

    /* Draw border */
    if (FLAG_DOUBLE_FRAME(flags))
        DrawHelper::Draw3DFrame(context, rect, scheme.border);
    else if (FLAG_SINGLE_FRAME(flags))
        context->DrawFrame(rect, scheme.border.outer);
}

void Button::ViewEventHandler::OnTrigger(TriggerEvent& event)
{
    auto owner = GetOwner();
    if (owner->triggerCallback_)
        owner->triggerCallback_(owner);
}


/*
 * ======= Protected: =======
 */

void Button::ViewEventHandler::ChangeState(const States state)
{
    switch (state)
    {
        case States::Idle:
            UnlockMouseFocus();
            break;
        default:
            break;
    }
    state_ = state;
}


/*
 * Button class
 */

Button::Button() :
    colorScheme{ GetActiveColorScheme().button }
{
    MakeEventHandler<ViewEventHandler>(this);
    Finalize();
}

void Button::ChangeFlags(const Flags::DataType flags)
{
    flags_ = flags;
    Finalize();
}


/*
 * ======= Private: =======
 */

Rect Button::ClientAreaBorder() const
{
    Rect border;

    if (FLAG_DOUBLE_FRAME(GetFlags()))
        border = { 2, 2, 2, 2 };
    else if (FLAG_SINGLE_FRAME(GetFlags()))
        border = { 1, 1, 1, 1 };

    return border;
}


} // /namespace GUI

} // /namespace Fork



// ========================