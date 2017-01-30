/*
 * Network import header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_IMPORT_H__
#define __FORK_NETWORK_IMPORT_H__


#include "Platform/Core/OS.h"


/* --- Socket API header files --- */

#if defined(FORK_WINDOWS_PLATFORM)
#   if defined(FORK_GCC_COMPILER)
#       include <unistd.h>
#   elif defined(FORK_MSVC_COMPILER)
#       include <process.h>
#   endif
#   include <WinSock2.h>
#   include <WS2tcpip.h>
#   include <Windows.h>
#   include <lmserver.h>
#elif defined(FORK_POSIX_PLATFORM)
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <sys/select.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   include <netdb.h>
#   include <unistd.h>
#else
#   error OS not supported for socket API!
#endif


/* --- Required macros --- */

#ifndef SOCKET
#   define SOCKET           int
#endif
#ifndef INVALID_SOCKET
#   define INVALID_SOCKET   -1
#endif
#ifndef SOCKET_ERROR
#   define SOCKET_ERROR     -1
#endif

#if defined(FORK_WINDOWS_PLATFORM)
#   define SOCKET_SETOPT_PTR const char*
#else
#   define SOCKET_SETOPT_PTR const void*
#endif


#endif



// ========================