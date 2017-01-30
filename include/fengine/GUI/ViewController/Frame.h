/*
 * GUI frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_FRAME_H__
#define __FORK_GUI_FRAME_H__


#include "GUI/ViewController/ScrollArea.h"
#include "GUI/Core/ColorScheme.h"


namespace Fork
{

namespace GUI
{


//! GUI frame class (also called "Window").
class FORK_EXPORT Frame : public ViewController
{
    
    public:
        
        /* === Classes === */

        //! Frame flags enumeration.
        struct Flags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                TitleBar        = (1 << 0), //!< The frame has a title bar.
                CloseBox        = (1 << 1), //!< The frame has a close box in the title bar.
                MinimizeBox     = (1 << 2), //!< The frame has a minimization box in the title bar.
                MaxmimizeBox    = (1 << 3), //!< The frame has a maximization box in the title bar.
                SizeBox         = (1 << 4), //!< The frame has a size box at the right bottom.
                Borderless      = (1 << 5), //!< The frame is borderless. This can not be used with "CloseBox", "MinimizeBox" or "MaximizeBox".
                ScrollArea      = (1 << 6), //!< The frame has a scroll area.
            };
        };

        //! Frame configuration structure.
        struct Configuration
        {
            int captionSize     = 30; //!< Frame caption (or rather title bar) size. By default 30.
            int statusBarSize   = 20; //!< Frame status bar size. By default 20.
        };

        //! Default frame event handler.
        class FORK_EXPORT ViewEventHandler : public EventHandler
        {
            
            public:
                
                //! \throws NullPointerException If 'owner' is null.
                ViewEventHandler(Frame* owner);

                void OnMouseDown( MouseEvent&   event ) override;
                void OnMouseUp  ( MouseEvent&   event ) override;
                void OnMouseMove( MouseEvent&   event ) override;
                void OnDraw     ( DrawEvent&    event ) override;

            protected:
                
                //! Frame interaction state.
                enum class States
                {
                    Idle,
                    MoveFrame,
                    ResizeFrame,
                };

                //! Sets the new internal state.
                void ChangeState(const States state);

                //! Returns the owner of this event handler.
                inline Frame* GetOwner() const
                {
                    return owner_;
                }

                //! Returns the current internal state.
                inline States GetState() const
                {
                    return state_;
                }

            private:
                
                Frame* owner_ = nullptr;
                States state_ = States::Idle;

        };

        /* === Functions === */

        Frame();

        //! Adjusts the scroll area.
        void Resize(Size size) override;

        //! Changes the frame flags. By default 0.
        void ChangeFlags(const Flags::DataType flags);

        /**
        Sets the virtual area of this frame.
        \see ScrollArea::SetupVirtualSize
        */
        void SetupVirtualSize(const Size& size);

        //! Returns the current color scheme depending on the frame's state for drawing the title bar.
        const ColorScheme::Box& TileBarColorScheme() const;

        //! Returns the frame flags.
        inline Flags::DataType GetFlags() const
        {
            return flags_;
        }

        //! Returns the frame configuration.
        inline const Configuration& GetConfig() const
        {
            return config_;
        }

        //! Color scheme for this frame.
        ColorScheme::Frame colorScheme;

    private:
        
        Rect ClientAreaBorder() const override;

        void UpdateScrollArea();

        /* === Members === */

        Flags::DataType flags_ = 0;
        Configuration   config_;
        ScrollArea      scrollArea_;

};



} // /namespace GUI

} // /namespace Fork


#endif



// ========================