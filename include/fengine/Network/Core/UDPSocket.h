/*
 * Network UDP socket header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_UDP_SOCKET_H__
#define __FORK_NETWORK_UDP_SOCKET_H__


#include "Network/Core/Socket.h"
#include "Network/Core/Packet.h"


namespace Fork
{

namespace Network
{


DECL_SHR_PTR(UDPSocket);

/**
UDP/IP socket class. UDP sockets should be used for connection less protocols.
Transmitting packets with UDP sockets can be faster as with TCP sockets.
But there is not guarantee for a correct transmission. UDP sockets are required
for broadcasting (i.e. sending packets to all end-systems in a network).
\ingroup network
*/
class FORK_EXPORT UDPSocket : public Socket
{
    
    public:
        
        /**
        UDP/IP socket constructor.
        \param[in] domain Specifies the address domain (or rather address family).
        Must be AF_INET (for IPv4) or AF_INET6 (for IPv6). By default AF_INET.
        */
        UDPSocket(int domain = AF_INET);

        /**
        Binds the socket to the specified address.
        \param[in] address Specifies the socket address. Only the port is relevant. The IP address will be ignored.
        \return True if the socket has been bound seuccessfully to the specified address.
        \see AddressIPv4::AddressIPv4(unsigned short)
        \see AddressIPv6::AddressIPv6(unsigned short)
        */
        bool Bind(const Address& address);

        /**
        Sends a message. This is a wrapper function for the global "send" function of the berkeley socket API.
        \param[in] buffer Constant raw-pointer to the buffer which is to be send.
        \param[in] maxBufferSize Specifies the maximal buffer size.
        \return Number of bytes which were send.
        \throws NullPointerException If 'buffer' is a null pointer.
        \throws InvalidArgumentException If 'address->Domain() != GetDomain()'.
        */
        int Send(const void* buffer, size_t maxBufferSize, const Address& address) const;
        /**
        Receives a message. This is a wrapper function for the global "recvfrom" function of the berkeley socket API.
        \param[in] buffer Raw-pointer to the buffer where the received message will be stored.
        \param[in] maxBufferSize Specifies the maximal buffer size.
        \param[out] address Specifies the resulting address of the sender from which the message has been received.
        \return Number of bytes which were received.
        \throws NullPointerException If 'buffer' is a null pointer.
        \throws InvalidArgumentException If 'address->Domain() != GetDomain()'.
        */
        int Recv(void* buffer, size_t maxBufferSize, Address& address) const;

        /**
        Comfort function to send a string.
        \see Send(const void*, size_t, const Address&)
        */
        int SendString(const std::string& message, const Address& address) const;
        /**
        Comfort function to receive a string.
        \return Received string with a maximal length of 2048 characters (excluding the null terminator).
        \see Recv(void*, size_t, Address&)
        */
        std::string RecvString(Address& address, const size_t bufferMaxSize = Packet::defaultBufferMaxSize) const;

        /**
        Comfort function to send a packet.
        \see Send(const void*, size_t, const Address&)
        \see Packet
        */
        int SendPacket(const Packet& packet, const Address& address) const;
        /**
        Comfort function to receive a packet.
        \return Shared pointer to the received network packet.
        \see Recv(void*, size_t, Address&)
        \see Packet
        */
        Packet RecvPacket(Address& address, const size_t bufferMaxSize = Packet::defaultBufferMaxSize) const;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================