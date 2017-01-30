/*
 * Command header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_COMMAND_H__
#define __FORK_UTILITY_COMMAND_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "IO/Core/CommandLine.h"

#include <string>
#include <vector>


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(Command);

class Terminal;

/**
Terminal command interface.
This class provides a simple interface for all terminal commands.
*/
class FORK_EXPORT Command
{
    
    public:
        
        virtual ~Command();

        //! Returns the command name.
        virtual std::string Name() const = 0;

        //! Returns a help text for this command.
        virtual std::string Help() const = 0;

        //! Returns all flags for this command (e.g. { "-f", "-verbose", "on", "off" }).
        virtual std::vector<std::string> Flags() const;

        /**
        Returns a list of flags which are suitable for the specified sub flag string.
        This is similar to 'TerminalCommandExecutor::FindSuitableCommands'.
        \see TerminalCommandExecutor::FindSuitableCommands
        */
        virtual std::vector<std::string> FindSuitableFlags(const std::string& subFlag) const;

        /**
        Executes this command.
        \param[in] args Specifies the arguments from the command line.
        The first argument must always be the command name itself.
        \param[in] userData Raw pointer to custom user data.
        \return True if the command could be executed correctly.
        Otherwise an error occured and is printed to the log output.
        \see CommandLine::GetArguments
        */
        virtual bool Execute(
            const IO::CommandLine::ArgumentListType& args,
            Terminal* terminal = nullptr, void* userData = nullptr
        ) = 0;

        /**
        Comparision operator for SWO (Strict Weak Ordering) sorting.
        This is the following relation: ( this->Name() < other.Name() );
        */
        bool operator < (const Command& other) const;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================