/*
 * Mouse header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_MOUSE_H__
#define __FORK_IO_MOUSE_H__


#include "IO/InputDevice/MouseKeyCodes.h"
#include "Math/Core/Vector2.h"
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


/**
This is the mouse (singleton) class. It only represents a single mouse model.
It can be used to receive mouse button, mouse wheel or motion events.
To read or write global cursor settings, use the "Cursor" namespace,
e.g. to show or hide the cursor from your OS or to read or write the cursor position.
*/
class FORK_EXPORT Mouse
{
    
    public:
        
        /* === Classes === */

        //! Frame mouse position structure.
        struct FrameMousePosition
        {
            Math::Point2i           position;           //!< Local mouse position.
            const Platform::Frame*  frame = nullptr;    //!< Current frame where the mouse event occured.
        };

        //! Mouse event handler interface.
        class FORK_EXPORT EventHandler
        {

            public:
                
                virtual ~EventHandler();

                /**
                Returns ture if this event handler has the mouse focus.
                \see LockFocus
                \see UnlockFocus
                \see Mouse::GetLockedEventHandler
                */
                bool HasFocus() const;

                //! Receives the 'button down' event and returns false if the event is to be omitted.
                virtual bool OnButtonDown(const MouseKeyCodes keyCode, const Platform::Frame* frame);
                //! Receives the 'button up' event and returns false if the event is to be omitted.
                virtual bool OnButtonUp(const MouseKeyCodes keyCode, const Platform::Frame* frame);
                //! Receives the 'button double clicked' event and returns false if the event is to be omitted.
                virtual bool OnButtonDoubleClicked(const MouseKeyCodes keyCode, const Platform::Frame* frame);

                //! Receives the 'wheel motion' event and returns false if the event is to be omitted.
                virtual bool OnWheelMotion(int motion, const Platform::Frame* frame);

                /**
                Receives the 'local mouse motion' event. This only occurs when the mouse is moved
                over the specified frame. This depends on the screen (or rather frame-) resolution.
                If you want to receive high-resolution mouse motions, use "OnGlobalMotion".
                \param[in] position Specifies the local mouse position relative to the specified frame.
                \param[in] frame Constant raw-pointer to the frame where the event occured.
                \see GetFrameMousePosition
                */
                virtual bool OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame);

                /**
                Receives the 'global mouse motion' event and returns false if the event is to be omitted.
                This event contains high-resolution mouse motions. The motion is also detected, when the cursor
                is at the screens border. But one disadvantage is, that the information does not dependt on any
                mouse settings, done in the OS. For those information, use "OnLocalMotion".
                \param[in] motion Specifies the global mouse motion direction.
                */
                virtual bool OnGlobalMotion(const Math::Vector2i& motion);

                /**
                Locks the mouse focus for this event handler.
                Only this event handler will receive further mouse events.
                This could be done, when a mouse button is pressed down and
                unlocked, when a mouse button is released.
                \return True if this event handler got mouse focus.
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
                
                friend class Mouse;

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
        Mouse button state data model structure.
        \see ButtonState
        */
        struct FORK_EXPORT ButtonStateModel
        {
            bool down           = false; //!< True if the mouse button is pressed down.
            bool hit            = false; //!< True if the mouse button was hit.
            bool released       = false; //!< True if the mouse button was released.
            bool doubleClicked  = false; //!< True if the mouse button was double clicked.
        };

        /* === Functions === */

        Mouse(const Mouse&) = delete;
        Mouse& operator = (const Mouse&) = delete;

        //! Returns the single mouse instance.
        static Mouse* Instance();

        /**
        Posts a mouse button press down event. This will be called automatically by the platform device
        handler when a mouse button was pressed. This also calls the event handler (if used).
        \param[in] keyCode Specifies the code number for the pressed mouse button.
        \param[in] frame Optional, non-owning, constant pointer to the frame from which the mouse event was posted. By default null.
        \see AddEventHandler
        \see KeyCodes
        */
        void PostEventButtonDown(const MouseKeyCodes keyCode, const Platform::Frame* frame = nullptr);
        //! \see PostEventButtonDown
        void PostEventButtonUp(const MouseKeyCodes keyCode, const Platform::Frame* frame = nullptr);
        //! \see PostEventButtonDown
        void PostEventButtonDoubleClicked(const MouseKeyCodes keyCode, const Platform::Frame* frame = nullptr);

