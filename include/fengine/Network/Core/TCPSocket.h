/*
 * Network TCP socket header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_TCP_SOCKET_H__
#define __FORK_NETWORK_TCP_SOCKET_H__


#include "Network/Core/Socket.h"
#include "Network/Core/Packet.h"


namespace Fork
{

namespace Network
{


DECL_SHR_PTR(TCPSocket);

/**
TCP/IP socket class. TCP sockets should be used for connection oriented protocols.
\ingroup network
*/
class FORK_EXPORT TCPSocket : public Socket
{
    
    public:
        
        /**
        TCP/IP socket constructor.
        \param[in] domain Specifies the address domain (or rather address family).
        Must be AF_INET (for IPv4) or AF_INET6 (for IPv6). By default AF_INET.
        */
        TCPSocket(int domain = AF_INET);
        /**
        TCP/IP socket constructor.
        \param[in] socketHandle Specifies the a native SOCKET handle.
        \param[in] unused Unused parameter. This is required because SOCKET is already an integer,
        like the first constructor's first parameter. Set this always to zero.
        */
        TCPSocket(SOCKET socketHandle, int unused);

        /**
        Starts a TCP/IP server.
        \param[in] address Specifies the server IP address. This can be either an IPv4 or IPv6 address.
        \param[in] queueLen Specifies the listen queue length.
        \return True if the server could be started.
        Otherwise an error occured which will be printed into the log output.
        \see AddressIPv4
        \see AddressIPv6
        */
        bool StartServer(const Address& address, int queueLen);

        /**
        Waits for incomming connections.
        \param[out] socket Output shared-pointer to the new accepted socket object.
        \param[out] address Output shared-pointer to the address of the new accepted socket.
        \return True if the incomming connection could be accepted.
        Otherwise blocking is disabled or an error occured which will be printed into the log output.
        \remarks This function will block until a new connection has been accepted if blocking mode is enabled!
        \see EnableBlocking
        */
        bool AcceptConnection(TCPSocketPtr& socket, AddressPtr& address);

        /**
        Joins (or rather connects to) a TCP/IP server.
        \param[in] address Specifies the server IP address. This can be either an IPv4 or IPv6 address.
        \return True if the connection was successful.
        Otherwise an error occured which will be printed into the log output.
        \see AddressIPv4
        \see AddressIPv6
        */
        bool JoinServer(const Address& address);

        /**
        Sends a message. This is a wrapper function for the global "send" function of the berkeley socket API.
        \param[in] buffer Constant raw-pointer to the buffer which is to be send.
        \param[in] maxBufferSize Specifies the maximal buffer size.
        \return Number of bytes which were send.
        \throws NullPointerException If 'buffer' is a null pointer.
        */
        int Send(const void* buffer, size_t maxBufferSize) const;
        /**
        Receives a message. This is a wrapper function for the global "recv" function of the berkeley socket API.
        \param[in] buffer Raw-pointer to the buffer where the received message will be stored.
        \param[in] maxBufferSize Specifies the maximal buffer size.
        \return Number of bytes which were received.
        \throws NullPointerException If 'buffer' is a null pointer.
        \note This function will block until a message has been received!
        */
        int Recv(void* buffer, size_t maxBufferSize) const;

        /**
        Comfort function to send a string.
        \see Send(const void*, size_t)
        */
        int SendString(const std::string& message) const;
        /**
        Comfort function to receive a string.
        \return Received string with a maximal length of 'bufferMaxSize' characters (excluding the null terminator).
        \see Recv(void*, size_t)
        */
        std::string RecvString(const size_t bufferMaxSize = Packet::defaultBufferMaxSize) const;

        /**
        Comfort function to send a packet.
        \see Send(const void*, size_t)
        \see Packet
        */
        int SendPacket(const Packet& packet) const;
        /**
        Comfort function to receive a packet.
        \return Shared pointer to the received network packet.
        \see Recv(void*, size_t)
        \see Packet
        */
        Packet RecvPacket(const size_t bufferMaxSize = Packet::defaultBufferMaxSize) const;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================