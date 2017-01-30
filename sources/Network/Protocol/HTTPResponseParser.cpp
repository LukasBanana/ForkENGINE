/*
 * Network protocol HTTP response parser file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Network/Protocol/HTTPResponseParser.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Network
{


void HTTPResponseParser::ParseGETResponse(const std::string& content)
{
    if (content.empty())
        return;

    content_ = content;
    pos_ = 0;
    
    /* Parse status line */
    ParseGETResponseStatusLine(FindNextLine());

    /* Parse header lines */
    while (true)
    {
        auto line = FindNextLine();

        if (line.empty())
            break;

        ParseGETResponseHeader(line);
    }

    /* Parse body content */
    responseGET_.bodyContent = content_.substr(pos_);
}

HTTPResponseParser::ResponseCodeMap HTTPResponseParser::ResponseCodes()
{
    return
    {
        { 100, "Continue"                        },
        { 101, "Switching Protocols"             },
        { 200, "OK"                              },
        { 201, "Created"                         },
        { 202, "Accepted"                        },
        { 203, "Non-Authoritative Information"   },
        { 204, "No Content"                      },
        { 205, "Reset Content"                   },
        { 206, "Partial Content"                 },
        { 300, "Multiple Choices"                },
        { 301, "Moved Permanently"               },
        { 302, "Found"                           },
        { 303, "See Other"                       },
        { 304, "Not Modified"                    },
        { 305, "Use Proxy"                       },
        { 307, "Temporary Redirect"              },
        { 400, "Bad Request"                     },
        { 401, "Unauthorized"                    },
        { 402, "Payment Required"                },
        { 403, "Forbidden"                       },
        { 404, "Not Found"                       },
        { 405, "Method Not Allowed"              },
        { 406, "Not Acceptable"                  },
        { 407, "Proxy Authentication Required"   },
        { 408, "Request Time-out"                },
        { 409, "Conflict"                        },
        { 410, "Gone"                            },
        { 411, "Length Required"                 },
        { 412, "Precondition Failed"             },
        { 413, "Request Entity Too Large"        },
        { 414, "Request-URI Too Large"           },
        { 415, "Unsupported Media Type"          },
        { 416, "Requested range not satisfiable" },
        { 417, "Expectation Failed"              },
        { 500, "Internal Server Error"           },
        { 501, "Not Implemented"                 },
        { 502, "Bad Gateway"                     },
        { 503, "Service Unavailable"             },
        { 504, "Gateway Time-out"                },
        { 505, "HTTP Version not supported"      },
    };
}


/*
 * ======= Private: =======
 */

char HTTPResponseParser::Chr() const
{
    return pos_ < content_.size() ? content_[pos_] : 0;
}

std::string HTTPResponseParser::FindNextLine()
{
    auto startPos = pos_;

    while (pos_ < content_.size())
    {
        if (Chr() == '\r')
        {
            ++pos_;

            if (Chr() == '\n')
            {
                ++pos_;
                return content_.substr(startPos, pos_ - startPos - 2);
            }
            else
                break;
        }

        ++pos_;
    }

    return "";
}

std::string HTTPResponseParser::FindNextPart(const std::string& line, size_t& pos)
{
    auto startPos = pos;

    pos = line.find(' ', startPos);

    if (pos != std::string::npos)
    {
        auto part = line.substr(startPos, pos - startPos);
        ++pos;
        return part;
    }

    return startPos < line.size() ? line.substr(startPos) : "";
}

void HTTPResponseParser::ParseGETResponseStatusLine(const std::string& line)
{
    if (line.empty())
        return;

    size_t pos = 0;

    /* Parse HTTP version */
    responseGET_.httpVersion    = FindNextPart(line, pos);
    responseGET_.statusCode     = StrToNum<int>(FindNextPart(line, pos));
    responseGET_.statusMessage  = FindNextPart(line, pos);
}

void HTTPResponseParser::ParseGETResponseHeader(const std::string& line)
{
    /* Parse GET response header entry line ( key ": " value ), e.g. "Server: Apache/1.3.27" */
    auto pos = line.find(": ");

    if (pos != std::string::npos)
    {
        /* Extract key and value */
        auto key = line.substr(0, pos);
        auto value = line.substr(pos + 2);

        responseGET_.headerEntries[key] = value;
    }
}


} // /namespace Network

} // /namespace Fork



// ========================