/*
 * Network address IPv6 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_ADDRESS_IP6_H__
#define __FORK_NETWORK_ADDRESS_IP6_H__


#include "Network/Core/Address.h"


namespace Fork
{

namespace Network
{


/**
Network address for IP Version 6.
This is a wrapper class for the "sockaddr_in6" structure of the berkely socket API.
You have access to this native address using the "GetNativeAddress" function.
\see AddressIPv6::GetNativeAddress
\ingroup network
*/
class FORK_EXPORT AddressIPv6 : public Address
{
    
    public:
        
        //! IP address structure (IP Version 6).
        struct FORK_EXPORT IPAddress
        {
            IPAddress();
            IPAddress(const unsigned char (&addrByte)[16]);
            IPAddress(const unsigned short (&addrWord)[8]);
            IPAddress(const unsigned int (&addrDWord)[4]);
            IPAddress(unsigned short word0, unsigned short word1, unsigned short word2, unsigned short word3);

            union
            {
                unsigned char byte[16];
                unsigned short word[8];
                unsigned int dword[4];
            };
        };

        /**
        Constructor with port number and the default address of the local host.
        \param[in] port Specifies the port number.
        */
        AddressIPv6(unsigned short port);
        /**
        Constructor with port number and IP address (IP Version 6).
        \param[in] port Specifies the port number.
        \param[in] addrIPv6 Specifies the IP address in version 6, i.e. 128-bit.
        */
        AddressIPv6(unsigned short port, const IPAddress& addrIPv6);
        /**
        Constructor with post number and IP address string (IP Version 6).
        \param[in] port Specifies the port number.
        \param[in] addrIPv6 Specifies the IP address string in version 6 (e.g. "").
        \param[in] isHostName Specifies whether the address string is
        the host name (true) or an IP address (false). By default false.
        */
        AddressIPv6(unsigned short port, const std::string& addrIPv6, bool isHostName = false);
        /**
        Constructor for native address.
        \param[in] nativeAddress Specifies the native address of the berkeley socket API.
        */
        AddressIPv6(const sockaddr_in6& nativeAddress);

        //! Returns AF_INET6.
        int Domain() const;

        unsigned short Port() const;

        /**
        Returns the IP address value (128-bit unsigned integer).
        \see IPAddress
        */
        IPAddress Address() const;

        //! Returns the IP address as string (e.g. "2001:0db8:85a3:0000:0000:8a2e:0370:7334").
        std::string AddressString() const;

        //! Returns the full IP address string (e.g. "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:8000").
        std::string FullAddressString() const;

        sockaddr* NativeBasePtr();
        const sockaddr* NativeBasePtr() const;

        //! Returns sizeof(sockaddr_in6).
        size_t NativeAddressSize() const;

        //! Returns the IP address of the local host. For IP version 6 this is "::1/128".
        static std::string LocalHost();

        //! Returns a reference to the native address of the berkeley socket API.
        inline sockaddr_in6& GetNativeAddress()
        {
            return addr_;
        }
        //! Returns a constant reference to the native address of the berkeley socket API.
        inline const sockaddr_in6& GetNativeAddress() const
        {
            return addr_;
        }

    private:
        
        sockaddr_in6 addr_;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================