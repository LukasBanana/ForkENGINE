/*
 * Network session reception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_SESSION_RECEPTION_H__
#define __FORK_NETWORK_SESSION_RECEPTION_H__


#include "Core/Export.h"
#include "Network/Core/UDPSocket.h"
#include "Network/Core/Address.h"
#include "Network/Core/ThreadLoopBase.h"

#include <mutex>


namespace Fork
{

namespace Network
{


/**
Network session recpetion. This can be used within a server application.
This reception runs a thread, which waits for clients who want to search the network for open game sessions.
\ingroup network
*/
class FORK_EXPORT SessionReception : private ThreadLoopBase
{
    
    public:
        
        /**
        Session reception constrcutor.
        \param[in] address Specifies the reception address. Since a session reception makes use of broadcasting -
        which is only available for UDP sockets, only the port number is used from this address.
        \param[in] sessionDesc Specifies the session description. This could be the current game level name.
        \param[in] sessionKey Specifies the session key. If this is not empty, each session login must pass the same session key.
        */
        SessionReception(const Address& address, const std::string& sessionDesc, const std::string& sessionKey = "");
        ~SessionReception();

        /**
        Sets the new session key. This will append the session-key-prefix (which is an '*' character).
        \see SessionKey
        */
        void ChangeSessionKey(const std::string& sessionKey);
        /**
        Returns the session key including the session-key-prefix (which is an '*' character).
        \note This function uses a mutex. So don't call this every frame to increase performance.
        */
        std::string SessionKey();

        /**
        Sets the new session description.
        \see SessionDesc
        */
        void ChangeSessionDesc(const std::string& sessionDesc);
        /**
        Returns the session description.
        \note This function uses a mutex. So don't call this every frame to increase performance.
        */
        std::string SessionDesc();

    private:
        
        void RecvRequests();

        std::unique_ptr<UDPSocket> socket_;
        std::unique_ptr<Address> loginAddress_;

        // <thread shared memory>
        
        std::string         sessionKey_;
        std::string         sessionDesc_;
        mutable std::mutex  sessionMutex_;

        // </thread shared memory>

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================