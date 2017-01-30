/*
 * Daemon server application header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DAEMON_SERVER_H__
#define __FORK_DAEMON_SERVER_H__


#include "Network/Core/TCPSocket.h"
#include "Network/Core/AddressIPv4.h"
#include "Network/System/Daemon/PacketStructs.h"
#include "ServerSideClient.h"

#include <memory>
#include <vector>


namespace Fork
{

using namespace Network;
using namespace DaemonInterface;

namespace Daemon
{


//! Main class for the daemon server application.
class DaemonServer
{
    
    public:
        
        DaemonServer();
        ~DaemonServer();

        /* === Callback functions === */

        bool OnInit(int argc, char* argv[]);
        void OnRun();
        int OnExit();

        /* === Functions === */

        std::string StatusInfo() const;

        //! Returns the daemon protocol version string.
        static std::string ProtocolVersion();

        //! Returns the server address used by this daemon application.
        static AddressIPv4 ServerAddress();
        //! Returns the server listener queue length.
        static int ServerQueueLength();

        static DaemonServer* Instance();

    private:
        
        /* === Functions === */

        void PrintIfVerbose(const std::string& message, const Client* client = nullptr);
        void ErrorIfVerbose(const std::string& message, const Client* client = nullptr);
        void PrintPacketProcess(const std::string& packetName, const Client& client);

        bool StartServer();

        void RegisterClient(const Client& client);
        void UnregisterClient(const Client& client);

        void AcceptNewConnection();

        void ProcessProtocol();
        void ProcessProtocol(Client& client);

        void ProcessPktClientShutDown   (const Client& client);
        void ProcessPktStatus           (const Client& client);
        void ProcessPktMemoryPushStart  (const Client& client, PktMemoryPushStartIn*    packetIn);
        void ProcessPktMemoryPushSegment(const Client& client, PktMemoryPushSegmentIn*  packetIn);
        void ProcessPktMemoryPullStart  (      Client& client, PktMemoryPullStartIn*    packetIn);
        void ProcessPktMemoryPullSegment(      Client& client);
        void ProcessPktMemoryStore      (const Client& client, PktMemoryStoreIn*        packetIn);
        void ProcessPktMemoryDrop       (const Client& client, PktMemoryDropIn*         packetIn);
        void ProcessPktMemoryQuery      (const Client& client);

        /* === Members === */

        std::unique_ptr<TCPSocket> serverSocket_;
        std::vector<Client> clients_, clientsToRemove_;

        bool shutDown_ = false;

        static DaemonServer* instance_;
        
};


} // /namespace Daemon

} // /namespace Fork


#endif



// ========================