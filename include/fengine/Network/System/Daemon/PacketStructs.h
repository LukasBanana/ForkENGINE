/*
 * Daemon interface structures header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_DAEMON_STRUCTS_H__
#define __FORK_NETWORK_DAEMON_STRUCTS_H__


#include "Network/System/Daemon/PacketEnums.h"


namespace Fork
{

namespace Network
{

namespace DaemonInterface
{


#include "Core/PackPush.h"

//! Maximal packet sub-buffer size (1024 Bytes).
static const size_t maxPktSubBufferSize = 1024;


/* --- Types --- */

//! Data type for the packer ID numbers (8-bit unsigned integer).
typedef unsigned char PacketIDType;

//! Memory chunk ID type (32-bit unsigned integer).
typedef unsigned int ChunkIDType;

//! Memory chunk size type (32-bit unsigned integer).
typedef unsigned int ChunkSizeType;


/* --- Common packets --- */

struct PktClientShutDownIn
{
    static const PacketIDs::DataType packetID = PacketIDs::ClientShutDownIn;

    PacketIDType    id; //!< Must be 'PacketIDs::ClientShutDownIn'. \see PacketIDs
}
PACK_STRUCT;

struct PktConfirmOut
{
    static const PacketIDs::DataType packetID = PacketIDs::ConfirmOut;

    PacketIDType    id;         //!< Must be 'PacketIDs::ConfirmOut'. \see PacketIDs
    unsigned char   confirmId;  //!< Identifies the confirmed packet ID. \see PacketIDs
}
PACK_STRUCT;

struct PktErrorOut
{
    static const PacketIDs::DataType packetID = PacketIDs::ErrorOut;

    PacketIDType    id;                             //!< Must be 'PacketIDs::ErrorOut'. \see PacketIDs
    unsigned char   code;                           //!< Error code. \see ErrorCodes
    char            message[maxPktSubBufferSize];   //!< Null terminated string to the error message.
}
PACK_STRUCT;

struct PktStatusIn
{
    static const PacketIDs::DataType packetID = PacketIDs::StatusIn;

    PacketIDType    id; //!< Must be 'PacketIDs::StatusIn'. \see PacketIDs
}
PACK_STRUCT;

struct PktStatusOut
{
    static const PacketIDs::DataType packetID = PacketIDs::StatusOut;

    PacketIDType    id;                         //!< Must be 'PacketIDs::StatusOut'. \see PacketIDs
    unsigned char   numClients;                 //!< Number of active clients the server is currently handling.
    unsigned int    numMemoryChunks;            //!< Number of active memory chunks the server holds for the requesting client.
    char            info[maxPktSubBufferSize];  //!< Null terminated string to the status information text.
}
PACK_STRUCT;

/* --- Service packets: "MemoryManager" --- */

struct PktMemoryPushStartIn
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryPushStartIn;

    PacketIDType    id;         //!< Must be 'PacketIDs::MemoryPushStartIn'. \see PacketIDs
    ChunkSizeType   chunkSize;  //!< Size of the memory chunk (in bytes).
}
PACK_STRUCT;

struct PktMemoryPushStartOut
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryPushStartOut;

    PacketIDType    id;         //!< Must be 'PacketIDs::MemoryPushStartOut'. \see PacketIDs
    ChunkIDType     chunkID;    //!< Memory chunk ID which was selected by the server. Use this for 'pull' requests. 0 is invalid!
}
PACK_STRUCT;

struct PktMemoryPushSegmentIn
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryPushSegmentIn;

    PacketIDType    id;                         //!< Must be 'PacketIDs::MemoryPushSegmentIn'. \see PacketIDs
    ChunkIDType     chunkID;                    //!< ID to the memory chunk which is to be filled.
    ChunkSizeType   chunkSize;                  //!< Size of the memory chunk (in bytes).
    ChunkSizeType   segmentOffset;              //!< Memory segment offset (in bytes).
    unsigned short  segmentSize;                //!< Memory segment size (in bytes), maximal 'maxPktSubBufferSize'. \see maxPktSubBufferSize
    char            data[maxPktSubBufferSize];  //!< Memory segment data.
}
PACK_STRUCT;

struct PktMemoryPullStartIn
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryPullStartIn;

    PacketIDType    id;         //!< Must be 'PacketIDs::MemoryPushStartOut'. \see PacketIDs
    ChunkIDType     chunkID;    //!< Memory chunk ID which was selected by the server. Use this for 'pull' requests. 0 is invalid!
}
PACK_STRUCT;

struct PktMemoryPullStartOut
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryPullStartOut;

    PacketIDType    id;         //!< Must be 'PacketIDs::MemoryPullStartOut'. \see PacketIDs
    ChunkSizeType   chunkSize;  //!< Size of the memory chunk (in bytes).
}
PACK_STRUCT;

struct PktMemoryPullSegmentIn
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryPullSegmentIn;

    PacketIDType    id;         //!< Must be 'PacketIDs::MemoryPullSegmentIn'. \see PacketIDs
}
PACK_STRUCT;

struct PktMemoryPullSegmentOut
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryPullSegmentOut;

    PacketIDType    id;                         //!< Must be 'PacketIDs::MemoryPullSegmentOut'. \see PacketIDs
    ChunkIDType     chunkID;                    //!< ID to the memory chunk which is to be filled.
    ChunkSizeType   chunkSize;                  //!< Size of the memory chunk (in bytes).
    ChunkSizeType   segmentOffset;              //!< Memory segment offset (in bytes).
    unsigned short  segmentSize;                //!< Memory segment size (in bytes), maximal 'maxPktSubBufferSize'. \see maxPktSubBufferSize
    char            data[maxPktSubBufferSize];  //!< Memory segment data.
}
PACK_STRUCT;

struct PktMemoryStoreIn
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryStoreIn;

    PacketIDType    id;                             //!< Must be 'PacketIDs::MemoryStoreIn'. \see PacketIDs
    ChunkIDType     chunkID;                        //!< ID to the memory chunk which si to be stored into a file.
    char            filename[maxPktSubBufferSize];  //!< Null terminated string to the output filename.
}
PACK_STRUCT;

struct PktMemoryDropIn
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryDropIn;

    PacketIDType    id;         //!< Must be 'PacketIDs::MemoryDropIn'. \see PacketIDs
    ChunkIDType     chunkID;    //!< ID to the memory chunk which is to be droped.
}
PACK_STRUCT;

struct PktMemoryQueryIn
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryQueryIn;

    PacketIDType    id; //!< Must be 'PacketIDs::MemoryQueryIn'. \see PacketIDs
}
PACK_STRUCT;

struct PktMemoryQueryOut
{
    static const PacketIDs::DataType packetID = PacketIDs::MemoryQueryOut;

    PacketIDType    id;             //!< Must be 'PacketIDs::MemoryQueryOut'. \see PacketIDs
    unsigned char   numChunks;      //!< Number of memory chunks, maximal 255.
    ChunkIDType     chunkIDs[255];  //!< Memory chunk ID list.
}
PACK_STRUCT;

#include "Core/PackPop.h"


} // /namespace DaemonInterface

} // /namespace Network

} // /namespace Fork


#endif



// ========================