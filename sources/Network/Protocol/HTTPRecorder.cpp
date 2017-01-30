/*
 * Network protocol HTTP file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Protocol/HTTPRecorder.h"
#include "Network/Core/AddressIPv4.h"

#include <functional>


namespace Fork
{

namespace Network
{


HTTPRecorder::HTTPRecorder()
{
}
HTTPRecorder::~HTTPRecorder()
{
    Disconnect();
}

bool HTTPRecorder::Connect(const std::string& hostName)
{
    /* Disconnect previous connection */
    if (IsConnected())
        Disconnect();

    /* Connect to HTTP host */
    socket_ = std::make_unique<TCPSocket>(AF_INET);

    hostName_ = hostName;
    return socket_->JoinServer(AddressIPv4(HTTPRecorder::Port(), hostName_, true));
}

void HTTPRecorder::Disconnect()
{
    if (socket_)
    {
        socket_->ShutDown();
        JoinThread();
        socket_ = nullptr;
    }
}

bool HTTPRecorder::IsConnected() const
{
    return socket_ != nullptr;
}

bool HTTPRecorder::SendGET(const std::string& resource)
{
    if (socket_)
    {
        /*
        Send "GET" request
        (HTTP standard requires "\r\n" as line ending)
        */
        socket_->SendString(
            "GET " + resource + " HTTP/1.1\r\n"
            "Host: " + GetHostName() + "\r\n"
            "\r\n"
        );

        /* Start response thread */
        RunThread();
    
        return true;
    }
    return false;
}

bool HTTPRecorder::SendOPTIONS()
{
    if (socket_)
    {
        /* Send "OPTIONS" request */
        socket_->SendString(
            "OPTIONS / HTTP/1.1\r\n"
            "Host: " + GetHostName() + "\r\n"
            "\r\n"
        );

        /* Start response thread */
        RunThread();
        
        return true;
    }
    return false;
}

bool HTTPRecorder::PollContent(std::string& content)
{
    std::lock_guard<std::mutex> gurad(contentQueueMutex_);

    if (!contentQueue_.empty())
    {
        content = contentQueue_.front();
        contentQueue_.pop();
        return true;
    }

    return false;
}

bool HTTPRecorder::IsContentComplete()
{
    return HasFinished();
}

unsigned short HTTPRecorder::Port()
{
    return 80;
}


/*
 * ======= Private: =======
 */

void HTTPRecorder::RunThread()
{
    ThreadLoopBase::RunThread(std::bind(&HTTPRecorder::ResponseThreadProc, this));
}

/*
This is the threading procedure for HTTP request responses.
*/
void HTTPRecorder::ResponseThreadProc()
{
    while (!HasFinished())
    {
        /* Receive next answer */
        auto message = socket_->RecvString();

        /* Check completion bit flag */
        if (message.empty())
        {
            Finish();
            return;
        }

        /* Push answer into queue */
        contentQueueMutex_.lock();
        contentQueue_.push(message);
        contentQueueMutex_.unlock();

        /* Wait a moment to give the other threads time to run */
        std::this_thread::yield();
    }
}


} // /namespace Network

} // /namespace Fork



// ========================