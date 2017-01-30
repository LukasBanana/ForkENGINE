/*
 * Daemon server peer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DAEMON_SERVER_SIDE_CLIENT_H__
#define __FORK_DAEMON_SERVER_SIDE_CLIENT_H__


#include "Network/Core/TCPSocket.h"
#include "Network/Core/AddressIPv4.h"
#include "Network/System/Daemon/PacketStructs.h"

#include <queue>


namespace Fork
{

namespace Daemon
{


//! Daemon (server side) client structure.
struct Client
{
    struct MemoryPullTask
    {
        Network::DaemonInterface::ChunkIDType   chunkID         = 0;
        Network::DaemonInterface::ChunkSizeType chunkSize       = 0;
        Network::DaemonInterface::ChunkSizeType segmentOffset   = 0;
    };

    inline operator bool () const
    {
        return socket != nullptr && address != nullptr;
    }

    Network::TCPSocketPtr       socket;
    Network::AddressPtr         address;

    std::queue<MemoryPullTask>  memoryPullQueue;
};

inline bool operator == (const Client& lhs, const Client& rhs)
{
    return lhs.socket == rhs.socket && lhs.address == rhs.address;
}


} // /namespace Daemon

} // /namespace Fork


#endif



// ========================