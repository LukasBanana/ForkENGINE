/*
 * Network session reception file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/System/Session/SessionReception.h"
#include "Network/Core/AddressIPv4.h"
#include "Network/Core/AddressIPv6.h"
#include "SessionCore.h"

#include "IO/Core/Log.h"//!!!


namespace Fork
{

namespace Network
{


/*
Add a consistent character (here '*') to the session key,
so that always a non-empty string will be send when a login request is made.
*/
SessionReception::SessionReception(const Address& address, const std::string& sessionDesc, const std::string& sessionKey) :
    sessionKey_ { std::string(sessionKeyPrefix) + sessionKey },
    sessionDesc_{ sessionDesc                                }
{
    /* Create UDP/IP socket */
    socket_ = std::make_unique<UDPSocket>(address.Domain());

    socket_->EnableBlocking(false);

    if (socket_->Bind(address))
    {
        /* Create login address */
        if (address.Domain() == AF_INET6)
            loginAddress_ = std::make_unique<AddressIPv6>(0);
        else
            loginAddress_ = std::make_unique<AddressIPv4>(0);

        /* Start request thread */
        RunThread(std::bind(&SessionReception::RecvRequests, this));
    }
}
SessionReception::~SessionReception()
{
    /* Join thread before the socket is implicitly destroyed */
    JoinThread();
}

void SessionReception::ChangeSessionKey(const std::string& sessionKey)
{
    std::lock_guard<std::mutex> gurad(sessionMutex_);
    sessionKey_ = std::string(sessionKeyPrefix) + sessionKey;
}

std::string SessionReception::SessionKey()
{
    std::lock_guard<std::mutex> gurad(sessionMutex_);
    return sessionKey_;
}

void SessionReception::ChangeSessionDesc(const std::string& sessionDesc)
{
    std::lock_guard<std::mutex> gurad(sessionMutex_);
    sessionDesc_ = sessionDesc;
}

std::string SessionReception::SessionDesc()
{
    std::lock_guard<std::mutex> gurad(sessionMutex_);
    return sessionDesc_;
}


/*
 * ======= Private: =======
 */

void SessionReception::RecvRequests()
{
    while (!HasFinished())
    {
        /* Receive next request */
        auto message = socket_->RecvString(*loginAddress_);

        #if 0//!DEBUGGING!
        if (!message.empty())
            IO::Log::Debug("SessionReception received message: \"" + message + "\"");
        #endif

        /* Compare session key */
        if (!message.empty() && message == SessionKey())
        {
            /* Answer the login request */
            socket_->SendString(SessionDesc(), *loginAddress_);
        }

        /* Wait a moment (100ms) to give the other threads time to run */
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


} // /namespace Network

} // /namespace Fork



// ========================