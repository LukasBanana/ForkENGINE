/*
 * Network thread loop base header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_THREAD_LOOP_BASE_H__
#define __FORK_NETWORK_THREAD_LOOP_BASE_H__


#include "Core/Export.h"

#include <thread>
#include <atomic>
#include <memory>


namespace Fork
{

namespace Network
{


//! Network system/protocol threading loop base class.
class FORK_EXPORT ThreadLoopBase
{
    
    public:
        
        virtual ~ThreadLoopBase();

        ThreadLoopBase(const ThreadLoopBase&) = delete;
        ThreadLoopBase& operator = (const ThreadLoopBase&) = delete;

        inline bool HasFinished() const
        {
            return hasFinished_;
        }

    protected:
        
        ThreadLoopBase() = default;

        template <class Function, class... Args> void RunThread(Function&& func, Args&&... args)
        {
            JoinThread();
            hasFinished_ = false;
            thread_ = std::make_unique<std::thread>(func, args...);
        }

        //! Sets the 'hasFinished' flag.
        void Finish();

        //! Tells the thread to finish and waits until the thread has been terminated its execution.
        void JoinThread();

    private:
        
        std::unique_ptr<std::thread> thread_;

        mutable std::atomic<bool> hasFinished_ { false };

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================