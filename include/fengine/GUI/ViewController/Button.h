/*
 * GUI button header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_BUTTON_H__
#define __FORK_GUI_BUTTON_H__


#include "GUI/ViewController/ViewController.h"
#include "GUI/Core/ColorScheme.h"


namespace Fork
{

namespace GUI
{


//! GUI button class.
class FORK_EXPORT Button : public ViewController
{
    
    public:
        
        //! Trigger callback for easy trigger event handling.
        typedef std::function<void (Button* owner)> TriggerCallback;

        /* === Classes === */

        //! Button flags enumeration.
        struct Flags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                Toggle      = (1 << 0), //!< The button has a toggle mode.
                SingleFrame = (1 << 1), //!< The button has a single frame. Can not be used with "DoubleFrame":
                DoubleFrame = (1 << 2), //!< The button has a double frame. Can not be used with "SingleFrame":
                PushCapture = (1 << 3), //!< Captures the button when it is pushed. This only affects the visual appearance of the button.
            };
        };

        //! Default button event handler.
        class FORK_EXPORT ViewEventHandler : public EventHandler
        {
            
            public:
                
                //! \throws NullPointerException If 'owner' is null.
                ViewEventHandler(Button* owner);

                void OnMouseDown( MouseEvent&   event ) override;
                void OnMouseUp  ( MouseEvent&   event ) override;
                void OnMouseMove( MouseEvent&   event ) override;
                void OnDraw     ( DrawEvent&    event ) override;
                void OnTrigger  ( TriggerEvent& event ) override;

            protected:
                
                //! Frame interaction state.
                enum class States
                {
                    Idle,
                    Pushed,
                };

                //! Sets the new internal state.
                void ChangeState(const States state);

                //! Returns the owner of this event handler.
                inline Button* GetOwner() const
                {
                    return owner_;
                }

                //! Returns the current internal state.
                inline States GetState() const
                {
                    return state_;
                }

                //! Returns true if the mouse is over the button.
                inline bool IsMouseOver() const
                {
                    return isMouseOver_;
                }

            private:
                
                Button* owner_          = nullptr;
                States  state_          = States::Idle;
                bool    isMouseOver_    = false;

        };

        /* === Functions === */

        Button();

        //! Changes the button flags. By default 0.
        void ChangeFlags(const Flags::DataType flags);

        //! Returns the button flags.
        inline Flags::DataType GetFlags() const
        {
            return flags_;
        }

        /**
        Sets the trigger callback procedure.
        \remarks This can be used for easy trigger event handling.
        \see ViewController::EventHandler::OnTrigger
        */
        inline void SetTriggerCallback(const TriggerCallback& triggerCallback)
        {
            triggerCallback_ = triggerCallback;
        }

        //! Color scheme for this button.
        ColorScheme::Button colorScheme;

    private:
        
        friend class ViewEventHandler;

        Rect ClientAreaBorder() const override;

        Flags::DataType flags_ = 0;

        TriggerCallback triggerCallback_;

};



} // /namespace GUI

} // /namespace Fork


#endif



// ========================