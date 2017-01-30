/*
 * Standard command "help" file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StandardCommandListing.h"
#include "Utility/Terminal/Terminal.h"


namespace Fork
{

namespace Utility
{


std::string StdCmdHelp::Name() const
{
    return "help";
}

std::string StdCmdHelp::Help() const
{
    return "Prints the command help documentation";
}

bool StdCmdHelp::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (!terminal)
        return false;

    const auto& cmdList = terminal->GetCommandFactory()->GetCommands();

    /* Find longest command identifier */
    size_t len = 0;

    for (const auto& cmd : cmdList)
    {
        if (len < cmd.second->Name().size())
            len = cmd.second->Name().size();
    }

    len += 3;

    /* Print command documentation */
    for (const auto& it : cmdList)
    {
        const auto& cmd = it.second;
        terminal->Message(
            cmd->Name() + std::string(len - cmd->Name().size(), ' ') + cmd->Help()
        );
    }

    return true;
}


} // /namespace Utility

} // /namespace Fork



// ========================