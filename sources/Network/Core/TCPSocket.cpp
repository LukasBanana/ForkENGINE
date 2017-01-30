/*
 * Network TCP socket file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/TCPSocket.h"
#include "Network/Core/AddressIPv4.h"
#include "Network/Core/AddressIPv6.h"
#include "Core/Exception/NullPointerException.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Network
{


TCPSocket::TCPSocket(int domain) :
    Socket{ domain, SOCK_STREAM, IPPROTO_TCP }
{
}
TCPSocket::TCPSocket(SOCKET socketHandle, int unused) :
    Socket{ socketHandle }
{
}

bool TCPSocket::StartServer(const Address& address, int queueLen)
{
    /* Bind socket to address */
    if (::bind(GetNativeSocket(), address.NativeBasePtr(), static_cast<int>(address.NativeAddressSize())) == SOCKET_ERROR)
    {
        IO::Log::Error("Binding TCP/IP socket to address " + address.FullAddressString() + " failed");
        return false;
    }

    /* Listen socket on this channel */
    if (::listen(GetNativeSocket(), queueLen) == SOCKET_ERROR)
    {
        IO::Log::Error("Listening TCP/IP socket to address " + address.FullAddressString() + " failed");
        return false;
    }

    return true;
}

bool TCPSocket::AcceptConnection(TCPSocketPtr& socket, AddressPtr& address)
{
    /* Setup address */
    union
    {
        sockaddr_in addrIPv4;
        sockaddr_in6 addrIPv6;
    };

    /*
    Accept new clients
    -> address length (addrLen) must be initialized with largest buffer size!
    */
    int addrLen = sizeof(sockaddr_in6);

    SOCKET socketHandle = ::accept(
        GetNativeSocket(),
        reinterpret_cast<sockaddr*>(&addrIPv4),
        &addrLen
    );

    if (socketHandle == INVALID_SOCKET)
    {
        if (IsBlocking())
            IO::Log::Error("Accepting incomming TCP/IP socket connection failed");
        return false;
    }

    /* Create address object */
    if (GetDomain() == AF_INET && addrLen == sizeof(sockaddr_in))
        address = std::make_shared<AddressIPv4>(addrIPv4);
    else if (GetDomain() == AF_INET6 && addrLen == sizeof(sockaddr_in6))
        address = std::make_shared<AddressIPv6>(addrIPv6);
    else
    {
        IO::Log::Error("Setting up IP address for incomming TCP/IP socket connectionn failed");
        return false;
    }

    /* Create socket object */
    socket = std::make_shared<TCPSocket>(socketHandle, true);

    return true;
}

bool TCPSocket::JoinServer(const Address& address)
{
    /* Connect socket to server */
    if (::connect(GetNativeSocket(), address.NativeBasePtr(), static_cast<int>(address.NativeAddressSize())) == SOCKET_ERROR)
    {
        IO::Log::Error("Connecting TCP/IP socket to address " + address.FullAddressString() + " failed");
        return false;
    }
    return true;
}

int TCPSocket::Send(const void* buffer, size_t maxBufferSize) const
{
    ASSERT_POINTER(buffer);

    return ::send(
        GetNativeSocket(),
        reinterpret_cast<const char*>(buffer),
        static_cast<int>(maxBufferSize),
        0
    );
}

int TCPSocket::Recv(void* buffer, size_t maxBufferSize) const
{
    ASSERT_POINTER(buffer);

    return ::recv(
        GetNativeSocket(),
        reinterpret_cast<char*>(buffer),
        static_cast<int>(maxBufferSize),
        0
    );
}

int TCPSocket::SendString(const std::string& message) const
{
    return Send(message.c_str(), message.size());
}

std::string TCPSocket::RecvString(const size_t bufferMaxSize) const
{
    std::string message;
    message.resize(bufferMaxSize);

    int recvLen = Recv(&message[0], bufferMaxSize);

    if (recvLen <= 0)
        return "";

    if (static_cast<size_t>(recvLen) < bufferMaxSize)
        message.resize(recvLen);

    return message;
}

int TCPSocket::SendPacket(const Packet& packet) const
{
    return Send(packet.RawBuffer(), packet.RawSize());
}

Packet TCPSocket::RecvPacket(const size_t bufferMaxSize) const
{
    Packet packet(bufferMaxSize);

    int recvLen = Recv(packet.RawBuffer(), packet.RawSize());

    if (recvLen <= 0 || static_cast<size_t>(recvLen) < Packet::BufferHeaderSize())
        packet.Resize(0);
    else
        packet.RawResize(recvLen);

    return packet;
}


} // /namespace Network

} // /namespace Fork



// ========================