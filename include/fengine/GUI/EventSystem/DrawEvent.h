/*
 * GUI draw event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_DRAW_EVENT_H__
#define __FORK_GUI_DRAW_EVENT_H__


#include "GUI/EventSystem/Event.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


class DrawContext;

//! Draw event when a GUI element must be redrawn.
class FORK_EXPORT DrawEvent : public Event
{
    
    public:
        
        //! \throws NullPointerException If 'owner' or 'context' is null.
        DrawEvent(ViewController* owner, DrawContext* context, const Rect& clientArea) :
            Event       { owner      },
            clientArea_ { clientArea },
            context_    { context    }
        {
            ASSERT_POINTER(context);
        }

        //! Returns the global client area of the owner for drawing operations.
        inline const Rect& GetClientArea() const
        {
            return clientArea_;
        }

        //! Returns the draw context.
        inline DrawContext* GetContext() const
        {
            return context_;
        }

    private:
        
        Rect            clientArea_;
        DrawContext*    context_ = nullptr;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================