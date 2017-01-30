/*
 * Daemon interface enumerations header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_DAEMON_ENUMS_H__
#define __FORK_NETWORK_DAEMON_ENUMS_H__


namespace Fork
{

namespace Network
{

namespace DaemonInterface
{


//! Default port number for the daemon server.
static const unsigned short defaultDaemonPort = 22914;

static const size_t maxPacketBufferSize = 1280;


//! Daemon interface packet ID enumeration.
struct PacketIDs
{
    typedef unsigned char DataType;
    enum : DataType
    {
        /* Common packets */
        ClientShutDownIn = 1,
        ConfirmOut,
        ErrorOut,
        StatusIn,
        StatusOut,

        /* Service packets: "MemoryManager" */
        MemoryPushStartIn,
        MemoryPushStartOut,
        MemoryPushSegmentIn,
        MemoryPullStartIn,
        MemoryPullStartOut,
        MemoryPullSegmentIn,
        MemoryPullSegmentOut,
        MemoryStoreIn,
        MemoryDropIn,
        MemoryQueryIn,
        MemoryQueryOut,
    };
};

//! Daemon interface error codes.
struct ErrorCodes
{
    typedef unsigned char DataType;
    enum : DataType
    {
        MemoryStoreFailed = 1,
        MemoryDropFailed,
    };
};


} // /namespace DaemonInterface

} // /namespace Network

} // /namespace Fork


#endif



// ========================