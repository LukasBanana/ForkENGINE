/*
 * Mouse file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/InputDevice/Mouse.h"
#include "Core/StaticConfig.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Core/STLHelper.h"

#include <algorithm>


namespace Fork
{

namespace IO
{


/*
 * Mouse::EventHandler class
 */

Mouse::EventHandler::~EventHandler()
{
}

bool Mouse::EventHandler::HasFocus() const
{
    return Mouse::Instance()->GetLockedEventHandler() == this;
}

bool Mouse::EventHandler::OnButtonDown(const MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    return true;
}

bool Mouse::EventHandler::OnButtonUp(const MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    return true;
}

bool Mouse::EventHandler::OnButtonDoubleClicked(const MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    return true;
}

bool Mouse::EventHandler::OnWheelMotion(int motion, const Platform::Frame* frame)
{
    return true;
}

bool Mouse::EventHandler::OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame)
{
    return true;
}

bool Mouse::EventHandler::OnGlobalMotion(const Math::Vector2i& motion)
{
    return true;
}

void Mouse::EventHandler::OnFocus()
{
    /* Dummy */
}

void Mouse::EventHandler::OnLostFocus()
{
    /* Dummy */
}

bool Mouse::EventHandler::LockFocus()
{
    return Mouse::Instance()->LockFocus(this);
}

bool Mouse::EventHandler::UnlockFocus()
{
    return Mouse::Instance()->UnlockFocus(this);
}


/*
 * Mouse class
 */

Mouse* Mouse::Instance()
{
    static Mouse instance;
    return &instance;
}

void Mouse::PostEventButtonDown(const MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    /* Get index from key code */
    const size_t index = static_cast<size_t>(keyCode);

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    if (index >= Mouse::numButtons)
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif

    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnButtonDown(keyCode, frame))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnButtonDown(keyCode, frame))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set button-hit state */
    auto& buttonState = buttonStates_[index];

    buttonState.hit     = !buttonState.doubleClicked;
    buttonState.down    = true;

    ++numButtonsDown_;
}

void Mouse::PostEventButtonUp(const MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    /* Get index from key code */
    const size_t index = static_cast<size_t>(keyCode);

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    if (index >= Mouse::numButtons)
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif

    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnButtonUp(keyCode, frame))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnButtonUp(keyCode, frame))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set button-released state */
    auto& buttonState = buttonStates_[index];

    buttonState.released    = true;
    buttonState.down        = false;

    if (numButtonsDown_ > 0)
        --numButtonsDown_;
    /*else
        < Warning?! >*/
}

void Mouse::PostEventButtonDoubleClicked(const MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    /* Get index from key code */
    const size_t index = static_cast<size_t>(keyCode);

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    if (index >= Mouse::numButtons)
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif

    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnButtonDoubleClicked(keyCode, frame))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnButtonDoubleClicked(keyCode, frame))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set button-double-clicked state */
    buttonStates_[index].doubleClicked = true;
}

void Mouse::PostEventWheelMotion(int motion, const Platform::Frame* frame)
{
    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnWheelMotion(motion, frame))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnWheelMotion(motion, frame))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set mouse wheel state */
    mouseWheel_ += motion;
}

void Mouse::PostEventLocalMotion(const Math::Point2i& position, const Platform::Frame* frame)
{
    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnLocalMotion(position, frame))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnLocalMotion(position, frame))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set motion state */
    frameMousePos_.position = position;
    frameMousePos_.frame    = frame;
}

void Mouse::PostEventGlobalMotion(const Math::Vector2i& motion)
{
    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnGlobalMotion(motion))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnGlobalMotion(motion))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set motion state */
    motion_ += motion;
}

void Mouse::ResetStates()
{
    /* Reset all button- hit, release and double-clicked states */
    for (auto& buttonState : buttonStates_)
    {
        buttonState.hit             = false;
        buttonState.released        = false;
        buttonState.doubleClicked   = false;
    }

    mouseWheel_ = 0;
    motion_ = { 0, 0 };
}

void Mouse::AddEventHandler(const EventHandlerPtr& eventHandler)
{
    if (eventHandler)
        eventHandlers_.push_back(eventHandler);
}

void Mouse::RemoveEventHandler(const EventHandlerPtr& eventHandler)
{
    RemoveAllFromList(eventHandlers_, eventHandler);
}

const Mouse::ButtonStateModel& Mouse::ButtonState(const MouseKeyCodes keyCode) const
{
    const size_t index = static_cast<size_t>(keyCode);

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    if (index >= Mouse::numButtons)
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif

    return buttonStates_[index];
}

bool Mouse::AnyButtonDown() const
{
    return NumButtonsDown() > 0;
}

bool Mouse::AnyButtonHit() const
{
    //...
    return false;
}

bool Mouse::AnyButtonReleased() const
{
    //...
    return false;
}

bool Mouse::AnyButtonDoubleClicked() const
{
    //...
    return false;
}

size_t Mouse::NumButtonsDown() const
{
    return numButtonsDown_;
}


/*
 * ======= Protected: =======
 */

bool Mouse::LockFocus(EventHandler* eventHandler)
{
    /* Check if event handler is already locked */
    if (lockedEventHandler_)
    {
        /* Check if this is the locked event handler */
        return lockedEventHandler_ == eventHandler;
    }

    /* Lock this event handler */
    lockedEventHandler_ = eventHandler;
    eventHandler->OnFocus();

    /* Tell other event handlers that they lost the focus */
    for (auto& evtRecv : eventHandlers_)
    {
        if (evtRecv.get() != eventHandler)
            evtRecv->OnLostFocus();
    }

    return true;
}

bool Mouse::UnlockFocus(EventHandler* eventHandler)
{
    if (lockedEventHandler_ == eventHandler)
    {
        lockedEventHandler_ = nullptr;
        return true;
    }
    return false;
}


} // /namespace IO

} // /namespace Fork



// ========================