
// ForkENGINE: Network Test
// 30/03/2014

#include <fengine/core.h>
#include <fengine/network.h>

#include <chrono>

using namespace Fork;

static void PrintIPAddress(const std::string& hostName, unsigned short port = 80)
{
    IO::Log::Message(
        hostName + " -> " +
        Network::AddressIPv4(port, hostName, true).FullAddressString()
    );
}

int main()
{
    FORK_MATCH_SDK_VERSION
    
    IO::Log::AddDefaultEventHandler();
    IO::Log::Message("--- Network Test ---");

    // Start up WinSock
    Platform::NetworkInterface::StartUpInfo info;
    Platform::NetworkInterface::StartUp(&info);

    IO::Log::Message(info.desc + " " + info.status);

    IO::Log::Blank();
    IO::Log::Message("--- IP Addresses: ---");
    PrintIPAddress("www.google.de");
    PrintIPAddress("www.google.com");
    PrintIPAddress("www.bing.de");
    PrintIPAddress("www.bing.com");
    PrintIPAddress("www.example.net");
    PrintIPAddress("softpixelengine.sourceforge.net");

    // Adapter test
    IO::Log::Blank();
    IO::Log::Message("--- Adapters: ---");

    auto adapterList = Platform::NetworkInterface::QueryAdapters();

    for (auto adapter : adapterList)
    {
        IO::Log::Message("Network Adapter:");
        IO::Log::ScopedIndent indent;

        IO::Log::Message( adapter.GetDesc()    );
        IO::Log::Message( adapter.GetAddress() );
        IO::Log::Message( adapter.GetMask()    );
        IO::Log::Message( "Broadcast Address: " + adapter.BroadcastAddress() );
    }

    // UDP/IP test
    #if 0
    IO::Log::Blank();
    IO::Log::Message("--- UDP/IP Socket Test: ---");

    {
        unsigned short testPort = 1234;

        Network::UDPSocket sock(AF_INET);

        sock.Bind(Network::AddressIPv4(testPort));

        Network::AddressIPv4 addr(testPort);
        auto packet = sock.RecvPacket(addr);

        IO::Log::Message("Packet Size: " + ToStr(packet.Size()));
    }
    #endif

    // Network session test
    const unsigned short sessionPort = 500;//12345;
    const std::string sessionKey = "";//":foo_bar:";

    int sel = 0;

    while (sel < 1 || sel > 2)
    {
        IO::Log::Message("Run Server App = 1");
        IO::Log::Message("Run Client App = 2");
        std::cin >> sel;
        std::cin.ignore(INT_MAX, '\n');
    }

    if (sel == 1)
    {
        IO::Log::Message("--- Start SERVER App: ---");

        std::string desc;

        #if 1
        IO::Log::Message("Enter Session Description:");
        std::getline(std::cin, desc);
        #else
        desc = "test";
        #endif

        // Open session reception
        auto reception = std::make_unique<Network::SessionReception>(
            Network::AddressIPv4(sessionPort), desc, sessionKey
        );

        IO::Console::Wait();
    }
    else if (sel == 2)
    {
        IO::Log::Message("--- Start CLIENT App: ---");

        class MySessionLogin : public Network::SessionLogin
        {
            public:
                MySessionLogin(unsigned short port) :
                    Network::SessionLogin(port, AF_INET)
                {
                }
            private:
                void OnResponse(const Network::Address& address, const std::string& sessionDesc)
                {
                    // Print request response
                    IO::Log::Message(
                        "Response from " + address.FullAddressString() +
                        ": \"" + sessionDesc + "\""
                    );
                }
        };

        auto login = std::make_unique<MySessionLogin>(sessionPort);

        for (auto adapter : adapterList)
        {
            // Send broadcast request
            login->SendRequest(
                Network::AddressIPv4(sessionPort, adapter.BroadcastAddress()),
                sessionKey
            );
        }

        // Wait for responses
        auto startTime = std::chrono::system_clock::now();

        int waitCounter = 0;

        while (waitCounter < 3)
        {
            login->RecvResponses();

            auto endTime = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

            if (duration > 0)
            {
                startTime = endTime;
                IO::Log::Message("Wait for response ...");
                ++waitCounter;
            }
        }

        IO::Console::Wait();
    }

    #if 0

    // Setup IP addresses
    IO::Log::Blank();
    IO::Log::Message("--- Connection Test: ---");

    Network::AddressIPv4 addrIPv4(80, "softpixelengine.sourceforge.net", true);

    Network::AddressIPv6 addrIPv6(80);

    IO::Log::Message(addrIPv4.FullAddressString());
    IO::Log::Message(addrIPv6.FullAddressString());

    {
        Network::HTTPRecorder http;

        try
        {
            http.Connect("softpixelengine.sourceforge.net");
            http.SendGET("/jimx_license.txt");

            std::string content;
            std::ofstream file("TestNetworkDebugOutput.txt");

            while (!http.IsContentComplete())
            {
                if (http.PollContent(content))
                    IO::Log::Message(content);

                std::this_thread::yield();
            }
        }
        catch (const DefaultException& err)
        {
            IO::Log::Error(err);
        }
    }

    #endif

    // Release network interface.
    Platform::NetworkInterface::CleanUp();
    
    return 0;
}


