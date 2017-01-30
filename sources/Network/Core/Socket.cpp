/*
 * Network socket file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/Socket.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Network
{


Socket::Socket(int domain, int type, int protocol) :
    domain_{ domain }
{
    socketHandle_ = ::socket(domain, type, protocol);

    if (socketHandle_ == INVALID_SOCKET)
        IO::Log::Error("Creating IP socket failed");
}
Socket::Socket(SOCKET socketHandle) :
    socketHandle_{ socketHandle }
{
}
Socket::~Socket()
{
    #ifdef FORK_WINDOWS_PLATFORM
    ::closesocket(socketHandle_);
    #else
    ::close(socketHandle_);
    #endif
}

bool Socket::IsValid() const
{
    return GetNativeSocket() != INVALID_SOCKET;
}

void Socket::ShutDown()
{
    /*
    2 means both sending and receiving operations will be shutdown.
    On WIN32 -> SD_BOTH = 2
    */
    ::shutdown(GetNativeSocket(), 2);
}

bool Socket::EnableBlocking(bool enable)
{
    /* Setup socket blocking mode */
    unsigned long flags = (enable ? 0 : 1);
    
    auto result = ::ioctlsocket(GetNativeSocket(), FIONBIO, &flags);

    if (result != NO_ERROR)
    {
        IO::Log::Error("Setting socket 'blocking' mode failed");
        return false;
    }

    /* Store new setting */
    isBlocking_ = enable;

    return true;
}

bool Socket::EnableBroadCasting(bool enable)
{
    /* Setup socket broad-casting mode */
    int flags = (enable ? 1 : 0);
    
    auto result = ::setsockopt(
        GetNativeSocket(), SOL_SOCKET, SO_BROADCAST,
        reinterpret_cast<SOCKET_SETOPT_PTR>(&flags), sizeof(flags)
    );

    if (result == SOCKET_ERROR)
    {
        IO::Log::Error("Setting socket 'broad-casting' mode failed");
        return false;
    }

    return true;
}

bool Socket::EnableReUseAddress(bool enable)
{
    /* Setup socket re-use address mode */
    int flags = (enable ? 1 : 0);

    auto result = ::setsockopt(
        GetNativeSocket(), SOL_SOCKET, SO_REUSEADDR,
        reinterpret_cast<SOCKET_SETOPT_PTR>(&flags), sizeof(flags)
    );

    if (result == SOCKET_ERROR)
    {
        IO::Log::Error("Setting socket 're-use address' mode failed");
        return false;
    }

    return true;
}


} // /namespace Network

} // /namespace Fork



// ========================