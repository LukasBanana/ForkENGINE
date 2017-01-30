/*
 * Network session login header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_SESSION_LOGIN_H__
#define __FORK_NETWORK_SESSION_LOGIN_H__


#include "Core/Export.h"
#include "Network/Core/UDPSocket.h"
#include "Network/Core/Address.h"


namespace Fork
{

namespace Network
{


/**
Network session login. This can be used within a client application.
This login can send a request to a running server session reception (see SessionReception).
\ingroup network
\see SessionReception
*/
class FORK_EXPORT SessionLogin
{
    
    public:
        
        SessionLogin(unsigned short port, int domain);
        virtual ~SessionLogin();

        /**
        Sends a login request to the specified address.
        \param[in] address Specifies the address to which the request is to be send. This can be a broadcast IP address.
        The port number will be ignored.
        \param[in] sessionKey Specifies the session key which will be send. By default unused.
        */
        bool SendRequest(const Address& address, const std::string& sessionKey = "");
        /**
        Sends a login request to all specified addresses.
        \see SendRequest(const Address&, const std::string&)
        */
        void SendRequest(const std::vector<const Address*>& addressList, const std::string& sessionKey = "");

        /**
        Receives the responses. This should be called every frame during a limited duration (e.g. for 2 or 3 seconds).
        If a request response is received, "OnResponse" will be called. This function should be overwritten.
        */
        void RecvResponses();

    protected:
        
        /**
        Request response callback function.
        \param[in] address Specifies the server address from the session reception.
        \param[in] sessionDesc Specifies the session description (e.g. the current game level name).
        \remarks Overwrite this function to receive session login response information.
        */
        virtual void OnResponse(const Address& address, const std::string& sessionDesc);

    private:
        
        UDPSocket socket_;

        std::unique_ptr<Address> loginAddress_;

        unsigned short port_ = 0;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================