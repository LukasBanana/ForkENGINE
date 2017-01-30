/*
 * Keyboard header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_KEYBOARD_H__
#define __FORK_IO_KEYBOARD_H__


#include "IO/InputDevice/KeyCodes.h"
#include "IO/InputDevice/KeyCodeRecord.h"
#include "Core/Export.h"

#include <functional>
#include <array>
#include <memory>
#include <vector>


namespace Fork
{

namespace Platform
{
    class Frame;
}

namespace IO
{


//! This is the keyboard (singleton) class.
class FORK_EXPORT Keyboard
{
    
    public:
        
        /* === Classes === */

        //! Event handler interface.
        class FORK_EXPORT EventHandler
        {

            public:
                
                virtual ~EventHandler();

                /**
                Returns ture if this event handler has the keyboard focus.
                \see LockFocus
                \see UnlockFocus
                \see Keyboard::GetLockedEventHandler
                */
                bool HasFocus() const;

                //! Receives the 'key down' event and returns false if the event is to be omitted.
                virtual bool OnKeyDown(const KeyCodes keyCode, const Platform::Frame* frame);
                //! Receives the 'key up' event and returns false if the event is to be omitted.
                virtual bool OnKeyUp(const KeyCodes keyCode, const Platform::Frame* frame);

                //! Receives the 'text enter' event.
                virtual void OnTextEnter(wchar_t chr, const Platform::Frame* frame);

                /**
                Locks the keyboard focus for this event handler.
                Only this event handler will receive further keyboard events.
                \return True if this event handler got keyboard focus.
                Otherwise the focus is already locked by enother event handler.
                \note Don't forget to unlock the focus!
                */
                bool LockFocus();
                /**
                Unlocks the mouse focus.
                \see LockFocus
                */
                bool UnlockFocus();

            protected:
                
                friend class Keyboard;

                EventHandler() = default;

                /**
                Receives the 'focus' event. This will be called, when this event handler locked and got the focus.
                \see LockFocus
                */
                virtual void OnFocus();
                /**
                Receives the 'lost focus' event. This will be called, when this
                event handler lost the focus, because another event handler locked the focus.
                \see LockFocus
                */
                virtual void OnLostFocus();

        };

        typedef std::shared_ptr<EventHandler> EventHandlerPtr;

        /**
        Key state data model structure.
        \see KeyState
        */
        struct FORK_EXPORT KeyStateModel
        {
            bool down       = false; //!< True if the key is pressed down.
            bool hit        = false; //!< True if the key was hit.
            bool hitRepeat  = false; //!< True if the key was hit (repeating).
            bool released   = false; //!< True if the key was released.
        };

        Keyboard(const Keyboard&) = delete;
        Keyboard& operator = (const Keyboard&) = delete;

        /* === Functions === */

        //! Returns the single keyboard instance.
        static Keyboard* Instance();

        /**
        Posts a key press down event. This will be called automatically by the platform device
        handler when a key was pressed. This also calls all event handlers.
        \param[in] keyCode Specifies the code number for the pressed key.
        \param[in] frame Optional, non-owning, constant pointer to the frame from which the key event was posted. By default null.
        \see AddEventHandler
        \see KeyCodes
        */
        void PostEventKeyDown(const KeyCodes keyCode, const Platform::Frame* frame = nullptr);
        //! \see PostEventKeyDown
        void PostEventKeyUp(const KeyCodes keyCode, const Platform::Frame* frame = nullptr);

        /**
        Posts a text enter event. This will be called automatically by the platform device handler when
        a text by the keyboard was entered. This also calls all event handlers.
        \see AddEventHandler
        */
        void PostEventTextEnter(wchar_t chr, const Platform::Frame* frame = nullptr);

        /**
        Resets the internal key states by using the recorded key codes.
        This should be called once every frame.
        */
        void ResetStates();

        /**
        Adds the specified event handler (if it's not null). This can be used to register key board events immediately.
        This callback will be called every time, when "PostEventKeyDown" or "PostEventKeyUp" is called.
        These two functions will be called automatically by the platform device handler.
        \see EventHandler
        \see PostEventKeyDown
        \see PostEventKeyUp
        \see PostEventTextEnter
        */
        void AddEventHandler(const EventHandlerPtr& eventHandler);
        //! Removes the specified event handler.
        void RemoveEventHandler(const EventHandlerPtr& eventHandler);

        /**
        Returns the current state of the specified key.
        \param[in] keyCode Specifies the key code whose state will be returned.
        \throws IndexOutOfBoundsException If 'keyCode' is out of bounds.
        \see KeyCodes
        \see KeyStateModel
        */
        const KeyStateModel& KeyState(const KeyCodes keyCode) const;

        /**
        Returns true if the specified key is pressed down.
        \see KeyCodes
        */
        inline bool KeyDown(const KeyCodes keyCode) const
        {
            return KeyState(keyCode).down;
        }
        /**
        Returns true if the specified key was hit. This state will be cleared when "ResetStates" is called.
        \see ResetStates
        \see KeyCodes
        */
        inline bool KeyHit(const KeyCodes keyCode) const
        {
            return KeyState(keyCode).hit;
        }
        /**
        Returns true if the specified key was hit (this depends on OS configuration for key hit repetion).
        This state will be cleared when "ResetStates" is called.
        \see ResetStates
        \see KeyCodes
        */
        inline bool KeyHitRepeat(const KeyCodes keyCode) const
        {
            return KeyState(keyCode).hitRepeat;
        }
        /**
        Returns true if the specified key was released. This state will be cleared when "ResetStates" is called.
        \see ResetStates
        \see KeyCodes
        */
        inline bool KeyReleased(const KeyCodes keyCode) const
        {
            return KeyState(keyCode).released;
        }

        //! Returns true if any key is pressed down.
        bool AnyKeyDown() const;
        //! Returns true if any key was hit.
        bool AnyKeyHit() const;
        //! Returns true if any key was released.
        bool AnyKeyReleased() const;

        //! Returns the number of keys currently pressed down.
        size_t NumKeysDown() const;

        /**
        Returns a constant raw-pointer to the event handler for which the keyboard focus
        is currently locked, or null if no event handler has locked the keyboard foucs.
        \see Keyboard::EventHandler::LockFocus
        */
        inline const EventHandler* GetLockedEventHandler() const
        {
            return lockedEventHandler_;
        }

    protected:

        friend class EventHandler;

        bool LockFocus(EventHandler* eventHandler);
        bool UnlockFocus(EventHandler* eventHandler);

        EventHandler* lockedEventHandler_ = nullptr;

    private:
        
        static const size_t numKeys = 152;

        /* === Make default constructors private for the singleton === */

        Keyboard() = default;

        /* === Members === */

        std::vector<EventHandlerPtr> eventHandlers_;

        KeyCodeRecord keyDownRecord_, keyUpRecord_;

        std::array<KeyStateModel, numKeys> keyStates_;
        size_t numKeysDown_ = 0;
        
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================