/*
 * Network protocol HTTP response parser header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_PROTOCOL_HTTP_RESPONSE_PARSER_H__
#define __FORK_NETWORK_PROTOCOL_HTTP_RESPONSE_PARSER_H__


#include "Core/Export.h"

#include <map>
#include <string>


namespace Fork
{

namespace Network
{


/**
HTTP network protocol response parser.
Can be used to parse the GET response header for instance.
\see HTTPRecorder
*/
class FORK_EXPORT HTTPResponseParser
{
    
    public:
        
        /* === Types === */

        /**
        GET response header entry hash-map. Key := type, Value := data.
        E.g. key = "Server" -> Value = "Apache/1.3.27".
        */
        typedef std::map<std::string, std::string> HeaderEntryMap;
        /**
        Response code hash-map. Key := response code, Value := response message.
        E.g. Key = 404 -> Value = "Not Found".
        */
        typedef std::map<int, std::string> ResponseCodeMap;

        /* === Structures === */

        /**
        Structure for the GET response parsing result.
        \see ParseGETResponse
        */
        struct FORK_EXPORT GETResponse
        {
            //! HTTP version. Commonly "HTTP/1.1".
            std::string     httpVersion;
            /**
            GET response status code. By default 0.
            \see ResponseCodes
            */
            int             statusCode = 0;
            /**
            Status message. This can also be queries by the 'ResponseCodes' function.
            If 'statusCode' is 404 for instance, 'statusMessage' will be "Not Found".
            \see ResponseCodes
            */
            std::string     statusMessage;
            /**
            GET response header entries.
            \see HeaderEntryMap
            */
            HeaderEntryMap  headerEntries;
            //! GET response body content.
            std::string     bodyContent;
        };

        /* === Functions === */

        /**
        Parses the GET response header entries.

            HTTP/1.1 200 OK
            Date: Thu, 29 May 2014 15:45:58 GMT
            Connection: close
            Server: Apache/1.3.27
            Accept-Ranges: bytes
            Content-Type: text/html
            Content-Length: 170
            Last-Modified: Tue, 18 May 2014 10:14:49 GMT

        \see GetResponseGET
        */
        void ParseGETResponse(const std::string& content);

        /**
        Returns the response codes hash-map.
        \see ResponseCodeMap
        */
        static ResponseCodeMap ResponseCodes();

        /**
        Returns the GET response result. Call 'ParseGETResponse' first.
        \see ParseGETResponse
        \see GETResponse
        */
        inline const GETResponse& GetResponseGET() const
        {
            return responseGET_;
        }

    private:
        
        /* === Functions === */

        char Chr() const;
        std::string FindNextLine();
        std::string FindNextPart(const std::string& line, size_t& pos);

        void ParseGETResponseStatusLine(const std::string& line);
        void ParseGETResponseHeader(const std::string& line);

        /* === Members === */

        std::string content_;
        size_t pos_ = std::string::npos;

        GETResponse responseGET_;

};


} // /namespace Network

} // /namespace Fork


#endif



// ========================