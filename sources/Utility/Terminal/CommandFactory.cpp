/*
 * Command factory file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Terminal/CommandFactory.h"
#include "Utility/Terminal/Terminal.h"
#include "Utility/Terminal/StandardCommand.h"
#include "StandardCommand/StandardCommandListing.h"
#include "Core/StringModifier.h"
#include "Core/Exception/NullPointerException.h"
#include "Platform/Core/SystemInfo.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "IO/Core/Log.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"

#include <algorithm>


namespace Fork
{

namespace Utility
{


CommandFactory::CommandFactory()
{
    EstablishStandardCommands();
}
CommandFactory::~CommandFactory()
{
}

void CommandFactory::EstablishStandardCommands()
{
    AddCommand(std::make_shared< StdCmdBg      >());
    AddCommand(std::make_shared< StdCmdClear   >());
    AddCommand(std::make_shared< StdCmdExit    >());
    AddCommand(std::make_shared< StdCmdFont    >());
    AddCommand(std::make_shared< StdCmdHelp    >());
    AddCommand(std::make_shared< StdCmdScript  >());
    AddCommand(std::make_shared< StdCmdTerm    >());
    AddCommand(std::make_shared< StdCmdVersion >());
}

void CommandFactory::AddCommand(const CommandPtr& cmd)
{
    if (cmd && !cmd->Name().empty())
        commands_[cmd->Name()] = cmd;
}

void CommandFactory::RemoveCommand(const std::string& name)
{
    auto it = commands_.find(name);
    if (it != commands_.end())
        commands_.erase(it);
}

void CommandFactory::ClearCommands()
{
    commands_.clear();
}

bool CommandFactory::ExecuteCommand(const IO::CommandLine& cmdLine, Terminal* terminal, void* userData)
{
    /* Extract command arguments */
    const auto& args = cmdLine.GetArguments();

    if (args.empty())
    {
        Error("No arguments to execute command", terminal);
        return false;
    }

    /* Get entered command */
    const auto& cmdName = args.front();

    auto cmd = FindCommand(cmdName);

    if (cmd)
    {
        if (args.back() == "--help" || args.back() == "-h")
        {
            /* Print command man page */
            Message(cmd->Help(), terminal);
            return true;
        }
        else
            return cmd->Execute(args, terminal, userData);
    }
    else
        Error("Unknown command: \"" + cmdName + "\"", terminal);

    return false;
}

CommandFactory::CommandListType CommandFactory::FindSuitableCommands(
    const CommandListType& cmdList, const std::string& subCmd) const
{
    CommandListType subCmdList;

    for (const auto& it : cmdList)
    {
        const auto& cmd = it.second;
        if (cmd->Name().size() >= subCmd.size() && cmd->Name().substr(0, subCmd.size()) == subCmd)
            subCmdList.insert(it);
    }

    return subCmdList;
}

CommandFactory::CommandListType CommandFactory::FindSuitableCommands(const std::string& subCmd) const
{
    return FindSuitableCommands(GetCommands(), subCmd);
}

std::string CommandFactory::FindCommonDivisor(const CommandListType& cmdList) const
{
    if (cmdList.empty())
        return "";

    /* Check if there are any empty commands */
    for (const auto& it : cmdList)
    {
        if (it.second->Name().empty())
            return "";
    }

    auto MatchCharAtPos = [&cmdList](size_t n, char chr) -> bool
    {
        /* Check if all commands match the same character 'chr' at the specified position 'n' */
        for (const auto& it : cmdList)
        {
            const auto& cmd = *it.second;
            auto name = cmd.Name();

            if (n >= name.size() || name[n] != chr)
                return false;
        }
        return true;
    };

    /* Search common divisor length */
    auto cmdName = cmdList.begin()->second->Name();

    for (size_t i = 0, n = cmdName.size(); i < n; ++i)
    {
        if (!MatchCharAtPos(i, cmdName[i]))
        {
            /* Return common divisor from first command name */
            return cmdName.substr(0, i);
        }
    }

    /* Return first command name */
    return cmdName;
}

Command* CommandFactory::FindCommand(const std::string& name)
{
    auto it = commands_.find(name);
    return it != commands_.end() ? it->second.get() : nullptr;
}


/*
 * ======= Protected: =======
 */

void CommandFactory::Message(const std::string& message, Terminal* terminal)
{
    if (terminal)
        terminal->Message(message);
    else
        IO::Log::Message(message);
}

void CommandFactory::Error(const std::string& message, Terminal* terminal)
{
    if (terminal)
        terminal->Error(message);
    else
        IO::Log::Error(message);
}


} // /namespace Utility

} // /namespace Fork



// ========================