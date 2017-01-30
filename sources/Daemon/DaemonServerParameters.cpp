/*
 * Daemon parameters file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "DaemonServerParameters.h"
#include "IO/Core/Log.h"
#include "Core/StringModifier.h"
#include "Plugins/cpplibext/range_iterator.hpp"

#include <functional>
#include <map>


namespace Fork
{

namespace Daemon
{


/* 
 * Internal members
 */

static Configuration appConfig;

static void ShowInfo()
{
    IO::Log::Message("ForkDaemonServer: No input");
    IO::Log::Message("Enter 'ForkDaemon help' for information");
}

static void ShowHelp()
{
    IO::Log::Message("Usage:");
    {
        IO::Log::ScopedIndent indent;
        IO::Log::Message("ForkDaemonServer [Options]");
    }
    IO::Log::Message("Options:");
    {
        IO::Log::ScopedIndent indent;
        IO::Log::ProgramOptions(
            {
                { "server:run",                 "Runs the daemon server."                                                           },
                { "verbose",                    "Prints all client/server communications to standard output."                       },
                { "verbose:logfile FILE",       "Prints all client/server communications to standard output and the log file."      },
                { "verbose:logfile FILE.html",  "Prints all client/server communications to standard output and the HTML log file." },
                { "singleton",                  "This daemon instance only communicates with a single client."                      },
                { "print-colors",               "Prints the output with more color highlighting."                                   },
                { "port N",                     "Overwrite default port by number 'N'."                                             },
                { "update-rate N",              "Overwrite default network packet update rate by number 'N'."                       }
            }
        );
    }
}


/*
 * Global functions
 */

void ParseAppArguments(const IO::CommandLine::ArgumentListType& arguments)
{
    typedef ext::const_range_forward_iterator<IO::CommandLine::ArgumentListType> ArgIterator;

    /* Setup program parameters */
    std::map<std::string, std::function<void (ArgIterator& argIt)>> parameters
    {
        { "help",           [](ArgIterator& argIt) { appConfig.showHelp         = true; } },
        { "server:run",     [](ArgIterator& argIt) { appConfig.runServer        = true; } },
        { "verbose",        [](ArgIterator& argIt) { appConfig.isVerbose        = true; } },
        { "singleton",      [](ArgIterator& argIt) { appConfig.isSingleton      = true; } },
        { "print-colors",   [](ArgIterator& argIt) { appConfig.printColors      = true; } },
        {
            "verbose:logfile",
            [](ArgIterator& argIt)
            {
                appConfig.isVerbose = true;
                if (argIt.has_next())
                {
                    ++argIt;
                    appConfig.logFilename = *argIt;
                }
                else
                    throw std::invalid_argument("Expected filename after \"verbose:logfile\" parameter");
            }
        },
        {
            "port",
            [](ArgIterator& argIt)
            {
                if (argIt.has_next())
                {
                    ++argIt;
                    appConfig.port = StrToNum<unsigned short>(*argIt);
                }
                else
                    throw std::invalid_argument("Expected number after \"port\" parameter");
            }
        },
        {
            "update-rate",
            [](ArgIterator& argIt)
            {
                if (argIt.has_next())
                {
                    ++argIt;
                    appConfig.updateRate = StrToNum<unsigned short>(*argIt);
                }
                else
                    throw std::invalid_argument("Expected number after \"update-rate\" parameter");
            }
        }
    };

    /* Parse program arguments */
    try
    {
        for (ArgIterator it { arguments }; !it.reached_end(); ++it)
        {
            /* Map argument to parameter */
            auto itParam = parameters.find(*it);
            if (itParam != parameters.end())
                itParam->second(it);
            else
                IO::Log::Error("Unknown program parameter \"" + *it + "\"");
        }
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
    }

    /* Evaluate program arguments */
    if (appConfig.showHelp)
        ShowHelp();
    else if (!appConfig.runServer)
        ShowInfo();
}

const Configuration& GetAppConfig()
{
    return appConfig;
}


} // /namespace Daemon

} // /namespace Fork



// ========================