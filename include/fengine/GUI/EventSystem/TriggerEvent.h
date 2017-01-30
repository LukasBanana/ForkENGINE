/*
 * GUI trigger event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_TRIGGER_EVENT_H__
#define __FORK_GUI_TRIGGER_EVENT_H__


#include "GUI/EventSystem/Event.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! Trigger event when a GUI element was triggered (e.g. a push button was pressed).
class FORK_EXPORT TriggerEvent : public Event
{
    
    public:
        
        TriggerEvent(ViewController* owner, TriggerIDType id) :
            Event   { owner },
            id_     { id    }
        {
        }

        /**
        Returns the ID number of the trigger event.
        \remarks This could be a pre-defined value such as 'Button::Triggers::Pressed' or the ID of a menu item.
        */
        inline TriggerIDType GetID() const
        {
            return id_;
        }

    private:
        
        TriggerIDType id_;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================