/*
 * Standard command header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_STANDARD_COMMAND_H__
#define __FORK_UTILITY_STANDARD_COMMAND_H__


#include "Utility/Terminal/Command.h"

#include <functional>


namespace Fork
{

namespace Utility
{


/**
Standard command class.
Can be used for easy command instantiation.
*/
class FORK_EXPORT StandardCommand : public Command
{
    
    public:
        
        /**
        Standard command function signature.
        \param[in] args Specifies the argument list (including the command name).
        \param[in] terminal Optional raw-pointer to the terminal utility.
        \param[in] userData Optional custom raw-pointer.
        */
        typedef std::function<bool (const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)> FunctionSignature;

        /**
        Constructs the standard command.
        \param[in] name Specifies the command name.
        \param[in] helpText Specifies the command help text.
        \throws NullPointerException If 'function' is null.
        */
        StandardCommand(
            const std::string& name, const std::string& helpText, const FunctionSignature& function
        );

        std::string Name() const;
        std::string Help() const;

        bool Execute(
            const IO::CommandLine::ArgumentListType& args,
            Terminal* terminal = nullptr, void* userData = nullptr
        );

    private:
        
        FunctionSignature function_;

        std::string name_;
        std::string helpText_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================