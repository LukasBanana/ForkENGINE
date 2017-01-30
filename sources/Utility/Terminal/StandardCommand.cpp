/*
 * Standard command file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Terminal/StandardCommand.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Utility
{


StandardCommand::StandardCommand(
    const std::string& name, const std::string& helpText, const FunctionSignature& function) :
        name_       { name     },
        helpText_   { helpText },
        function_   { function }
{
    ASSERT_POINTER(function);
}

std::string StandardCommand::Name() const
{
    return name_;
}

std::string StandardCommand::Help() const
{
    return helpText_;
}

bool StandardCommand::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    return function_(args, terminal, userData);
}


} // /namespace Utility

} // /namespace Fork



// ========================