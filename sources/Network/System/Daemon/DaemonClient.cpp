/*
 * Daemon client file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/System/Daemon/DaemonClient.h"
#include "Network/Core/NetworkException.h"
#include "IO/Core/Log.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"

#include <type_traits>


namespace Fork
{

namespace Network
{


using namespace DaemonInterface;

DaemonClient::DaemonClient(const AddressIPv4& address)
{
    if (!socket_.JoinServer(address))
    {
        throw NetworkException(
            __FUNCTION__,
            "Connection to daemon server (" + address.FullAddressString() + ") failed"
        );
    }
    socket_.EnableBlocking(false);
}
DaemonClient::~DaemonClient()
{
    SendIDOnly<PktClientShutDownIn>();
}

void DaemonClient::RecvResponses()
{
    /* Receive next packet from server */
    while (socket_.Recv(packetBufferIn_.data(), maxPacketBufferSize) > 0)
    {
        /* Extract packet ID and select callback function */
        const auto packetID = static_cast<PacketIDs::DataType>(packetBufferIn_[0]);

        switch (packetID)
        {
            case PacketIDs::ConfirmOut:
                OnConfirm(InputBuffer<PktConfirmOut>());
                break;
            case PacketIDs::ErrorOut:
                OnError(InputBuffer<PktErrorOut>());
                break;
            case PacketIDs::StatusOut:
                OnStatus(InputBuffer<PktStatusOut>());
                break;

            case PacketIDs::MemoryPushStartOut:
                OnMemoryPushStart(InputBuffer<PktMemoryPushStartOut>());
                break;
            case PacketIDs::MemoryPullStartOut:
                OnMemoryPullStart(InputBuffer<PktMemoryPullStartOut>());
                break;
            case PacketIDs::MemoryPullSegmentOut:
                OnMemoryPullSegment(InputBuffer<PktMemoryPullSegmentOut>());
                break;
            case PacketIDs::MemoryQueryOut:
                OnMemoryQuery(InputBuffer<PktMemoryQueryOut>());
                break;

            default:
                OnInvalidPacket(packetID);
                break;
        }
    }
}

void DaemonClient::SendStatusRequest()
{
    SendIDOnly<PktStatusIn>();
}

void DaemonClient::SendMemoryPushStartRequest(const ChunkSizeType chunkSize)
{
    auto packet = OutputBuffer<PktMemoryPushStartIn>();
    {
        packet->chunkSize = chunkSize;
    }
    Send(sizeof(PktMemoryPushStartIn));
}

void DaemonClient::SendMemoryPushSegmentRequest(
    const ChunkIDType chunkID, const ChunkSizeType chunkSize,
    const ChunkSizeType segmentOffset, const unsigned short segmentSize, const void* data)
{
    /* Validate pointer and data size */
    ASSERT_POINTER(data);

    if (segmentSize > maxPktSubBufferSize)
        throw InvalidArgumentException(__FUNCTION__, "size", "Memory segment must not be larger than " + ToStr(maxPktSubBufferSize) + " bytes");

    /* Get byte-aligned pointer of 'data' */
    auto dataByteAlign = reinterpret_cast<const char*>(data);

    /* Setup and send 'push-segment' packet*/
    auto packet = OutputBuffer<PktMemoryPushSegmentIn>();
    {
        packet->chunkID         = chunkID;
        packet->chunkSize       = chunkSize;
        packet->segmentOffset   = segmentOffset;
        packet->segmentSize     = segmentSize;
        std::copy(dataByteAlign, dataByteAlign + segmentSize, packet->data);
    }
    Send(size_t(11u + segmentSize));
}

void DaemonClient::SendMemoryPullStartRequest(const ChunkIDType chunkID)
{
    auto packet = OutputBuffer<PktMemoryPullStartIn>();
    {
        packet->chunkID = chunkID;
    }
    Send(sizeof(PktMemoryPullStartIn));
}

void DaemonClient::SendMemoryPullSegmentRequest()
{
    SendIDOnly<PktMemoryPullSegmentIn>();
}

void DaemonClient::SendMemoryStoreRequest(const DaemonInterface::ChunkIDType chunkID, const std::string& filename)
{
    if (filename.size() + 1 > maxPktSubBufferSize)
        throw InvalidArgumentException(__FUNCTION__, "filename", "Filename must not be larger than " + ToStr(maxPktSubBufferSize - 1) + " characters");

    auto packet = OutputBuffer<PktMemoryStoreIn>();
    {
        packet->chunkID = chunkID;
        std::copy(&filename[0], &filename[0] + filename.size() + 1, packet->filename);
    }
    Send(size_t(5u) + sizeof(char)*(filename.size() + 1));
}

void DaemonClient::SendMemoryDropRequest(const ChunkIDType chunkID)
{
    auto packet = OutputBuffer<PktMemoryDropIn>();
    {
        packet->chunkID = chunkID;
    }
    Send(sizeof(PktMemoryDropIn));
}

void DaemonClient::SendMemoryQueryRequest()
{
    SendIDOnly<PktMemoryQueryIn>();
}


/*
 * ======= Protected: =======
 */

void DaemonClient::OnInvalidPacket(const DaemonInterface::PacketIDs::DataType packetID)
{
    IO::Log::Error("Invalid packet ID ( " + ToStr(static_cast<int>(packetID)) + " ) from daemon server");
}

void DaemonClient::OnConfirm        ( const PktConfirmOut&           packet ) { /* dummy */ }
void DaemonClient::OnError          ( const PktErrorOut&             packet ) { /* dummy */ }
void DaemonClient::OnStatus         ( const PktStatusOut&            packet ) { /* dummy */ }
void DaemonClient::OnMemoryPushStart( const PktMemoryPushStartOut&   packet ) { /* dummy */ }
void DaemonClient::OnMemoryQuery    ( const PktMemoryQueryOut&       packet ) { /* dummy */ }

void DaemonClient::OnMemoryPullStart(const PktMemoryPullStartOut& packet)
{
    if (packet.chunkSize > 0)
        SendMemoryPullSegmentRequest();
}

void DaemonClient::OnMemoryPullSegment(const PktMemoryPullSegmentOut& packet)
{
    if (packet.segmentOffset + packet.segmentSize < packet.chunkSize)
        SendMemoryPullSegmentRequest();
}


/*
 * ======= Private: =======
 */

void DaemonClient::Send(size_t size)
{
    socket_.Send(packetBufferOut_.data(), size);
}

template <class T> void DaemonClient::SendIDOnly()
{
    OutputBuffer<T>();
    Send(sizeof(T));
}

template <class T> const T& DaemonClient::InputBuffer() const
{
    static_assert(sizeof(T) < maxPacketBufferSize, "Structure is too large for packet buffer");
    static_assert(!std::is_pointer<T>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
    return *reinterpret_cast<const T*>(packetBufferIn_.data());
}

template <class T> T* DaemonClient::OutputBuffer()
{
    static_assert(sizeof(T) < maxPacketBufferSize, "Structure is too large for packet buffer");
    static_assert(!std::is_pointer<T>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
    auto packet = reinterpret_cast<T*>(packetBufferOut_.data());
    packet->id = T::packetID;
    return packet;
}


} // /namespace Network

} // /namespace Fork



// ========================