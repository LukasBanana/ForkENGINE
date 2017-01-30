/*
 * GUI scroll area header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_SCROLL_AREA_H__
#define __FORK_GUI_SCROLL_AREA_H__


#include "GUI/ViewController/ScrollBar.h"


namespace Fork
{

namespace GUI
{


//! GUI scroll area class.
class FORK_EXPORT ScrollArea : public ViewController
{
    
    public:
        
        /* === Classes === */

        //! Scroll area flags enumeration.
        struct Flags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                VertLeft    = (1 << 0), //!< The vertical scroll bar is on the left (otherwise right).
                HorzTop     = (1 << 1), //!< The horizontal scroll bar is on the top (otherwise bottom).
            };
        };

        //! Event handler for the frame's scroll bars.
        class FORK_EXPORT ScrollBarEventHandler : public ScrollBar::ViewEventHandler
        {
            
            public:
                
                //! \throws NullPointerException If 'owner' or 'virtualArea' is null.
                ScrollBarEventHandler(ScrollBar* owner, ViewController* virtualArea);

                void OnResize(SizeEvent& event) override;

            protected:
                
                void Scroll(int position) override;

                //! Returns the virtual area view controller.
                inline ViewController* GetVirtualArea() const
                {
                    return virtualArea_;
                }

            private:
                
                ViewController* virtualArea_ = nullptr;

        };

        /* === Functions === */

        ScrollArea();

        //! Adjusts the child scroll bars.
        void Resize(Size size) override;

        //! Changes the frame flags. By default 0.
        void ChangeFlags(const Flags::DataType flags);

        /**
        Sets the virtual area of this scroll area.
        This will interact with the vertical and horizontal scroll bars.
        */
        void SetupVirtualSize(const Size& size);

        //! Returns the frame flags.
        inline Flags::DataType GetFlags() const
        {
            return flags_;
        }

        //! Specifies the virtual area view controller.
        ViewController virtualArea;

    private:
        
        void UpdateScrollBars();

        /* === Members === */

        Flags::DataType flags_ = 0;

        ScrollBar       vertScrollBar_;
        ScrollBar       horzScrollBar_;

};



} // /namespace GUI

} // /namespace Fork


#endif



// ========================