/*
 * Command factory header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_COMMAND_FACTORY_H__
#define __FORK_UTILITY_COMMAND_FACTORY_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "IO/Core/CommandLine.h"
#include "Utility/Terminal/Command.h"

#include <string>
#include <vector>
#include <map>


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(CommandFactory);

class Terminal;

//! Terminal command factory base class.
class FORK_EXPORT CommandFactory
{
    
    public:
        
        //! Type of the command list.
        typedef std::map<std::string, CommandPtr> CommandListType;

        /* === Functions === */

        CommandFactory();
        virtual ~CommandFactory();

        //! Establishs (or adds) all standard commands.
        void EstablishStandardCommands();
        
        /**
        Adds a new command to the factory.
        \param[in] cmd Shared pointer to the new command which is to be added.
        \remarks Here is a small example on how to add a standard command:
        \code
        AddCommand(std::make_shared<Util::StandardCommand>(
            "SimpleCommand",
            "Help information for this simple command",
            [](const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData) -> bool
            {
                // execute the command here ...
            }
        ));
        \endcode
        \see Command
        \see StandardCommand
        */
        void AddCommand(const CommandPtr& cmd);
        /**
        Removes the specified command from the factory.
        \see AddCommand
        */
        void RemoveCommand(const std::string& name);
        /**
        Clears the command list. To add all standard commands again, use "EstablishStandardCommands".
        \see EstablishStandardCommands
        */
        void ClearCommands();

        /**
        Executes the specified terminal command.
        \param[in] terminal Raw-pointer to the terminal which will receive some input events (see Terminal::Print).
        \param[in] cmdLine Specifies the command line which is to be executed. Here is a list of all supported commands:
        - "clear" Clears the terminal history.
        - "version" Prints the engine version.
        - "term" Prints information about this terminal's state.
        - "exit" Quits the entire application.
        - "script" Switchtes to the script editing mode (Quit with ESC).
        - "fullscreen" Switches the fullscreen mode.
        - "help" Prints the help information.
        - "bg #a" Changes the background color with the color value alpha.
        - "bg #r #g #b" Changes the background color with the color values red, green and blue.
        - "bg #r #g #b #a" Changes the background color with the color values red, green, blue and alpha.
        \return True if a valid command was entered. Otherwise an error message will be printed into the terminal.
        \see Utility::Terminal
        \see Utility::Terminal::Print
        \throws NullPointerException
        */
        virtual bool ExecuteCommand(
            const IO::CommandLine& cmdLine, Terminal* terminal = nullptr, void* userData = nullptr
        );

        /**
        Returns a list of commands which are suitable for the specified sub command string.
        \code
        // Pseudocode:
        FindSuitableCommands({ "clear", "close", "cancel" }, "cl") -> { "clear", "close" }
        FindSuitableCommands({ "clear", "close", "cancel" }, "claer") -> { }
        FindSuitableCommands({ "system_info", "set_pos", "get_pos" }, "s") -> { "system_info", "set_pos" }
        \endcode
        \see ExecuteCommand
        */
        virtual CommandListType FindSuitableCommands(const CommandListType& cmdList, const std::string& subCmd) const;

        /**
        This is equivalent to:
        \code
        commandFactory->FindSuitableCommands(commandFactory->GetCommands(), subCmd);
        \endcode
        \see FindSuitableCommands(const CommandListType&, const std::string&)
        */
        CommandListType FindSuitableCommands(const std::string& subCmd) const;

        /**
        Returns the common divisor (as command sub name from left-to-right) of the specified command list.
        \code
        // Pseudocode:
        FindCommonDivisor({ "clear", "close" }) -> "cl"
        FindCommonDivisor({ "test", "foo test" }) -> ""
        FindCommonDivisor({ "ABC_abc", "ABc_abc" }) -> "AB"
        \endcode
        */
        std::string FindCommonDivisor(const CommandListType& cmdList) const;

        //! Returns a raw-pointer to the command with the specified name or null if there is no such command.
        virtual Command* FindCommand(const std::string& name);

        //! Returns the command list. This is by default always alphabetically sorted.
        inline const CommandListType& GetCommands() const
        {
            return commands_;
        }

    protected:
        
        //! Prints the specified message to the terminal or to the log output if 'terminal' is null.
        void Message(const std::string& message, Terminal* terminal = nullptr);
        //! Prints the specified error to the terminal or to the log output if 'terminal' is null.
        void Error(const std::string& message, Terminal* terminal = nullptr);

    private:

        CommandListType commands_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================