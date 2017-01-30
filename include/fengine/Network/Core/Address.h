/*
 * Network address header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_ADDRESS_H__
#define __FORK_NETWORK_ADDRESS_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Network/Core/NetworkImport.h"

#include <string>


namespace Fork
{

namespace Network
{


DECL_SHR_PTR(Address);

/**
Network address (or rather IP-address) interface.
\ingroup network
*/
class FORK_EXPORT Address
{
    
    public:
        
        virtual ~Address()
        {
        }

        //! Returns the address domain (or rather the address family). This will be AF_INET or AF_INET6.
        virtual int Domain() const = 0;

        //! Returns the port number.
        virtual unsigned short Port() const = 0;

        //! Returns the IP address as string (e.g. "127.0.0.1" for IPv4).
        virtual std::string AddressString() const = 0;

        //! Returns the full IP address string (e.g. "127.0.0.1:8000" for IPv4).
        virtual std::string FullAddressString() const = 0;

        //! Returns a raw-pointer to the native base address.
        virtual sockaddr* NativeBasePtr() = 0;
        //! Returns a constant raw-pointer to the native base address.
        virtual const sockaddr* NativeBasePtr() const = 0;

        //! Returns the size (in bytes) of the native address structure.
        virtual size_t NativeAddressSize() const = 0;

    protected:
        
        Address() = default;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================