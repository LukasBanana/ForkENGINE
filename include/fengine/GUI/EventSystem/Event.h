/*
 * GUI event header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_EVENT_H__
#define __FORK_GUI_EVENT_H__


#include "Core/Export.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace GUI
{


class ViewController;

//! Base class for all GUI events.
class FORK_EXPORT Event
{
    
    public:
        
        //! \throws NullPointerException If 'owner' is null.
        Event(ViewController* owner) :
            owner_{ owner }
        {
            ASSERT_POINTER(owner);
        }
        virtual ~Event()
        {
        }

        //! Returns the owner of this event.
        inline ViewController* GetOwner() const
        {
            return owner_;
        }

        //! Sends this event a veto, i.e. the event will be ignore.
        inline void Veto()
        {
            isVeto_ = true;
        }
        //! Returns true if this event has received a veto.
        inline bool IsVeto() const
        {
            return isVeto_;
        }

    private:
        
        ViewController* owner_  = nullptr;
        bool            isVeto_ = false;

};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================