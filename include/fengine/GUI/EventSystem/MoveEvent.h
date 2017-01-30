/*
 * GUI move event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_MOVE_EVENT_H__
#define __FORK_GUI_MOVE_EVENT_H__


#include "GUI/EventSystem/Event.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! Move event when a GUI element is moved.
class FORK_EXPORT MoveEvent : public Event
{
    
    public:
        
        MoveEvent(ViewController* owner, const Point& position) :
            Event   { owner    },
            position{ position }
        {
        }

        //! Specifies the position value.
        Point position;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================