/*
 * Standard command "term" file
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


std::string StdCmdTerm::Name() const
{
    return "term";
}

std::string StdCmdTerm::Help() const
{
    return "Prints information about this terminal's state";
}

bool StdCmdTerm::Execute(
    const IO::CommandLine::ArgumentListType& args, Terminal* terminal, void* userData)
{
    if (!terminal)
        return false;

    /* Collect terminal status information */
    size_t totalTextLen = 0;

    const auto& entries = terminal->GetModel()->GetEntries();
    const size_t numEntries = entries.size();

    for (const auto& entry : entries)
    {
        switch (entry->Type())
        {
            case TerminalModel::Entry::Types::Text:
                totalTextLen += dynamic_cast<const TerminalModel::TextEntry*>(entry.get())->GetText().size();
                break;

            case TerminalModel::Entry::Types::SourceCode:
                totalTextLen += dynamic_cast<TerminalModel::SourceCodeEntry*>(entry.get())->GetCodeLine().size();
                break;

            default:
                break;
        }
    }

    /* Print collected information */
    terminal->Message("Entries:           " + ToStr(numEntries));
    terminal->Message("Total text length: " + ToStr(totalTextLen));

    return true;
}


} // /namespace Utility

} // /namespace Fork



// ========================