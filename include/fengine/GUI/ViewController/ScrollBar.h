/*
 * GUI scroll bar header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_SCROLL_BAR_H__
#define __FORK_GUI_SCROLL_BAR_H__


#include "GUI/ViewController/ViewController.h"
#include "GUI/Core/ColorScheme.h"
#include "Utility/ScrollBarModel.h"


namespace Fork
{

namespace GUI
{


//! GUI scroll bar class.
class FORK_EXPORT ScrollBar : public ViewController
{
    
    public:
        
        /* === Classes === */

        //! Scroll bar flags enumeration.
        struct Flags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                SingleStepButtons   = (1 << 0), //!< The scroll bar has the two single-step buttons (for up/down or left/right).
                HideOnFullSize      = (1 << 1), //!< Hides the scroll bar when the virtual size has its full size.
            };
        };

        //! Default scroll bar event handler.
        class FORK_EXPORT ViewEventHandler : public EventHandler
        {
            
            public:
                
                //! \throws NullPointerException If 'owner' is null.
                ViewEventHandler(ScrollBar* owner);

                void OnMouseDown    ( MouseEvent&   event ) override;
                void OnMouseUp      ( MouseEvent&   event ) override;
                void OnMouseMove    ( MouseEvent&   event ) override;
                void OnWheelMotion  ( MouseEvent&   event ) override;
                void OnUpdate       ( UpdateEvent&  event ) override;
                void OnDraw         ( DrawEvent&    event ) override;

            protected:
                
                //! Frame interaction state.
                enum class States
                {
                    Idle,
                    Forwards,
                    Backwards,
                    Drag,
                };

                struct ButtonState
                {
                    bool isMouseOver    = false;
                    bool isPushed       = false;
                };

                //! Sets the new internal state.
                void ChangeState(const States state);

                //! Returns the button color for the specified button state.
                const ColorScheme::Box& ButtonColor(const ColorScheme::Button& buttonColors, const ButtonState& buttonState) const;

                /**
                This is called when the scroll bar is moved.
                \param[in] position Specifies the local position of the scroll bar.
                */
                virtual void Scroll(int position);

                //! Returns the owner of this event handler.
                inline ScrollBar* GetOwner() const
                {
                    return owner_;
                }

                //! Returns the current internal state.
                inline States GetState() const
                {
                    return state_;
                }

                //! Returns the state of the forward button.
                inline const ButtonState& GetForwardButtonState() const
                {
                    return forwardButtonState_;
                }
                //! Returns the state of the backward button.
                inline const ButtonState& GetBackwardButtonState() const
                {
                    return backwardButtonState_;
                }
                //! Returns the state of the drag button.
                inline const ButtonState& GetDragButtonState() const
                {
                    return dragButtonState_;
                }

            private:
                
                ScrollBar*  owner_      = nullptr;
                States      state_      = States::Idle;

                ButtonState forwardButtonState_;
                ButtonState backwardButtonState_;
                ButtonState dragButtonState_;

                int         dragOffset_ = 0;

        };

        /* === Functions === */

        ScrollBar(bool isVertical);

        //! Adjusts the child buttons.
        void Resize(Size size) override;

        //! Changes the scroll bar flags. By default 0.
        void ChangeFlags(const Flags::DataType flags);

        /**
        Returns the local scroll bar position. If the scroll bar is at the beginning, the position is 0.
        X if the scroll bar is horizontal or Y if the scroll bar is vertical.
        */
        int ScrollPosition() const;
        /**
        Sets the new scroll bar position.
        \see ScrollBarPosition
        */
        void SetupScrollPosition(int position);

        /**
        Returns the local scroll bar size.
        Width if the scroll bar is horizontal or height if the scroll bar is vertical.
        */
        int ScrollBarSize() const;

        /**
        Similar to the "ScrollPosition" function but returns the scroll position of the virutal area.
        \see ScrollPosition
        */
        int VirtualScrollPosition() const;

        //! Returns the global rectangle of the forward button.
        Rect ForwardButtonRect(const Rect& globalArea) const;
        //! Returns the global rectangle of the backward button.
        Rect BackwardButtonRect(const Rect& globalArea) const;
        //! Returns the global rectangle of the drag button.
        Rect DragButtonRect(const Rect& globalArea) const;

        //! Returns the button flags.
        inline Flags::DataType GetFlags() const
        {
            return flags_;
        }

        //! Returns true if this is a vertical scroll bar.
        inline bool IsVertical() const
        {
            return isVertical_;
        }

        //! Color scheme for this scroll bar.
        ColorScheme::ScrollBar colorScheme;

        //!TODO! -> Change this to private inheritance instead of member variable!!!
        //! Scroll bar data model. Use this to change the scroll position.
        Utility::ScrollBarModel model;

        //! Specifies the scroll speed when the up/left or down/right button is pushed. By default 3.
        int scrollSpeed = 3;

        //! Specifies the scroll speed when the mouse wheel is used. By default 30.
        int scrollWheelSpeed = 30;

    private:
        
        friend class ViewEventHandler;

        void UpdateBarSize();

        Flags::DataType flags_ = 0;

        bool            isVertical_ = false;

};



} // /namespace GUI

} // /namespace Fork


#endif



// ========================