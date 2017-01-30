/*
 * Daemon parameters header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DAEMON_PARAMETERS_H__
#define __FORK_DAEMON_PARAMETERS_H__


#include "IO/Core/CommandLine.h"
#include "Network/System/Daemon/PacketEnums.h"


namespace Fork
{

namespace Daemon
{


/**
Daemon application configuration structure (only configurable via program arguments!).
\see ParseParameters
*/
struct Configuration
{
    bool            showHelp    = false;
    bool            runServer   = false;
    bool            isVerbose   = false;
    bool            isSingleton = false;
    bool            printColors = false;                                        //!< Prints the output with more color highlighting.
    unsigned short  port        = Network::DaemonInterface::defaultDaemonPort;
    unsigned short  updateRate  = 10;                                           //!< Network packet update rate (in milliseconds). 0 means no sleep time.
    std::string     logFilename;
};


/**
Parses the specified program arguments.
\remarks Program arguments:
- "verbose" -> Prints all client/server communications to standard output.
- "singleton" -> This daemon instance only communicates with a single client.
- "port N" -> Overwrite default port by number 'N'.
\see GetConfig
*/
void ParseAppArguments(const IO::CommandLine::ArgumentListType& arguments);

/**
Returns a constant reference to the daemon application configuration.
\remarks This is only configurable via program arguments!
\see ParseParameters
*/
const Configuration& GetAppConfig();


} // /namespace Daemon

} // /namespace Fork


#endif



// ========================