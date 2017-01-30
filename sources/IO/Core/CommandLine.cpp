/*
 * Command line file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/CommandLine.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace IO
{


CommandLine::CommandLine(const std::string& commandLine)
{
    /* Parse argument list from command line */
    size_t prevPos = 0, pos = 0;

    bool beginArg = false;
    bool beginQuote = false;

    for (size_t len = commandLine.size(); pos < len; ++pos)
    {
        const auto& chr = commandLine[pos];

        if (chr == '\"')
        {
            if (beginQuote)
            {
                /* Add new quoted argument */
                arguments_.push_back(commandLine.substr(prevPos, pos - prevPos));
                prevPos = pos;
                beginQuote = false;
            }
            else
            {
                beginQuote = true;
                prevPos = pos + 1;
            }
        }
        else if (!beginQuote)
        {
            if (chr == ' ')
            {
                if (beginArg)
                {
                    /* Add new argument */
                    arguments_.push_back(commandLine.substr(prevPos, pos - prevPos));
                    beginArg = false;
                }
            }
            else
            {
                if (!beginArg)
                    prevPos = pos;
                beginArg = true;
            }
        }
    }

    if (beginArg)
        arguments_.push_back(commandLine.substr(prevPos));
}
CommandLine::CommandLine(const char* commandLine) :
    CommandLine{ std::string(commandLine) }
{
}
CommandLine::CommandLine(const CommandLine&& other) :
    arguments_{ std::move(other.arguments_) }
{
}

std::string CommandLine::Name() const
{
    return !arguments_.empty() ? arguments_.front() : "";
}

CommandLine::operator std::string () const
{
    /* Concatenate all arguments and separate them with a space character */
    std::string commandLine;
    size_t num = arguments_.size();

    for (const auto& arg : arguments_)
    {
        if (HasWhiteSpaces(arg))
            commandLine += "\"" + arg + "\"";
        else
            commandLine += arg;

        if (num > 1)
        {
            commandLine += ' ';
            --num;
        }
    }

    return commandLine;
}

CommandLine::ArgumentListType CommandLine::ToList(int argc, char* argv[])
{
    ArgumentListType args;

    if (argc > 0)
    {
        args.resize(static_cast<size_t>(argc));

        for (int i = 0; i < argc; ++i)
            args[i] = std::string(argv[i]);
    }

    return args;
}


} // /namespace IO

} // /namespace Fork



// ========================