/*
 * GUI mouse event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_MOUSE_EVENT_H__
#define __FORK_GUI_MOUSE_EVENT_H__


#include "GUI/EventSystem/Event.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! Mouse event when a GUI element is resized.
class FORK_EXPORT MouseEvent : public Event
{
    
    public:
        
        MouseEvent(ViewController* owner, const Point& globalPosition, const Point& localPosition, const Point& motion) :
            Event           { owner          },
            globalPosition_ { globalPosition },
            localPosition_  { localPosition  },
            motion_         { motion         }
        {
        }
        MouseEvent(ViewController* owner, const Point& globalPosition, const Point& localPosition, const MouseKeyCodes keyCode) :
            Event           { owner          },
            globalPosition_ { globalPosition },
            localPosition_  { localPosition  },
            keyCode_        { keyCode        }
        {
        }
        MouseEvent(ViewController* owner, const Point& globalPosition, const Point& localPosition, int wheelMotion) :
            Event           { owner          },
            globalPosition_ { globalPosition },
            localPosition_  { localPosition  },
            wheelMotion_    { wheelMotion    }
        {
        }

        /**
        Returns the key code.
        The default key value is 'MouseKeyCodes::MouseLeft', when no mouse key event was send.
        \see MouseKeyCodes
        */
        inline MouseKeyCodes GetKeyCode() const
        {
            return keyCode_;
        }

        //! Returns the global mouse position.
        inline const Point& GetGlobalPosition() const
        {
            return globalPosition_;
        }

        //! Returns the local mouse position relative to the event owner.
        inline const Point& GetLocalPosition() const
        {
            return localPosition_;
        }

        /**
        Returns the local mouse motion. This is also called "delta position",
        because it's something like the derivative of the position.
        */
        inline const Point& GetMotion() const
        {
            return motion_;
        }

        //! Returns the mouse wheel motion.
        inline int GetWheelMotion() const
        {
            return wheelMotion_;
        }

    private:
        
        Point           globalPosition_;
        Point           localPosition_;
        Point           motion_;
        int             wheelMotion_    = 0;
        MouseKeyCodes   keyCode_        = MouseKeyCodes::MouseLeft;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================