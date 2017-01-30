/*
 * Observable header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_OBSERVABLE_H__
#define __FORK_OBSERVABLE_H__


#include <set>


namespace Fork
{


/**
Base class for all observable objects.
\tparam ObserverClass Specifies the observer class type.
\tparam NotifyArgs Specifies the argument list of the notification function.
This can be any class. There are no required functions for this class type.
\remarks When you extend this class, only a single function
must be implemented, which is "NotifyObserver".
\see NotifyObserver
*/
template <class ObserverClass, typename... NotifyArgs> class Observable
{
    
    public:
        
        virtual ~Observable()
        {
        }

        /**
        Calls the notify function for all observers.
        \remarks This is used to notify all observers about any changes in this observable object.
        \see ComponentObserver::Notify
        \see AttachObserver
        */
        void NotifyAllObservers(NotifyArgs... args) const
        {
            for (auto it : observers_)
                NotifyObserver(*it, args...);
        }

        //! Attaches the specified observer.
        void AttachObserver(ObserverClass& observer)
        {
            observers_.insert(&observer);
        }
        //! Detaches the specified observer.
        void DetachObserver(ObserverClass& observer)
        {
            observers_.erase(&observer);
        }

    protected:
        
        typedef std::set<ObserverClass*> ObserverList;

        //! This function must notify the specified observer.
        virtual void NotifyObserver(ObserverClass& observer, NotifyArgs... args) const = 0;

        //! Returns the observers hash-map.
        inline const ObserverList& GetObservers() const
        {
            return observers_;
        }

    private:
        
        ObserverList observers_;

};


} // /namespace Fork


#endif



// ========================