        /**
        Posts a mouse wheel event. This will be called automatically by the platform device handler when
        a mouse wheel was moved. This also calls the event handler (if used).
        \see AddEventHandler
        */
        void PostEventWheelMotion(int motion, const Platform::Frame* frame = nullptr);

        /**
        Posts a local mouse motion event.
        \see AddEventHandler
        \see EventHandler::OnLocalMotion
        */
        void PostEventLocalMotion(const Math::Point2i& position, const Platform::Frame* frame = nullptr);

        /**
        Posts a global mouse motion event.
        \see AddEventHandler
        \see EventHandler::OnGlobalMotion
        */
        void PostEventGlobalMotion(const Math::Vector2i& motion);

        /**
        Resets the internal mouse button states.
        This should be called once every frame.
        */
        void ResetStates();

        /**
        Adds an event handler. This can be used to register key board events immediately.
        This callback will be called every time, when "PostEventButtonDown", "PostEventButtonUp" or "PostEventButtonDoubleClicked" is called.
        These two functions will be called automatically by the platform device handler.
        \see EventHandler
        \see PostEventButtonDown
        */
        void AddEventHandler(const EventHandlerPtr& eventHandler);
        //! Removes the specified event handler.
        void RemoveEventHandler(const EventHandlerPtr& eventHandler);

        /**
        Returns the current state of the specified mouse button.
        \param[in] keyCode Specifies the mouse key code whose state will be returned.
        \throws IndexOutOfBoundsException If 'keyCode' is out of bounds.
        \see MouseKeyCodes
        \see ButtonStateModel
        */
        const ButtonStateModel& ButtonState(const MouseKeyCodes keyCode) const;

        /**
        Returns true if the specified mouse button is pressed down.
        \see MouseKeyCodes
        */
        inline bool ButtonDown(const MouseKeyCodes keyCode) const
        {
            return ButtonState(keyCode).down;
        }
        /**
        Returns true if the specified mouse button was hit. This state will be cleared when "ResetStates" is called.
        \see ResetStates
        \see MouseKeyCodes
        */
        inline bool ButtonHit(const MouseKeyCodes keyCode) const
        {
            return ButtonState(keyCode).hit;
        }
        /**
        Returns true if the specified mouse button was released. This state will be cleared when "ResetStates" is called.
        \see ResetStates
        \see MouseKeyCodes
        */
        inline bool ButtonReleased(const MouseKeyCodes keyCode) const
        {
            return ButtonState(keyCode).released;
        }
        /**
        Returns true if the specified mouse button was double clicked. This state will be cleared when "ResetStates" is called.
        \note Only supported under MS/Windows.
        \see ResetStates
        \see MouseKeyCodes
        */
        inline bool ButtonDoubleClicked(const MouseKeyCodes keyCode) const
        {
            return ButtonState(keyCode).doubleClicked;
        }

        //! Returns true if any mouse button is pressed down.
        bool AnyButtonDown() const;
        //! Returns true if any mouse button was hit. \todo Not yet implemented.
        bool AnyButtonHit() const;
        //! Returns true if any mouse button was released. \todo Not yet implemented.
        bool AnyButtonReleased() const;
        //! Returns true if any mouse button was double clicked. \todo Not yet implemented.
        bool AnyButtonDoubleClicked() const;

        //! Returns the number of mouse buttons currently pressed down.
        size_t NumButtonsDown() const;

        //! Returns the mouse wheel motion. This state will be cleared when "ResetStates" is called.
        inline int GetWheelMotion() const
        {
            return mouseWheel_;
        }

        //! Returns the mouse motion. This state will be cleared when "ResetStates" is called.
        inline const Math::Vector2i& GetMotion() const
        {
            return motion_;
        }

        //! Returns the local mouse position.
        inline const FrameMousePosition& GetFrameMousePosition() const
        {
            return frameMousePos_;
        }

        /**
        Returns a constant raw-pointer to the event handler for which the mouse focus
        is currently locked, or null if no event handler has locked the mouse foucs.
        \see Mouse::EventHandler::LockFocus
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
        
        static const size_t numButtons = 5;

        /* === Make default constructors private for the singleton === */

        Mouse() = default;

        /* === Members === */

        std::vector<EventHandlerPtr> eventHandlers_;

        std::array<ButtonStateModel, numButtons> buttonStates_;
        size_t numButtonsDown_ = 0;

        Math::Vector2i motion_;
        FrameMousePosition frameMousePos_;

        int mouseWheel_ = 0;
        
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================