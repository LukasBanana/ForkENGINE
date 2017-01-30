/*
 * Editor memento care taker header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_HISTORY_MEMENTO_CARETAKER_H__
#define __FORK_EDITOR_HISTORY_MEMENTO_CARETAKER_H__


#include "Memento.h"

#include <vector>


namespace Fork
{

namespace Editor
{

namespace History
{


#if 0//!TODO!

//! Memento care taker interface.
class MementoCareTaker
{
    
    public:
        
        MementoCareTaker() = default;

        virtual ~MementoCareTaker()
        {
        }

        /**
        Adds the specified memento and sets the iterator to the end of the list.
        \param[in] memento Shared pointer to the memento which is to be added to the list.
        \param[in] limit Specifies the memento list limit. If this is 'Memento::unlimited',
        no limit will be set (except the limit of 'std::vector'). By default Memento::defaultLimit.
        */
        void AddMemento(const MementoPtr& memento, unsigned int limit = Memento::defaultLimit);

        /**
        Restores the memento 
        */
        void Restore(int iteratorDirection);

        //! Discards all mementos after the current iterator.
        void DiscardFromIterator();

    private:
        
        std::vector<MementoPtr>             mementos_;  //!< Memento list.
        std::vector<MementoPtr>::iterator   mementoIt_; //!< Iterator which points to the current used memento.

};

#endif


} // /namespace History

} // /namespace Editor

} // /namespace Fork


#endif



// ========================