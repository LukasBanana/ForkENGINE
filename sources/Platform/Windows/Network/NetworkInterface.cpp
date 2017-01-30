/*
 * WIN32: Network interface file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Network/NetworkInterface.h"
#include "Network/Core/NetworkImport.h"
#include "IO/Core/Log.h"
#include "Core/StringModifier.h"

#include <lm.h>
#include <Iphlpapi.h>
#include <memory>


namespace Fork
{

namespace Platform
{

namespace NetworkInterface
{


static WSADATA winSockData;

FORK_EXPORT bool StartUp(StartUpInfo* info)
{
    /* Windows socket startup */
    const WORD versionRequest = MAKEWORD(2, 2);
    
    const int errorCode = WSAStartup(versionRequest, &winSockData);

    /* Check for error */
    if (errorCode)
    {
        std::string err;

        switch (errorCode)
        {
            case WSASYSNOTREADY:
                err = "WinSock not ready"; break;
            case WSAVERNOTSUPPORTED:
                err = "Requested WinSock version not supported"; break;
            case WSAEINPROGRESS:
                err = "Blocking WinSock 1.1 operation in progress"; break;
            case WSAEPROCLIM:
                err = "Maximum WinSock tasks reached"; break;
            case WSAEFAULT:
                err = "lpWSAData is not a valid pointer"; break;
            default:
                err = "Unknown error code (" + ToStr(errorCode) + ")"; break;
        }

        IO::Log::Error("WinSock startup error: " + err);

        return false;
    }

    if (info)
    {
        /* Pass start-up information to output argument */
        info->desc = winSockData.szDescription;
        info->status = winSockData.szSystemStatus;
    }

    return true;
}

FORK_EXPORT void CleanUp()
{
    WSACleanup();
}

FORK_EXPORT std::vector<Network::Adapter> QueryAdapters()
{
    std::vector<Network::Adapter> adapterList;

    /* Get first network adapter */
    ULONG bufferSize = sizeof(IP_ADAPTER_INFO);

    std::vector<char> adapterInfoBuffer(bufferSize);
    auto adapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(adapterInfoBuffer.data());
    
    if (GetAdaptersInfo(adapterInfo, &bufferSize) != ERROR_SUCCESS)
    {
        /* Resize adapter info buffer */
        adapterInfoBuffer.resize(bufferSize);
        adapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>(adapterInfoBuffer.data());
    }
    
    /* Final call to receive adapter info */
    DWORD result = GetAdaptersInfo(adapterInfo, &bufferSize);
    
    if (result != ERROR_SUCCESS)
    {   
        std::string ErrorStr;
        
        switch (result)
        {
            case ERROR_BUFFER_OVERFLOW:
                ErrorStr = "Buffer overflow"; break;
            case ERROR_INVALID_DATA:
                ErrorStr = "Invalid data"; break;
            case ERROR_INVALID_PARAMETER:
                ErrorStr = "Invalid parameter"; break;
            case ERROR_NO_DATA:
                ErrorStr = "No data"; break;
            case ERROR_NOT_SUPPORTED:
                ErrorStr = "Not supported"; break;
            default:
                ErrorStr = "Unknown error"; break;
        }
        
        IO::Log::Error("Enumerating network adapter information failed (" + ErrorStr + ")");
        
        return adapterList;
    }
    
    /* Iterate over all network adapters */
    while (adapterInfo)
    {
        /* Setup adapter data */
        auto desc       = std::string(adapterInfo->Description);
        auto address    = std::string(adapterInfo->IpAddressList.IpAddress.String);
        auto mask       = std::string(adapterInfo->IpAddressList.IpMask.String);
        bool isEnabled  = (adapterInfo->DhcpEnabled != 0);
        
        /* Setup adapter type */
        Network::Adapter::Types type = Network::Adapter::Types::__Unknown__;

        switch (adapterInfo->Type)
        {
            case MIB_IF_TYPE_ETHERNET:
                type = Network::Adapter::Types::Ethernet; break;
            case MIB_IF_TYPE_TOKENRING:
                type = Network::Adapter::Types::TokenRing; break;
            case MIB_IF_TYPE_FDDI:
                type = Network::Adapter::Types::FDDI; break;
            case MIB_IF_TYPE_PPP:
                type = Network::Adapter::Types::PPP; break;
            case MIB_IF_TYPE_LOOPBACK:
                type = Network::Adapter::Types::LoopBack; break;
            case MIB_IF_TYPE_SLIP:
                type = Network::Adapter::Types::SLIP; break;
            case MIB_IF_TYPE_OTHER:
            default:
                break;
        }
        
        /* Store adapter into list and get next adapter info */
        adapterList.push_back(Network::Adapter(type, desc, address, mask, isEnabled));

        adapterInfo = adapterInfo->Next;
    }

    return adapterList;
}


} // /namespace NetworkInterface

} // /namespace Platform

} // /namespace Fork



// ========================