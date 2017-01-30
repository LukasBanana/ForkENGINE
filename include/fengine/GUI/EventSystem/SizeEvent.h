/*
 * GUI size event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_SIZE_EVENT_H__
#define __FORK_GUI_SIZE_EVENT_H__


#include "GUI/EventSystem/Event.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! Size event when a GUI element is resized.
class FORK_EXPORT SizeEvent : public Event
{
    
    public:
        
        SizeEvent(ViewController* owner, const Size& size) :
            Event   { owner },
            size    { size  }
        {
        }

        //! Specifies the size value.
        Size size;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================