/*
 * Standard command "script" file
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


std::string StdCmdScript::Name() const
{
    return "script";
}

std::string StdCmdScript::Help() const
{
    return "Changes the use mode to scripting (Quit with ESC)";
}

bool StdCmdScript::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (terminal)
    {
        terminal->GetModel()->mode = TerminalModel::Modes::Script;
        return true;
    }
    return false;
}


} // /namespace Utility

} // /namespace Fork



// ========================