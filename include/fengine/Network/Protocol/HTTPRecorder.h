/*
 * Network protocol HTTP header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_PROTOCOL_HTTP_H__
#define __FORK_NETWORK_PROTOCOL_HTTP_H__


#include "Network/Core/TCPSocket.h"
#include "Network/Core/ThreadLoopBase.h"

#include <memory>
#include <mutex>
#include <queue>


namespace Fork
{

namespace Network
{


/**
Simple HTTP (Hyper-Text-Transfer-Protocol) recorder.
\remarks This class makes use of multi-threading.
\ingroup network
*/
class FORK_EXPORT HTTPRecorder : private ThreadLoopBase
{
    
    public:
        
        HTTPRecorder();
        ~HTTPRecorder();

        /**
        Connects to the specified HTTP host.
        \param[in] hostName Specifeis the HTTP host name (e.g. "www.google.de").
        \return True if the connection was successful.
        */
        bool Connect(const std::string& hostName);
        //! Disconnects from the current host.
        void Disconnect();

        /**
        Returns true if this recorder is currently connected.
        \see Connect
        */
        bool IsConnected() const;

        /**
        Sends a "GET" request for the specified content of the current HTTP server.
        \param[in] resource Specifies the content resource which is to be loaded (e.g. "/index.html").
        \return True if the request was send successfully.
        */
        bool SendGET(const std::string& resource);

        /**
        Sends an "OPTIONS" request to the current HTTP server.
        \return True if the request was send successfully.
        */
        bool SendOPTIONS();

        /**
        Polls the next string from the resource content.
        \return True if a new content could be polled.
        Otherwise the content queue is empty.
        */
        bool PollContent(std::string& content);

        //! Returns true if the content has been loaded completely.
        bool IsContentComplete();

        //! Returns the HTTP port number. This is 80.
        static unsigned short Port();

        //! Returns the current host name.
        inline const std::string& GetHostName() const
        {
            return hostName_;
        }

    private:
        
        void RunThread();

        void ResponseThreadProc();

        std::unique_ptr<TCPSocket>  socket_;
        std::string                 hostName_;

        // <thread shared memory>
        
        std::queue<std::string> contentQueue_;
        mutable std::mutex      contentQueueMutex_;

        // </thread shared memory>

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================