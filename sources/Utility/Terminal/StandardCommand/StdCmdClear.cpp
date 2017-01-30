/*
 * Standard command "clear" file
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


std::string StdCmdClear::Name() const
{
    return "clear";
}

std::string StdCmdClear::Help() const
{
    return "Clears the terminal history";
}

bool StdCmdClear::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (terminal)
    {
        terminal->GetModel()->ClearEntries();
        return true;
    }
    return false;
}


} // /namespace Utility

} // /namespace Fork



// ========================