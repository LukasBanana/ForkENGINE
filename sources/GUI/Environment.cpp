/*
 * GUI environment file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/Environment.h"
#include "GUI/EventSystem/MouseEvent.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/HWDrawContext.h"
#include "Core/STLHelper.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContext.h"
#include "EventSystem/MouseLock.h"


namespace Fork
{

namespace GUI
{


Environment::Environment(const Video::PrimitiveRendererPtr& renderer, const DrawContextPtr& context) :
    context_{ context }
{
    if (!context_)
    {
        context_ = std::make_shared<HWDrawContext>(
            renderer,
            Video::RenderSystem::Active(),
            Video::RenderContext::Active()
        );
    }

    /* Initialize environemnt client area */
    clientArea = { {}, Video::RenderContext::Active()->Resolution().Cast<int>() };
    desktop_.Resize(clientArea.Size());
}

void Environment::Update()
{
    desktop_.Update();
}

void Environment::Draw()
{
    context_->Begin();
    {
        desktop_.Draw(context_.get(), desktop_.GetPosition(), clientArea);
    }
    context_->End();
}

void Environment::SetupCursorPosition(Point position)
{
    /* Clamp mouse position into rectangle (if enabled) */
    GetMouseLock().ClampPoint(position);

    /* Get mouse motion and set cursor position */
    auto motion = position - cursor_.position;
    cursor_.position = position;

    /* Check for locked event handler */
    if (GetMouseLock().eventHandler)
    {
        /* Post mouse event only to locked event handler */
        auto owner = GetMouseLock().owner;
        MouseEvent event(owner, cursor_.position, cursor_.position - owner->GlobalPosition(), motion);
        GetMouseLock().eventHandler->OnMouseMove(event);
    }
    else
    {
        /* Post mouse event to view controller hierarchy */
        ViewController* focusedViewController = nullptr;
        desktop_.PostMouseMovement(position, motion, {}, focusedViewController);

        /* Check if focused view controller has switched */
        if (focusedViewController != prevFocusedViewController_)
        {
            /* Post lost- and get focus events */
            if (prevFocusedViewController_)
                prevFocusedViewController_->PostLostFocus();
            if (focusedViewController)
                focusedViewController->PostGetFocus(prevFocusedViewController_);

            /* Store new focused view controller */
            prevFocusedViewController_ = focusedViewController;
        }
    }
}

void Environment::PostMouseKeyDown(const MouseKeyCodes keyCode)
{
    /* Check for locked event handler */
    if (GetMouseLock().eventHandler)
    {
        auto owner = GetMouseLock().owner;
        MouseEvent event(owner, cursor_.position, cursor_.position - owner->GlobalPosition(), keyCode);
        GetMouseLock().eventHandler->OnMouseDown(event);
    }
    else
        desktop_.PostMouseKeyDown(cursor_.position, keyCode, {});
}

void Environment::PostMouseKeyUp(const MouseKeyCodes keyCode)
{
    /* Check for locked event handler */
    if (GetMouseLock().eventHandler)
    {
        auto owner = GetMouseLock().owner;
        MouseEvent event(owner, cursor_.position, cursor_.position - owner->GlobalPosition(), keyCode);
        GetMouseLock().eventHandler->OnMouseUp(event);
    }
    else
        desktop_.PostMouseKeyUp(cursor_.position, keyCode, {});
}

void Environment::PostWheelMotion(int wheelMotion)
{
    /* Check for locked event handler */
    if (GetMouseLock().eventHandler)
    {
        auto owner = GetMouseLock().owner;
        MouseEvent event(owner, cursor_.position, cursor_.position - owner->GlobalPosition(), wheelMotion);
        GetMouseLock().eventHandler->OnMouseUp(event);
    }
    else
        desktop_.PostWheelMotion(cursor_.position, wheelMotion, {});
}

Frame* Environment::CreateFrame(
    const Point& position, const Size& size, const String& title, ViewController* parent, const Frame::Flags::DataType flags)
{
    auto frame = CreateViewController<Frame>();
    
    InitViewController(frame, position, size, title, parent);
    frame->ChangeFlags(flags);

    return frame;
}

Button* Environment::CreateButton(
    const Point& position, const Size& size, const String& title, ViewController* parent, const Button::Flags::DataType flags)
{
    auto button = CreateViewController<Button>();

    InitViewController(button, position, size, title, parent);
    button->ChangeFlags(flags);

    return button;
}

ScrollBar* Environment::CreateScrollBar(
    const Point& position, const Size& size, bool isVertical, ViewController* parent, const ScrollBar::Flags::DataType flags)
{
    auto scrollBar = CreateViewController<ScrollBar>(isVertical);

    InitViewController(scrollBar, position, size, L"", parent);
    scrollBar->ChangeFlags(flags);

    return scrollBar;
}

void Environment::DeleteViewController(ViewController* viewCtrl)
{
    RemoveFromListIf(
        viewControllers_,
        [&viewCtrl](const std::unique_ptr<ViewController>& element)
        {
            return element.get() == viewCtrl;
        }
    );
}


/*
 * ======= Private: =======
 */

void Environment::InitViewController(
    ViewController* viewCtrl, const Point& position, const Size& size, const String& title, ViewController* parent)
{
    viewCtrl->Locate(position);
    viewCtrl->Resize(size);
    viewCtrl->SetupTitle(title);
    viewCtrl->SetupParent(parent != nullptr ? parent : &desktop_);
}


} // /namespace GUI

} // /namespace Fork



// ========================