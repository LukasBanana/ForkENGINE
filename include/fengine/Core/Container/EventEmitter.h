/*
 * Event emitter header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EVENT_EMITTER_H__
#define __FORK_EVENT_EMITTER_H__

#if 0 //!!!


#include "Core/STLHelper.h"

#include <memory>
#include <vector>
#include <algorithm>


namespace Fork
{


/**
Base class for all event emitters.
\tparam EventHandlerClass Specifies the event handler class.
This should be a sub-class of the event emitter.
\see IO::Mouse
\see IO::Keyboard
\see Video::TextureManager
\todo Currently unused.
*/
template <class EventHandlerClass> class EventEmitter
{
    
    public:
        
        typedef std::shared_ptr<EventHandlerClass> EventHandlerPtr;

        virtual ~EventEmitter()
        {
        }

        /**
        Adds an event handler. This can be used to discard and/or register texture loading events.
        \see EventHandler
        */
        void AddEventHandler(const EventHandlerPtr& eventHandler)
        {
            if (eventHandler)
                eventHandlers_.push_back(eventHandler);
        }
        //! Removes the specified event handler.
        void RemoveEventHandler(const EventHandlerPtr& eventHandler)
        {
            RemoveFromList(eventHandlers_, eventHandler);
        }

    protected:
        
        std::vector<EventHandlerPtr> eventHandlers_;

};


} // /namespace Fork


#endif

#endif



// ========================