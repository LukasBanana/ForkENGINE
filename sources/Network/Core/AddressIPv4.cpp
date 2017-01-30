/*
 * Network address IPv4 file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/AddressIPv4.h"
#include "Core/StringModifier.h"
#include "Math/Core/Arithmetic/BitArithmetic.h"

#include <algorithm>


namespace Fork
{

namespace Network
{


AddressIPv4::AddressIPv4(unsigned short port)
{
    memset(&addr_, 0, sizeof(sockaddr_in));
    
    addr_.sin_family        = Domain();
    addr_.sin_port          = ::htons(port);
    addr_.sin_addr.s_addr   = ::htonl(INADDR_ANY);
}
AddressIPv4::AddressIPv4(unsigned short port, unsigned int addrIPv4)
{
    memset(&addr_, 0, sizeof(sockaddr_in));

    addr_.sin_family        = Domain();
    addr_.sin_port          = ::htons(port);
    addr_.sin_addr.s_addr   = addrIPv4;
}
AddressIPv4::AddressIPv4(unsigned short port, const std::string& addrIPv4, bool isHostName)
{
    memset(&addr_, 0, sizeof(sockaddr_in));

    addr_.sin_family    = Domain();
    addr_.sin_port      = ::htons(port);

    if (isHostName)
    {
        hostent* host = ::gethostbyname(addrIPv4.c_str());
        if (host)
            memcpy(&addr_.sin_addr, host->h_addr_list[0], host->h_length);
    }
    else
        addr_.sin_addr.s_addr = ::inet_addr(addrIPv4.c_str());
}
AddressIPv4::AddressIPv4(const sockaddr_in& nativeAddress)
{
    memcpy(&addr_, &nativeAddress, sizeof(sockaddr_in));
}

int AddressIPv4::Domain() const
{
    return AF_INET;
}

unsigned short AddressIPv4::Port() const
{
    return ::ntohs(addr_.sin_port);
}

unsigned int AddressIPv4::Address() const
{
    return addr_.sin_addr.s_addr;
}

std::string AddressIPv4::AddressString() const
{
    auto addr = reinterpret_cast<const unsigned char*>(&addr_.sin_addr.s_addr);

    std::string addrStr;

    addrStr += ToStr(static_cast<int>(addr[0])) + ".";
    addrStr += ToStr(static_cast<int>(addr[1])) + ".";
    addrStr += ToStr(static_cast<int>(addr[2])) + ".";
    addrStr += ToStr(static_cast<int>(addr[3]));

    return addrStr;
}

std::string AddressIPv4::FullAddressString() const
{
    return AddressString() + ":" + ToStr(Port());
}

sockaddr* AddressIPv4::NativeBasePtr()
{
    return reinterpret_cast<sockaddr*>(&addr_);
}

const sockaddr* AddressIPv4::NativeBasePtr() const
{
    return reinterpret_cast<const sockaddr*>(&addr_);
}

size_t AddressIPv4::NativeAddressSize() const
{
    return sizeof(sockaddr_in);
}

AddressIPv4::Classes AddressIPv4::AddressClass() const
{
    const auto addr = Address();

    if (!Math::GetBitL2R(addr, 0))
        return Classes::A;
    if (!Math::GetBitL2R(addr, 1))
        return Classes::B;
    if (!Math::GetBitL2R(addr, 2))
        return Classes::C;

    return Classes::Unknown;
}

std::string AddressIPv4::LocalHost()
{
    return "127.0.0.1";
}


} // /namespace Network

} // /namespace Fork



// ========================