/*
 * GUI key event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_KEY_EVENT_H__
#define __FORK_GUI_KEY_EVENT_H__


#include "GUI/EventSystem/Event.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! Key event when a key was pressed or released on a GUI element.
class FORK_EXPORT KeyEvent : public Event
{
    
    public:
        
        KeyEvent(ViewController* owner, const KeyCodes keyCode) :
            Event   { owner   },
            keyCode_{ keyCode }
        {
        }

        //! Returns the key code.
        inline KeyCodes GetKeyCode() const
        {
            return keyCode_;
        }

    private:
        
        KeyCodes keyCode_;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================