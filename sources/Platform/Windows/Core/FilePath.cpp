/*
 * WIN32: File path file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/FilePath.h"

#include <cctype>


namespace Fork
{

namespace Platform
{


FORK_EXPORT bool IsAbsolutePath(const std::string& path)
{
    return path.size() >= 2 && std::isalpha(path[0]) != 0 && path[1] == ':';
}


} // /namespace Platform

} // /namespace Fork



// ========================