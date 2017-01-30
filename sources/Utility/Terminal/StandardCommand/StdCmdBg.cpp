/*
 * Standard command "bg" file
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


std::string StdCmdBg::Name() const
{
    return "bg";
}

std::string StdCmdBg::Help() const
{
    return "Changes the background color (with args: [a], [r g b] or [r g b a])";
}

bool StdCmdBg::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (args.empty() || !terminal)
        return false;

    auto view = terminal->GetView();

    switch (args.size())
    {
        case 2:
            view->groundColor.a = StrToNum<float>(args[1]);
            break;
        case 4:
            view->groundColor.r = StrToNum<float>(args[1]);
            view->groundColor.g = StrToNum<float>(args[2]);
            view->groundColor.b = StrToNum<float>(args[3]);
            break;
        case 5:
            view->groundColor.r = StrToNum<float>(args[1]);
            view->groundColor.g = StrToNum<float>(args[2]);
            view->groundColor.b = StrToNum<float>(args[3]);
            view->groundColor.a = StrToNum<float>(args[4]);
            break;
        default:
            terminal->ErrorNumArgs(Name());
            return false;
    }

    return true;
}


} // /namespace Utility

} // /namespace Fork



// ========================