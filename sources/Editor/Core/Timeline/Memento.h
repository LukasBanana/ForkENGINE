/*
 * Editor memento header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_HISTORY_MEMENTO_H__
#define __FORK_EDITOR_HISTORY_MEMENTO_H__


#include "Core/DeclPtr.h"
#include "../Component/Component.h"


namespace Fork
{

namespace Editor
{

namespace History
{


DECL_SHR_PTR(Memento);

//! Memento interface.
class Memento// : public IO::VirtualFile
{
    
    public:
        
        //! Value if limit is to be disabled.
        static const unsigned int unlimited = 0;
        //! Default memento limit.
        static const unsigned int defaultLimit = 100;

        Memento(const Component::Types type) :
            type_(type)
        {
        }

        Memento(const Memento&) = delete;
        Memento& operator = (const Memento&) = delete;

        //! Returns the memento (or rather component) type.
        inline Component::Types GetType() const
        {
            return type_;
        }

    private:
        
        Component::Types type_ = Component::Types::__Unknown__;

};


} // /namespace History

} // /namespace Editor

} // /namespace Fork


#endif



// ========================