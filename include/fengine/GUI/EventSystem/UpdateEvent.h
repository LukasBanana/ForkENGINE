/*
 * GUI udpate event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_UPDATE_EVENT_H__
#define __FORK_GUI_UPDATE_EVENT_H__


#include "GUI/EventSystem/Event.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! Update event when a GUI element must be updated.
class FORK_EXPORT UpdateEvent : public Event
{
    
    public:
        
        UpdateEvent(ViewController* owner) :
            Event{ owner }
        {
        }

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================