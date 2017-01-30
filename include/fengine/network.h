/*
 * Network header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_H__
#define __FORK_NETWORK_H__


/* --- Static config header file --- */

#include "Core/StaticConfig.h"


/* --- Core header files --- */

#include "Network/Core/TCPSocket.h"
#include "Network/Core/UDPSocket.h"
#include "Network/Core/AddressIPv4.h"
#include "Network/Core/AddressIPv6.h"
#include "Network/Core/Packet.h"
#include "Network/Core/Adapter.h"
#include "Network/Core/NetworkException.h"


/* --- Protocol header files --- */

#include "Network/Protocol/HTTPRecorder.h"
#include "Network/Protocol/HTTPResponseParser.h"


/* --- System header files --- */

#include "Network/System/Daemon/PacketEnums.h"
#include "Network/System/Daemon/PacketStructs.h"
#include "Network/System/Daemon/DaemonClient.h"
#include "Network/System/Session/SessionReception.h"
#include "Network/System/Session/SessionLogin.h"


/* --- Platform header files --- */

#include "Platform/Network/NetworkInterface.h"


#endif



// ========================