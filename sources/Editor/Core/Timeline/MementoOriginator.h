/*
 * Editor memento originator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_HISTORY_MEMENTO_ORIGINATOR_H__
#define __FORK_EDITOR_HISTORY_MEMENTO_ORIGINATOR_H__


#include "Memento.h"


namespace Fork
{

namespace Editor
{

namespace History
{


//! Memento originator interface.
class MementoOriginator
{
    
    public:
        
        MementoOriginator() = default;

        virtual ~MementoOriginator()
        {
        }

        //! Stores the current originator state to a new memento instance.
        virtual MementoPtr StoreToMemento() const = 0;
        /**
        Restores a previous originator state from the specified memento.
        \throws NullPointerException If 'memento' is null.
        \throws PointerConversionException If 'memento' is not the correct memento class for the respective originator.
        */
        virtual void RestoreFromMemento(const Memento* memento) = 0;

};


} // /namespace History

} // /namespace Editor

} // /namespace Fork


#endif



// ========================