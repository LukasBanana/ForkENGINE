/*
 * Keyboard file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/InputDevice/Keyboard.h"
#include "Core/StaticConfig.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Core/STLHelper.h"

#include <algorithm>


namespace Fork
{

namespace IO
{


/*
 * Keyboard::EventHandler class
 */

Keyboard::EventHandler::~EventHandler()
{
}

bool Keyboard::EventHandler::HasFocus() const
{
    return Keyboard::Instance()->GetLockedEventHandler() == this;
}

bool Keyboard::EventHandler::OnKeyDown(const KeyCodes keyCode, const Platform::Frame* frame)
{
    return true;
}

bool Keyboard::EventHandler::OnKeyUp(const KeyCodes keyCode, const Platform::Frame* frame)
{
    return true;
}

void Keyboard::EventHandler::OnTextEnter(wchar_t chr, const Platform::Frame* frame)
{
    /* Dummy */
}

void Keyboard::EventHandler::OnFocus()
{
    /* Dummy */
}

void Keyboard::EventHandler::OnLostFocus()
{
    /* Dummy */
}

bool Keyboard::EventHandler::LockFocus()
{
    return Keyboard::Instance()->LockFocus(this);
}

bool Keyboard::EventHandler::UnlockFocus()
{
    return Keyboard::Instance()->UnlockFocus(this);
}


/*
 * Keyboard class
 */

Keyboard* Keyboard::Instance()
{
    static Keyboard instance;
    return &instance;
}

void Keyboard::PostEventKeyDown(const KeyCodes keyCode, const Platform::Frame* frame)
{
    /* Get index from key code */
    const size_t index = static_cast<size_t>(keyCode);

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    if (index >= Keyboard::numKeys)
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif

    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnKeyDown(keyCode, frame))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnKeyDown(keyCode, frame))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set key-hit state */
    auto& keyState = keyStates_[index];

    keyState.hit        = !keyState.down;
    keyState.hitRepeat  = true;
    keyState.down       = true;

    /* Record key event for faster 'key-hit' reset */
    keyDownRecord_.Add(index);

    ++numKeysDown_;
}

void Keyboard::PostEventKeyUp(const KeyCodes keyCode, const Platform::Frame* frame)
{
    /* Get index from key code */
    const size_t index = static_cast<size_t>(keyCode);

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    if (index >= Keyboard::numKeys)
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif

    /* Call event handlers */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
        {
            if (!lockedEventHandler_->OnKeyUp(keyCode, frame))
                return;
        }
        else
        {
            bool omit = false;

            for (const auto& evtRecv : eventHandlers_)
            {
                if (!evtRecv->OnKeyUp(keyCode, frame))
                    omit = true;
            }

            if (omit)
                return;
        }
    }

    /* Set key-released state */
    auto& keyState = keyStates_[index];

    keyState.released   = true;
    keyState.down       = false;

    /* Record key event for faster 'key-released' reset */
    keyUpRecord_.Add(index);

    if (numKeysDown_ > 0)
        --numKeysDown_;
    /*else
        < Warning?! >*/
}

void Keyboard::PostEventTextEnter(wchar_t chr, const Platform::Frame* frame)
{
    /* Only call event handlers here */
    if (!eventHandlers_.empty())
    {
        if (lockedEventHandler_)
            lockedEventHandler_->OnTextEnter(chr, frame);
        else
        {
            for (const auto& evtRecv : eventHandlers_)
                evtRecv->OnTextEnter(chr, frame);
        }
    }
}

void Keyboard::ResetStates()
{
    /* Reset all key- hit and release states */
    for (size_t i = 0; i < keyDownRecord_.num; ++i)
    {
        auto& keyState = keyStates_[keyDownRecord_.records[i]];

        keyState.hit        = false;
        keyState.hitRepeat  = false;
    }
    keyDownRecord_.Clear();

    for (size_t i = 0; i < keyUpRecord_.num; ++i)
        keyStates_[keyUpRecord_.records[i]].released = false;
    keyUpRecord_.Clear();
}

void Keyboard::AddEventHandler(const EventHandlerPtr& eventHandler)
{
    if (eventHandler)
        eventHandlers_.push_back(eventHandler);
}

void Keyboard::RemoveEventHandler(const EventHandlerPtr& eventHandler)
{
    RemoveAllFromList(eventHandlers_, eventHandler);
}

const Keyboard::KeyStateModel& Keyboard::KeyState(const KeyCodes keyCode) const
{
    const size_t index = static_cast<size_t>(keyCode);

    #ifdef FORK_ENABLE_ALL_EXCEPTIONS
    if (index >= Keyboard::numKeys)
        throw IndexOutOfBoundsException(__FUNCTION__, index);
    #endif

    return keyStates_[index];
}

bool Keyboard::AnyKeyDown() const
{
    return NumKeysDown() > 0;
}

bool Keyboard::AnyKeyHit() const
{
    return keyDownRecord_.num > 0;
}

bool Keyboard::AnyKeyReleased() const
{
    return keyUpRecord_.num > 0;
}

size_t Keyboard::NumKeysDown() const
{
    return numKeysDown_;
}


/*
 * ======= Protected: =======
 */

bool Keyboard::LockFocus(EventHandler* eventHandler)
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

bool Keyboard::UnlockFocus(EventHandler* eventHandler)
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