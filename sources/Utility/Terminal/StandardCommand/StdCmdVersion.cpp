/*
 * Standard command "version" file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "StandardCommandListing.h"
#include "Utility/Terminal/Terminal.h"
#include "Core/Version.h"


namespace Fork
{

namespace Utility
{


std::string StdCmdVersion::Name() const
{
    return "version";
}

std::string StdCmdVersion::Help() const
{
    return "Prints the engine's version";
}

bool StdCmdVersion::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (terminal)
    {
        terminal->Message(std::string(GetEngineIDName()) + " Version " + EngineVersion());
        return true;
    }
    return false;
}


} // /namespace Utility

} // /namespace Fork



// ========================