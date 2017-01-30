
// ForkENGINE: Daemon Client Test
// 23/09/2014

#include <fengine/core.h>
#include <fengine/network.h>
#include <stack>

using namespace Fork;
using namespace Network;
using namespace DaemonInterface;

std::stack<unsigned int> chunkIDs;
unsigned int currentChunkID = 0;

bool waitForPacket = false;

void PushChunkID(unsigned int id)
{
    chunkIDs.push(currentChunkID);
    currentChunkID = id;
}

void PopChunkID()
{
    if (!chunkIDs.empty())
    {
        currentChunkID = chunkIDs.top();
        chunkIDs.pop();
    }
}

class MyClient : public DaemonClient
{
    
    public:
        
        MyClient(const AddressIPv4& address) :
            DaemonClient(address)
        {
        }

    private:
        
        void OnConfirm(const PktConfirmOut& packet) override
        {
        }

        void OnError(const PktErrorOut& packet) override
        {
        }

        void OnStatus(const PktStatusOut& packet) override
        {
            IO::Log::Message("Received <PktStatusOut>");
            IO::Log::Message("Clients: " + ToStr(static_cast<int>(packet.numClients)));
            IO::Log::Message("Chunks: " + ToStr(static_cast<int>(packet.numMemoryChunks)));
            IO::Log::Message("{");
            IO::Log::Message(packet.info);
            IO::Log::Message("}");
        }

        void OnMemoryPushStart(const PktMemoryPushStartOut& packet) override
        {
            PushChunkID(packet.chunkID);
            IO::Log::Message("Allocated memory chunk ( " + ToStr(currentChunkID) + " )");
        }

        void OnMemoryPullStart(const PktMemoryPullStartOut& packet) override
        {
            DaemonClient::OnMemoryPullStart(packet);
            waitForPacket = (packet.chunkSize > 0);
        }

        void OnMemoryPullSegment(const PktMemoryPullSegmentOut& packet) override
        {
            DaemonClient::OnMemoryPullSegment(packet);
            waitForPacket = (packet.segmentOffset + packet.segmentSize < packet.chunkSize);
        }

        void OnMemoryQuery(const PktMemoryQueryOut& packet) override
        {
            std::string info = "Memory chunk IDs: ";
            for (size_t i = 0; i < packet.numChunks; ++i)
            {
                info += ToStr(packet.chunkIDs[i]);
                if (i + 1 < packet.numChunks)
                    info += ", ";
            }
            IO::Log::Message(info);
        }

};

int main()
{
    FORK_MATCH_SDK_VERSION
    
    IO::Log::Message("--- ForkENGINE-Daemon Client Test ---");

    // Start up WinSock
    Platform::NetworkInterface::StartUpInfo info;
    Platform::NetworkInterface::StartUp(&info);

    IO::Log::AddDefaultEventHandler();
    IO::Log::Message(info.desc + " " + info.status);
    IO::Log::Blank();

    try
    {
        auto port = defaultDaemonPort;

        // Join to daemon server
        MyClient client({ port, "127.0.0.1" });

        // Main loop
        while (true)
        {
            char input = 0;

            do
            {
                // User input
                std::cout << std::endl << "Enter request code:" << std::endl;
                std::cout << " e -> exit" << std::endl;
                std::cout << " s -> daemon status" << std::endl;
                std::cout << " a -> alloc memory chunk" << std::endl;
                std::cout << " d -> drop memory chunk" << std::endl;
                std::cout << " p -> push segment to memory chunk" << std::endl;
                std::cout << " l -> pull segment from memory chunk" << std::endl;
                std::cout << " f -> store previous memory chunk to file" << std::endl;
                std::cout << " q -> query memory chunk IDs" << std::endl;

                std::cin >> input;
            }
            while (input != 'e' && input != 's' && input != 'a' && input != 'p' && input != 'l' && input != 'f' && input != 'd' && input != 'q');

            if (input == 'e')
                break;

            // Send packest
            switch (input)
            {
                case 's':
                    client.SendStatusRequest();
                    break;

                case 'a':
                    client.SendMemoryPushStartRequest(5000);
                    break;

                case 'd':
                    client.SendMemoryDropRequest(currentChunkID);
                    PopChunkID();
                    break;

                case 'p':
                    {
                    std::string str = "Hello, World.\nThis string was written to file by the ForkENGINE-Daemon!\n";
                    client.SendMemoryPushSegmentRequest(currentChunkID, 5000, 0, str.size(), str.c_str());
                    }
                    break;

                case 'l':
                    client.SendMemoryPullStartRequest(currentChunkID);
                    break;

                case 'f':
                    {
                    std::cout << "Enter filename:" << std::endl;
                    std::string filename;
                    while (filename.empty() || filename.size() > 1000)
                        std::getline(std::cin, filename);

                    client.SendMemoryStoreRequest(currentChunkID, filename);
                    }
                    break;

                case 'q':
                    client.SendMemoryQueryRequest();
                    break;
            }

            // Receive packets
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            do
            {
                client.RecvResponses();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            while (waitForPacket);
        }
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
    }

    // Release network interface.
    Platform::NetworkInterface::CleanUp();
    
    return 0;
}


