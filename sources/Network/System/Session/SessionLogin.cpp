/*
 * Network session login file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/System/Session/SessionLogin.h"
#include "Network/Core/AddressIPv4.h"
#include "Network/Core/AddressIPv6.h"
#include "SessionCore.h"

#include "IO/Core/Log.h"//!!!


namespace Fork
{

namespace Network
{


SessionLogin::SessionLogin(unsigned short port, int domain) :
    socket_ { domain },
    port_   { port   }
{
    if (domain == AF_INET6)
        loginAddress_ = std::make_unique<AddressIPv6>(port);
    else
        loginAddress_ = std::make_unique<AddressIPv4>(port);

    socket_.EnableBlocking(false);
}
SessionLogin::~SessionLogin()
{
}

bool SessionLogin::SendRequest(const Address& address, const std::string& sessionKey)
{
    /* Setup request message and destination address */
    std::string message = std::string(sessionKeyPrefix) + sessionKey;

    #if 1//!!!
    IO::Log::Debug("SessionLogin sends \"" + message + "\" request to " + address.FullAddressString());
    #endif

    /* Send login request to destination */
    int result = SOCKET_ERROR;

    if (address.Domain() == AF_INET6)
    {
        const auto& addrIPv6 = static_cast<const AddressIPv6&>(address);
        result = socket_.SendString(message, AddressIPv6(port_, addrIPv6.Address()));
    }
    else
    {
        const auto& addrIPv4 = static_cast<const AddressIPv4&>(address);
        result = socket_.SendString(message, AddressIPv4(port_, addrIPv4.Address()));
    }

    /* Check for errors */
    if (result == SOCKET_ERROR)
    {
        IO::Log::Error("Sending session login request to " + address.FullAddressString() + " failed");
        return false;
    }

    return true;
}

void SessionLogin::SendRequest(const std::vector<const Address*>& addressList, const std::string& sessionKey)
{
    for (auto addr : addressList)
    {
        if (addr)
            SendRequest(*addr, sessionKey);
    }
}

void SessionLogin::RecvResponses()
{
    /* Receive request responses */
    auto message = socket_.RecvString(*loginAddress_);

    if (!message.empty())
        OnResponse(*loginAddress_, message);
}


/*
 * ======= Protected: =======
 */

void SessionLogin::OnResponse(const Address& address, const std::string& sessionDesc)
{
    /* Dummy */
}


} // /namespace Network

} // /namespace Fork



// ========================