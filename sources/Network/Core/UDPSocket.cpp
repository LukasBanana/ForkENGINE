/*
 * Network UDP socket file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/UDPSocket.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Network
{


UDPSocket::UDPSocket(int domain) :
    Socket{ domain, SOCK_DGRAM, IPPROTO_UDP }
{
}

bool UDPSocket::Bind(const Address& address)
{
    /* Bind socket to address */
    if (::bind(GetNativeSocket(), address.NativeBasePtr(), static_cast<int>(address.NativeAddressSize())) == SOCKET_ERROR)
    {
        IO::Log::Error("Binding UDP/IP socket to address " + address.FullAddressString() + " failed");
        return false;
    }
    return true;
}

int UDPSocket::Send(const void* buffer, size_t maxBufferSize, const Address& address) const
{
    ASSERT_POINTER(buffer);

    if (address.Domain() != GetDomain())
        throw InvalidArgumentException(__FUNCTION__, "address", "Invalid address domain (or rather address family)");

    /* Send packet */
    return ::sendto(
        GetNativeSocket(),
        reinterpret_cast<const char*>(buffer),
        static_cast<int>(maxBufferSize),
        0,
        address.NativeBasePtr(),
        address.NativeAddressSize()
    );
}

int UDPSocket::Recv(void* buffer, size_t maxBufferSize, Address& address) const
{
    ASSERT_POINTER(buffer);

    if (address.Domain() != GetDomain())
        throw InvalidArgumentException(__FUNCTION__, "address", "Invalid address domain (or rather address family)");

    /* Sender address */
    union
    {
        sockaddr_in addrIPv4;
        sockaddr_in6 addrIPv6;
    };

    /*
    Receive packet
    -> address length (addrLen) must be initialized with largest buffer size!
    */
    int addrLen = sizeof(sockaddr_in6);

    int result = ::recvfrom(
        GetNativeSocket(),
        reinterpret_cast<char*>(buffer),
        static_cast<int>(maxBufferSize),
        0,
        reinterpret_cast<sockaddr*>(&addrIPv4),
        &addrLen
    );

    if (result > 0)
    {
        /* Setup output address */
        if (GetDomain() == AF_INET && addrLen == sizeof(sockaddr_in))
            memcpy(address.NativeBasePtr(), &addrIPv4, sizeof(sockaddr_in));
        else if (GetDomain() == AF_INET6 && addrLen == sizeof(sockaddr_in6))
            memcpy(address.NativeBasePtr(), &addrIPv6, sizeof(sockaddr_in6));
        else
        {
            IO::Log::Error("Setting up IP address from received UDP/IP packet failed");
            return 0;
        }
    }

    return result;
}

int UDPSocket::SendString(const std::string& message, const Address& address) const
{
    return Send(message.c_str(), message.size(), address);
}

std::string UDPSocket::RecvString(Address& address, const size_t bufferMaxSize) const
{
    std::string message;
    message.resize(bufferMaxSize);

    int recvLen = Recv(&message[0], bufferMaxSize, address);

    if (recvLen <= 0)
        return "";

    if (static_cast<size_t>(recvLen) < bufferMaxSize)
        message.resize(recvLen);

    return message;
}

int UDPSocket::SendPacket(const Packet& packet, const Address& address) const
{
    return Send(packet.RawBuffer(), packet.RawSize(), address);
}

Packet UDPSocket::RecvPacket(Address& address, const size_t bufferMaxSize) const
{
    Packet packet(bufferMaxSize);

    int recvLen = Recv(packet.RawBuffer(), packet.RawSize(), address);

    if (recvLen <= 0 || static_cast<size_t>(recvLen) < Packet::BufferHeaderSize())
        packet.Resize(0);
    else
        packet.RawResize(recvLen);

    return packet;
}


} // /namespace Network

} // /namespace Fork



// ========================