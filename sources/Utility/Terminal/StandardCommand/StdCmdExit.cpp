/*
 * Standard command "exit" file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StandardCommandListing.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Utility
{


std::string StdCmdExit::Name() const
{
    return "exit";
}

std::string StdCmdExit::Help() const
{
    return "Exits the entire application (optional argument: [code])";
}

bool StdCmdExit::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (args.size() == 2)
    {
        int returnCode = StrToNum<int>(args.back());
        exit(returnCode);
    }
    else
        exit(0);
    return true;
}


} // /namespace Utility

} // /namespace Fork



// ========================