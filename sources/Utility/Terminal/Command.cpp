/*
 * Command file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Terminal/Command.h"


namespace Fork
{

namespace Utility
{


Command::~Command()
{
}

std::vector<std::string> Command::Flags() const
{
    return {};
}

std::vector<std::string> Command::FindSuitableFlags(const std::string& subFlag) const
{
    std::vector<std::string> flagList;

    for (const auto& flag : Flags())
    {
        if (flag.size() >= subFlag.size() && flag.substr(0, flag.size()) == subFlag)
            flagList.push_back(flag);
    }

    return flagList;
}

bool Command::operator < (const Command& other) const
{
    return Name() < other.Name();
}


} // /namespace Utility

} // /namespace Fork



// ========================