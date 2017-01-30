/*
 * Standard command "font" file
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


std::string StdCmdFont::Name() const
{
    return "font";
}

std::string StdCmdFont::Help() const
{
    return "Changes the terminal font (with args: [name size])";
}

bool StdCmdFont::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (args.empty() || !terminal)
        return false;
    
    if (args.size() == 3)
    {
        /* Create new terminal default font */
        auto defaultFont = terminal->GetRenderer()->CreateTextureFont({ args[1], StrToNum<int>(args[2]) });
        terminal->ChangeDefaultFont(defaultFont);
    }
    else
        terminal->ErrorNumArgs(Name());

    return true;
}


} // /namespace Utility

} // /namespace Fork



// ========================