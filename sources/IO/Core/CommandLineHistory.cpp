/*
 * Command line history file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/Core/CommandLineHistory.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace IO
{


void CommandLineHistory::AddCommandLine(const std::string& commandLine)
{
    auto ContainsOnlyWhiteSpaces = [](const std::string& str)
    {
        for (const auto& chr : str)
        {
            if (!IsWhiteSpace(chr))
                return false;
        }
        return true;
    };

    /* Check if command line is empty or consists only of white spaces */
    if (commandLine.empty() || ContainsOnlyWhiteSpaces(commandLine))
        return;

    /* Check if previous command line in history equals the new command line */
    if (commandLineList_.empty() || commandLineList_.back() != commandLine)
    {
        /* Add new command line to the list */
        commandLineList_.push_back(commandLine);

        /* Remove front entry if the limit is exceeded */
        if (historyLimit > 0 && commandLineList_.size() > historyLimit)
            commandLineList_.erase(commandLineList_.begin());
    }

    ResetIterator();
}

std::string CommandLineHistory::CurrentCommandLine()
{
    return !commandLineList_.empty() && commandLineIt_ != commandLineList_.end() ?
        *commandLineIt_ :
        "";
}

bool CommandLineHistory::Prev()
{
    if (!commandLineList_.empty() && commandLineIt_ != commandLineList_.begin())
    {
        --commandLineIt_;
        return true;
    }
    return false;
}

bool CommandLineHistory::Next()
{
    if (!commandLineList_.empty() && commandLineIt_ != commandLineList_.end())
    {
        ++commandLineIt_;
        return true;
    }
    return false;
}

void CommandLineHistory::Clear()
{
    /* Clear the list and reset the iterator */
    commandLineList_.clear();
    ResetIterator();
}

void CommandLineHistory::ResetIterator()
{
    commandLineIt_ = commandLineList_.end();
}


} // /namespace IO

} // /namespace Fork



// ========================