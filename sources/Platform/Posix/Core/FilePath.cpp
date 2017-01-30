/*
 * Posix: File path file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/FilePath.h"


namespace Fork
{

namespace Platform
{


FORK_EXPORT bool IsAbsolutePath(const std::string& path)
{
    return path.size() >= 1 && path[0] == '/';
}


} // /namespace Platform

} // /namespace Fork



// ========================