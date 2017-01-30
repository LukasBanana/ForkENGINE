/*
 * GUI focus event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_FOCUS_EVENT_H__
#define __FORK_GUI_FOCUS_EVENT_H__


#include "GUI/EventSystem/Event.h"


namespace Fork
{

namespace GUI
{


//! Focus event when a GUI element get or lost its focus.
class FORK_EXPORT FocusEvent : public Event
{
    
    public:
        
        FocusEvent(ViewController* owner, const ViewController* prevFocusedElement = nullptr) :
            Event               { owner              },
            prevFocusedElement_ { prevFocusedElement }
        {
        }

        //! Returns the previously focused GUI element. This may also be null!
        inline const ViewController* GetPrevFocusedElement() const
        {
            return prevFocusedElement_;
        }

    private:
        
        const ViewController* prevFocusedElement_ = nullptr;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================