/*
 * Network address IPv6 file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Core/AddressIPv6.h"
#include "Core/StringModifier.h"
#include "Math/Core/Arithmetic/BitArithmetic.h"

#include <algorithm>


namespace Fork
{

namespace Network
{


/*
 * IPAddress structure
 */

AddressIPv6::IPAddress::IPAddress()
{
    memset(&byte, 0, sizeof(unsigned char)*16);
    //std::fill(&byte, &byte + 16, 0);
}
AddressIPv6::IPAddress::IPAddress(const unsigned char (&addrByte)[16])
{
    memcpy(&byte, &addrByte, sizeof(unsigned char)*16);
    //std::copy(&addrByte, &addrByte + 16, &byte);
}
AddressIPv6::IPAddress::IPAddress(const unsigned short (&addrWord)[8])
{
    memcpy(&word, &addrWord, sizeof(unsigned short)*8);
    //std::copy(&addrWord, &addrWord + 8, &word);
}
AddressIPv6::IPAddress::IPAddress(const unsigned int (&addrDWord)[4])
{
    memcpy(&dword, &addrDWord, sizeof(unsigned int)*4);
    //std::copy(&addrDWord, &addrDWord + 4, &dword);
}
AddressIPv6::IPAddress::IPAddress(
    unsigned short word0, unsigned short word1, unsigned short word2, unsigned short word3)
{
    word[0] = word0;
    word[1] = word1;
    word[2] = word2;
    word[3] = word3;
    word[4] = 0;
    word[5] = 0;
    word[6] = 0;
    word[7] = 0;
}


/*
 * AddressIPv6 class
 */

AddressIPv6::AddressIPv6(unsigned short port)
{
    memset(&addr_, 0, sizeof(sockaddr_in6));
    //std::fill(&addr_, &addr_ + 1, 0);
    
    addr_.sin6_family        = Domain();
    addr_.sin6_port          = ::htons(port);
    //addr_.sin6_addr.u.Word   = ::htonl(INADDR_ANY);
}
AddressIPv6::AddressIPv6(unsigned short port, const IPAddress& addrIPv6)
{
}
AddressIPv6::AddressIPv6(unsigned short port, const std::string& addrIPv6, bool isHostName)
{
}
AddressIPv6::AddressIPv6(const sockaddr_in6& nativeAddress)
{
    memcpy(&addr_, &nativeAddress, sizeof(sockaddr_in6));
    //std::copy(&nativeAddress, &nativeAddress + 1, addr_);
}

int AddressIPv6::Domain() const
{
    return AF_INET6;
}

unsigned short AddressIPv6::Port() const
{
    return ::ntohs(addr_.sin6_port);
}

AddressIPv6::IPAddress AddressIPv6::Address() const
{
    return IPAddress(addr_.sin6_addr.u.Word);
}

std::string AddressIPv6::AddressString() const
{
    const auto addr = Address();

    std::string addrStr;

    for (int i = 0; i < 8; ++i)
    {
        addrStr += NumToHex(addr.word[i]);
        if (i < 7)
            addrStr += ':';
    }

    return addrStr;
}

std::string AddressIPv6::FullAddressString() const
{
    return "[" + AddressString() + "]:" + ToStr(Port());
}

sockaddr* AddressIPv6::NativeBasePtr()
{
    return reinterpret_cast<sockaddr*>(&addr_);
}

const sockaddr* AddressIPv6::NativeBasePtr() const
{
    return reinterpret_cast<const sockaddr*>(&addr_);
}

size_t AddressIPv6::NativeAddressSize() const
{
    return sizeof(sockaddr_in6);
}

std::string AddressIPv6::LocalHost()
{
    return "::1/128";
}


} // /namespace Network

} // /namespace Fork



// ========================