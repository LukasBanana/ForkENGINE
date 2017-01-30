/*
 * Network thread loop base file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/ThreadLoopBase.h"


namespace Fork
{

namespace Network
{


ThreadLoopBase::~ThreadLoopBase()
{
    JoinThread();
}


/*
 * ======= Protected: =======
 */

void ThreadLoopBase::Finish()
{
    hasFinished_ = true;
}

void ThreadLoopBase::JoinThread()
{
    if (thread_)
    {
        /* Cancel thread execution and wait until thread is finish */
        Finish();
        thread_->join();
        thread_ = nullptr;
    }
}


} // /namespace Network

} // /namespace Fork



// ========================