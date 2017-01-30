/*
 * Network socket header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_SOCKET_H__
#define __FORK_NETWORK_SOCKET_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Network/Core/NetworkImport.h"
#include "Network/Core/Address.h"


namespace Fork
{

namespace Network
{


DECL_SHR_PTR(Socket);

//! Socket wrapper interface for the berkeley socket API.
class FORK_EXPORT Socket
{
    
    public:
        
        /**
        Wrapper interface to the "socket" function from the berkeley socket API.
        \see http://pubs.opengroup.org/onlinepubs/009695399/functions/socket.html
        */
        Socket(int domain, int type, int protocol);
        Socket(SOCKET socketHandle);
        virtual ~Socket();

        //! Returns true if this is a valid socket. Otherwise socket creation failed.
        bool IsValid() const;

        /**
        Disables the sending and receiving process for this socket.
        \remarks This should be used when sub thread is receiving messages from this socket
        while the socket is blocking (because no further messages are received)
        and another thread wants to join this thread (to terminate the sub thread).
        */
        void ShutDown();

        //! Sets the socket blocking mode and returns true on success. By default enabled.
        bool EnableBlocking(bool enable);
        //! Sets the socket broad casting mode and returns true on success. By default disabled.
        bool EnableBroadCasting(bool enable);
        //! Sets the socket re-use address mode and returns true on success. By default disabled.
        bool EnableReUseAddress(bool enable);

        //! Returns a constant reference to the native socket of the berkeley socket API.
        inline SOCKET GetNativeSocket() const
        {
            return socketHandle_;
        }

        //! Returns the socket domain (or rather address family). This should be AF_INET or AF_INET6.
        inline int GetDomain() const
        {
            return domain_;
        }

        /**
        Returns true if blocking mode is enabled. This is true for sockets by default.
        \see EnableBlocking
        */
        inline bool IsBlocking() const
        {
            return isBlocking_;
        }

    private:
        
        SOCKET  socketHandle_   = INVALID_SOCKET;
        int     domain_         = 0;

        bool    isBlocking_     = true;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================