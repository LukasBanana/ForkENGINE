/*
 * Network address IPv4 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_ADDRESS_IP4_H__
#define __FORK_NETWORK_ADDRESS_IP4_H__


#include "Network/Core/Address.h"


namespace Fork
{

namespace Network
{


/**
Network address for IP Version 4.
This is a wrapper class for the "sockaddr_in" structure of the berkely socket API.
You have access to this native address using the "GetNativeAddress" function.
\see AddressIPv4::GetNativeAddress
\ingroup network
*/
class FORK_EXPORT AddressIPv4 : public Address
{
    
    public:
        
        //! Network address classes.
        enum class Classes
        {
            Unknown,    //!< Unknown network address class.
            A,          //!< Network address class A has the net-mask 255.0.0.0.
            B,          //!< Network address class B has the net-mask 255.255.0.0.
            C,          //!< Network address class C has the net-mask 255.255.255.0.
        };

        /**
        Constructor with port number and the default address of the local host.
        \param[in] port Specifies the port number.
        */
        AddressIPv4(unsigned short port);
        /**
        Constructor with port number and IP address (IP Version 4).
        \param[in] port Specifies the port number.
        \param[in] addrIPv4 Specifies the IP address in version 4, i.e. 32-bit.
        */
        AddressIPv4(unsigned short port, unsigned int addrIPv4);
        /**
        Constructor with post number and IP address string (IP Version 4).
        \param[in] port Specifies the port number.
        \param[in] addrIPv4 Specifies the IP address string in version 4 (e.g. "127.0.0.1").
        \param[in] isHostName Specifies whether the address string is
        the host name (true) or an IP address (false). By default false.
        */
        AddressIPv4(unsigned short port, const std::string& addrIPv4, bool isHostName = false);
        /**
        Constructor for native address.
        \param[in] nativeAddress Specifies the native address of the berkeley socket API.
        */
        AddressIPv4(const sockaddr_in& nativeAddress);

        //! Returns AF_INET.
        int Domain() const;

        unsigned short Port() const;

        //! Returns the IP address value (32-bit unsigned integer).
        unsigned int Address() const;

        //! Returns the IP address as string (e.g. "127.0.0.1").
        std::string AddressString() const;

        //! Returns the full IP address string (e.g. "127.0.0.1:8000").
        std::string FullAddressString() const;

        sockaddr* NativeBasePtr();
        const sockaddr* NativeBasePtr() const;

        //! Returns sizeof(sockaddr_in).
        size_t NativeAddressSize() const;

        /**
        Returns the network address class.
        \see Classes
        */
        Classes AddressClass() const;

        //! Returns the IP address of the local host. For IP version 4 this is "127.0.0.1".
        static std::string LocalHost();

        //! Returns a reference to the native address of the berkeley socket API.
        inline sockaddr_in& GetNativeAddress()
        {
            return addr_;
        }
        //! Returns a constant reference to the native address of the berkeley socket API.
        inline const sockaddr_in& GetNativeAddress() const
        {
            return addr_;
        }

    private:
        
        sockaddr_in addr_;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================