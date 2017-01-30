/*
 * Daemon client header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_DAEMON_CLIENT_H__
#define __FORK_NETWORK_DAEMON_CLIENT_H__


#include "Network/Core/TCPSocket.h"
#include "Network/Core/AddressIPv4.h"
#include "Network/System/Daemon/PacketEnums.h"
#include "Network/System/Daemon/PacketStructs.h"

#include <array>


namespace Fork
{

namespace Network
{


/**
Main class for the daemon client.
\remarks The instantiation of this class requires that the network system was already initialized.
\see Platform::NetworkInterface::StartUp
*/
class FORK_EXPORT DaemonClient
{
    
    public:
        
        DaemonClient(const DaemonClient&) = delete;
        DaemonClient& operator = (const DaemonClient&) = delete;

        /**
        Constructs the clients and connects to the specified daemon server.
        \param[in] address Specifies the daemon server address.
        By default { DaemonInterface::defaultDaemonPort, "127.0.0.1" } (to connect to the local host).
        \note This function will block until the connection is established or failed!
        After that the socket will be configured to be non-blocking.
        \throws NetworkException If joining the daemon server failed.
        \see Socket::EnableBlocking
        \see DaemonInterface::defaultDaemonPort
        \see NetworkException
        */
        DaemonClient(const AddressIPv4& address = { DaemonInterface::defaultDaemonPort, "127.0.0.1" });
        virtual ~DaemonClient();

        /**
        Receives the server responses to the client requests.
        This function will execute the ForkENGINE-Daemon protocol.
        */
        void RecvResponses();

        void SendStatusRequest();
        void SendMemoryPushStartRequest(const DaemonInterface::ChunkSizeType chunkSize);
        /**
        \throws NullPointerException If 'data' is null.
        \throws InvalidArgumentException If 'size' is larger than 'DaemonInterface::maxPktSubBufferSize'.
        \see DaemonInterface::maxPktSubBufferSize
        */
        void SendMemoryPushSegmentRequest(
            const DaemonInterface::ChunkIDType chunkID,
            const DaemonInterface::ChunkSizeType chunkSize,
            const DaemonInterface::ChunkSizeType segmentOffset,
            const unsigned short segmentSize,
            const void* data
        );
        void SendMemoryPullStartRequest(const DaemonInterface::ChunkIDType chunkID);
        void SendMemoryPullSegmentRequest();
        /**
        \throws InvalidArgumentException If 'filename' is larger than 'DaemonInterface::maxPktSubBufferSize - 1'.
        */
        void SendMemoryStoreRequest(const DaemonInterface::ChunkIDType chunkID, const std::string& filename);
        void SendMemoryDropRequest(const DaemonInterface::ChunkIDType chunkID);
        void SendMemoryQueryRequest();

    protected:
        
        /**
        Callback function when an invalid packet ID was send from the daemon server.
        By default the following message is print to the log output.
        \code
        IO::Log::Error("Invalid packet ID ( " + ToStr(static_cast<int>(packetID)) + " ) from daemon server");
        \endcode
        \see IO::Log::Error
        */
        virtual void OnInvalidPacket(const DaemonInterface::PacketIDs::DataType packetID);

        virtual void OnConfirm          ( const DaemonInterface::PktConfirmOut&             packet );
        virtual void OnError            ( const DaemonInterface::PktErrorOut&               packet );
        virtual void OnStatus           ( const DaemonInterface::PktStatusOut&              packet );

        virtual void OnMemoryPushStart  ( const DaemonInterface::PktMemoryPushStartOut&     packet );
        virtual void OnMemoryPullStart  ( const DaemonInterface::PktMemoryPullStartOut&     packet );
        virtual void OnMemoryPullSegment( const DaemonInterface::PktMemoryPullSegmentOut&   packet );
        virtual void OnMemoryQuery      ( const DaemonInterface::PktMemoryQueryOut&         packet );

    private:
        
        /* === Functions === */

        void Send(size_t size);

        template <class T> void SendIDOnly();

        template <class T> const T& InputBuffer() const;
        template <class T> T* OutputBuffer();

        /* === Members === */

        TCPSocket socket_;

        std::array<char, DaemonInterface::maxPacketBufferSize> packetBufferIn_;     //!< Packet buffer for the client input packets.
        std::array<char, DaemonInterface::maxPacketBufferSize> packetBufferOut_;    //!< Packet buffer for the client output packets.
        
};


} // /namespace Network

} // /namespace Fork


#endif



// ========================