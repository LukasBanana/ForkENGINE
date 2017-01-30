/*
 * Daemon server application file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "DaemonServer.h"
#include "DaemonServerParameters.h"
#include "Network/System/Daemon/PacketEnums.h"
#include "Service/ServiceMemoryManager.h"
#include "Platform/Network/NetworkInterface.h"
#include "IO/Core/SystemClock.h"
#include "IO/Core/Log.h"
#include "IO/FileSystem/LogFile.h"
#include "Core/STLHelper.h"

#include <thread>
#include <algorithm>


/* --- Main function --- */

int main(int argc, char* argv[])
{
    Fork::Daemon::DaemonServer app;

    if (!app.OnInit(argc, argv))
        return 0;
    
    app.OnRun();

    return app.OnExit();
}


namespace Fork
{

namespace Daemon
{


/* --- Internal membesr --- */

static const int serverMaxQueueLength = 32;

static char packetBuffer[maxPacketBufferSize];

/*
Returns a raw pointer to the specified structure 'T',
which has been reinterpret-casted from the packet buffer.
*/
template <class T> T* StructuredPacketBuffer()
{
    static_assert(sizeof(T) < maxPacketBufferSize, "Structure is too large for the packet buffer");
    return reinterpret_cast<T*>(packetBuffer);
}


/*
 * DaemonServer class
 */

DaemonServer* DaemonServer::instance_ = nullptr;

DaemonServer::DaemonServer()
{
    DaemonServer::instance_ = this;
    IO::Log::AddDefaultEventHandler();
}
DaemonServer::~DaemonServer()
{
    IO::Log::Message("Terminated @ " + IO::SystemClock::CurrentTimePoint());
    DaemonServer::instance_ = nullptr;
}

bool DaemonServer::OnInit(int argc, char* argv[])
{
    /* Parse and evalutate program arguments */
    ParseAppArguments(IO::CommandLine::ToList(argc - 1, argv + 1));

    /* Start running the daemon server */
    if (!GetAppConfig().runServer)
        return false;

    if (!GetAppConfig().logFilename.empty())
    {
        const auto fileExt = ToLower(ExtractFileExt(GetAppConfig().logFilename));
        if (fileExt == "html")
        {
            /* Opem HTML log file */
            IO::Log::AddEventHandler(std::make_shared<IO::LogFile::LogHTMLEventHandler>(
                GetAppConfig().logFilename, "ForkENGINE-Daemon Protocol History"
            ));
        }
        else
        {
            /* Open standard log file */
            IO::Log::AddEventHandler(std::make_shared<IO::LogFile::LogEventHandler>(
                GetAppConfig().logFilename
            ));
        }
    }

    IO::Log::Message(
        "Running ForkENGINE-Daemon (Protocol Version " +
        DaemonServer::ProtocolVersion() + ") @ " +
        IO::SystemClock::CurrentTimePoint()
    );

    /* Start-up network interface */
    Platform::NetworkInterface::StartUpInfo info;
    if (!Platform::NetworkInterface::StartUp(&info))
        return false;

    PrintIfVerbose(info.desc + " : " + info.status);

    /* Start daemon server */
    if (!StartServer())
        return false;

    return true;
}

void DaemonServer::OnRun()
{
    /* Main loop */
    while (!shutDown_)
    {
        /* Accept new connection and process protocol for all clients */
        AcceptNewConnection();
        ProcessProtocol();

        /* Give other processes time to run */
        if (GetAppConfig().updateRate > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(GetAppConfig().updateRate));
    }
}

int DaemonServer::OnExit()
{
    serverSocket_.reset();
    Platform::NetworkInterface::CleanUp();
    return 0;
}

std::string DaemonServer::StatusInfo() const
{
    const auto& cfg = GetAppConfig();

    return (
        "verbose:      " + ToStr( cfg.isVerbose   ) + "\n"
        "singleton:    " + ToStr( cfg.isSingleton ) + "\n"
        "print colors: " + ToStr( cfg.printColors ) + "\n"
        "port:         " + ToStr( cfg.port        ) + "\n"
        "update rate:  " + ToStr( cfg.updateRate  )
    );
}

std::string DaemonServer::ProtocolVersion()
{
    return "1.0";
}

AddressIPv4 DaemonServer::ServerAddress()
{
    return AddressIPv4(GetAppConfig().port, "127.0.0.1");
}

int DaemonServer::ServerQueueLength()
{
    return GetAppConfig().isSingleton ? 1 : serverMaxQueueLength;
}

DaemonServer* DaemonServer::Instance()
{
    return instance_;
}


/*
 * ======= Private: =======
 */

static std::string ClientAddr(const Client& client)
{
    return client.address->FullAddressString();
}

// This functions guarantees that an incomming network packet string is terminated by zero.
static std::string SafePacketString(char* str)
{
    str[maxPktSubBufferSize - 1] = '\0';
    return std::string(str);
}

void DaemonServer::PrintIfVerbose(const std::string& message, const Client* client)
{
    if (GetAppConfig().isVerbose)
    {
        if (client)
            IO::Log::Message(message + " @ " + ClientAddr(*client));
        else
            IO::Log::Message(message);
    }
}

void DaemonServer::ErrorIfVerbose(const std::string& message, const Client* client)
{
    if (GetAppConfig().isVerbose)
    {
        if (client)
            IO::Log::Error(message + " @ " + ClientAddr(*client));
        else
            IO::Log::Error(message);
    }
}

void DaemonServer::PrintPacketProcess(const std::string& packetName, const Client& client)
{
    if (GetAppConfig().isVerbose)
    {
        if (GetAppConfig().printColors)
            IO::Log::MessageColored("Process &lt;<1101>" + packetName + "</>&gt; @ <1011>" + ClientAddr(client) + "</>");
        else
            IO::Log::Message("Process <" + packetName + "> @ " + ClientAddr(client));
    }
}

bool DaemonServer::StartServer()
{
    serverSocket_ = std::make_unique<TCPSocket>();
    
    serverSocket_->EnableBlocking(false);

    auto address = DaemonServer::ServerAddress();
    if (serverSocket_->StartServer(address, DaemonServer::ServerQueueLength()))
    {
        IO::Log::Success("Started server @ " + address.FullAddressString() + " successful");
        return true;
    }

    return false;
}

/*
Registers a new client.
-> Disable blocking mode for the client's socket.
*/
void DaemonServer::RegisterClient(const Client& client)
{
    if (client)
    {
        clients_.push_back(client);

        client.socket->EnableBlocking(false);

        PrintIfVerbose("Register client " + ClientAddr(client));
    }
}

void DaemonServer::UnregisterClient(const Client& client)
{
    if (client)
    {
        PrintIfVerbose("Unregister client " + ClientAddr(client));
        RemoveFromList(clients_, client);
    }
}

void DaemonServer::AcceptNewConnection()
{
    /* Wait for new connections */
    TCPSocketPtr clientSocket;
    AddressPtr clientAddress;

    if (serverSocket_->AcceptConnection(clientSocket, clientAddress))
    {
        /*
        Allways accecpt new connections to flush queue,
        but only register new client if server configuration allows it.
        */
        if (!GetAppConfig().isSingleton || clients_.empty())
            RegisterClient({ clientSocket, clientAddress });
    }
}

void DaemonServer::ProcessProtocol()
{
    /* Process the protocol for each client */
    for (auto& client : clients_)
        ProcessProtocol(client);

    /* Unregister all clients which are to be removed (and close their socket) */
    if (!clientsToRemove_.empty())
    {
        for (const auto& client : clientsToRemove_)
            UnregisterClient(client);
        clientsToRemove_.clear();
    }
}

void DaemonServer::ProcessProtocol(Client& client)
{
    /* Receive packet buffer from client */
    while (client.socket->Recv(packetBuffer, maxPacketBufferSize) > 0)
    {
        /* Extract packet ID and select callback function */
        const auto packetID = static_cast<PacketIDs::DataType>(packetBuffer[0]);

        switch (packetID)
        {
            /* Common packets */
            case PacketIDs::ClientShutDownIn:
                ProcessPktClientShutDown(client);
                break;
            case PacketIDs::StatusIn:
                ProcessPktStatus(client);
                break;

            /* Service packets: "MemoryManager" */
            case PacketIDs::MemoryPushStartIn:
                ProcessPktMemoryPushStart(client, StructuredPacketBuffer<PktMemoryPushStartIn>());
                break;
            case PacketIDs::MemoryPushSegmentIn:
                ProcessPktMemoryPushSegment(client, StructuredPacketBuffer<PktMemoryPushSegmentIn>());
                break;
            case PacketIDs::MemoryPullStartIn:
                ProcessPktMemoryPullStart(client, StructuredPacketBuffer<PktMemoryPullStartIn>());
                break;
            case PacketIDs::MemoryPullSegmentIn:
                ProcessPktMemoryPullSegment(client);
                break;
            case PacketIDs::MemoryStoreIn:
                ProcessPktMemoryStore(client, StructuredPacketBuffer<PktMemoryStoreIn>());
                break;
            case PacketIDs::MemoryDropIn:
                ProcessPktMemoryDrop(client, StructuredPacketBuffer<PktMemoryDropIn>());
                break;
            case PacketIDs::MemoryQueryIn:
                ProcessPktMemoryQuery(client);
                break;

            default:
                PrintIfVerbose(
                    "Invalid packet ID ( " + ToStr(static_cast<int>(packetID)) +
                    " ) from client " + ClientAddr(client)
                );
                break;
        }
    }
}

void DaemonServer::ProcessPktClientShutDown(const Client& client)
{
    PrintPacketProcess("PktClientShutDownIn", client);

    /* Add client to the remove list */
    clientsToRemove_.push_back(client);
}

void DaemonServer::ProcessPktStatus(const Client& client)
{
    PrintPacketProcess("PktStatusIn", client);

    /* Setup output packet */
    auto packetOut = StructuredPacketBuffer<PktStatusOut>();

    packetOut->id               = PacketIDs::StatusOut;
    packetOut->numClients       = static_cast<unsigned char>(clients_.size());
    packetOut->numMemoryChunks  = static_cast<unsigned int>(Service::MemoryManager::NumChunks(client));

    /* Copy status info string */
    const auto info = StatusInfo();
    std::copy(info.begin(), info.end(), packetOut->info);
    packetOut->info[info.size()] = '\0';

    /* Send status back to client */
    client.socket->Send(
        packetOut,
        sizeof(unsigned char)*2 + sizeof(unsigned int) + sizeof(char)*(info.size() + 1)
    );
}

void DaemonServer::ProcessPktMemoryPushStart(const Client& client, PktMemoryPushStartIn* packetIn)
{
    PrintPacketProcess("PktMemoryPushStartIn", client);

    /* Allocate new memory chunk */
    const auto chunkID = Service::MemoryManager::AllocChunk(packetIn->chunkSize, client);
    
    PrintIfVerbose(
        "Allocate memory chunk ( " + ToStr(chunkID) +
        " ) (" + ToStr(packetIn->chunkSize) + " Bytes)"
    );

    /* Setup output packet */
    auto packetOut = StructuredPacketBuffer<PktMemoryPushStartOut>();

    packetOut->id       = PacketIDs::MemoryPushStartOut;
    packetOut->chunkID  = chunkID;

    /* Send output packet */
    client.socket->Send(packetOut, sizeof(PktMemoryPushStartOut));
}

void DaemonServer::ProcessPktMemoryPushSegment(const Client& client, PktMemoryPushSegmentIn* packetIn)
{
    PrintPacketProcess("PktMemoryPushSegmentIn", client);

    /* Push memory segment to chunk */
    PrintIfVerbose(
        "Push segment (" + ToStr(packetIn->segmentSize) +
        " Bytes) to memory chunk ( " + ToStr(packetIn->chunkID) + " )"
    );

    auto result = Service::MemoryManager::PushSegment(
        packetIn->chunkID,
        client,
        packetIn->segmentOffset,
        packetIn->segmentSize,
        packetIn->data
    );

    if (!result)
        ErrorIfVerbose("Pushing segment to memory chunk failed");
}

void DaemonServer::ProcessPktMemoryPullStart(Client& client, PktMemoryPullStartIn* packetIn)
{
    PrintPacketProcess("PktMemoryPullStartIn", client);

    /* Query memory chunk meta data */
    const auto chunkID  = packetIn->chunkID;
    auto chunkSize      = Service::MemoryManager::QueryChunkSize(chunkID, client);

    /* Add <pull task> to queue for this client */
    Client::MemoryPullTask task;
    {
        task.chunkID    = chunkID;
        task.chunkSize  = chunkSize;
    }
    client.memoryPullQueue.push(task);

    /* Setup output packet */
    auto packetOut = StructuredPacketBuffer<PktMemoryPullStartOut>();

    packetOut->id          = PacketIDs::MemoryPullStartOut;
    packetOut->chunkSize   = chunkSize;

    PrintIfVerbose(
        "Begin to pull memory chunk ( " + ToStr(task.chunkID) +
        " ) (" + ToStr(chunkSize) + " Bytes)"
    );

    /* Send output packet */
    client.socket->Send(packetOut, sizeof(PktMemoryPullStartOut));
}

void DaemonServer::ProcessPktMemoryPullSegment(Client& client)
{
    PrintPacketProcess("PktMemoryPullSegmentIn", client);

    /* Fetch current task in client queue */
    if (client.memoryPullQueue.empty())
        return;

    auto& task = client.memoryPullQueue.front();

    if (task.segmentOffset < task.chunkSize)
    {
        /* Get size for current packet block */
        const auto size = std::min(
            task.chunkSize - task.segmentOffset,
            static_cast<unsigned int>(maxPktSubBufferSize)
        );

        /* Pull segment from memory chunk */
        PrintIfVerbose(
            "Pull segment (" + ToStr(task.segmentOffset + size) +
            "/" + ToStr(task.chunkSize) + " Bytes) from memory chunk ( " + ToStr(task.chunkID) + " )"
        );

        /* Setup output packet */
        auto packetOut = StructuredPacketBuffer<PktMemoryPullSegmentOut>();

        packetOut->id               = PacketIDs::MemoryPullSegmentOut;
        packetOut->chunkID          = task.chunkID;
        packetOut->chunkSize        = task.chunkSize;
        packetOut->segmentOffset    = task.segmentOffset;
        packetOut->segmentSize      = size;

        auto result = Service::MemoryManager::PullSegment(task.chunkID, client, task.segmentOffset, size, packetOut->data);
        if (!result)
        {
            ErrorIfVerbose("Pulling segment from memory chunk failed");
            client.memoryPullQueue.pop();
            return;
        }

        task.segmentOffset += size;

        /* Send output packet */
        client.socket->Send(packetOut, sizeof(char) * (11 + size));
    }

    /* Pop task from client's queue if there is no further segment to pull */
    if (task.segmentOffset >= task.chunkSize)
        client.memoryPullQueue.pop();
}

void DaemonServer::ProcessPktMemoryStore(const Client& client, PktMemoryStoreIn* packetIn)
{
    PrintPacketProcess("PktMemoryStoreIn", client);

    /* Store memory to file */
    const auto filename = SafePacketString(packetIn->filename);

    PrintIfVerbose("Store memory chunk ( " + ToStr(packetIn->chunkID) + " ) to file \"" + filename + "\"");

    auto result = Service::MemoryManager::StoreChunkToFile(
        packetIn->chunkID, client, filename
    );

    if (!result)
        ErrorIfVerbose("Storing memory chunk to file failed");
}

void DaemonServer::ProcessPktMemoryDrop(const Client& client, PktMemoryDropIn* packetIn)
{
    PrintPacketProcess("PktMemoryDropIn", client);

    /* Free client's memory chunk */
    PrintIfVerbose(
        "Drop memory chunk ( " +
        (packetIn->chunkID == 0 ? std::string("All") : ToStr(packetIn->chunkID)) + " )"
    );

    auto result = Service::MemoryManager::FreeChunk(
        packetIn->chunkID, client
    );
    
    if (!result)
        ErrorIfVerbose("Droping memory chunk failed");
}

void DaemonServer::ProcessPktMemoryQuery(const Client& client)
{
    PrintPacketProcess("PktMemoryQueryIn", client);

    /* Enumerate chunk IDs by owner */
    auto chunkIDs = Service::MemoryManager::EnumChunkIDs(client);

    auto packetOut = StructuredPacketBuffer<PktMemoryQueryOut>();
    packetOut->id = PacketIDs::MemoryQueryOut;

    size_t n = chunkIDs.size();
    size_t offset = 0;

    if (n == 0)
    {
        /* Send output packet (with 0 IDs) */
        packetOut->numChunks = 0;
        client.socket->Send(packetOut, sizeof(unsigned char)*2);
        return;
    }

    while (n > 0)
    {
        /* Get number of IDs for current packet block */
        auto num = std::min(n, 255u);
        n -= num;

        /* Setup output packet */
        packetOut->numChunks = static_cast<unsigned char>(num);
        std::copy(chunkIDs.data() + offset, chunkIDs.data() + (offset + num), packetOut->chunkIDs);
        offset += num;

        /* Send output packet */
        client.socket->Send(packetOut, sizeof(unsigned char)*2 + sizeof(unsigned int)*num);
    }
}


} // /namespace Daemon

} // /namespace Fork



// ========================