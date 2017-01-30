/*
 * File path header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_FILEPATH_H__
#define __FORK_PLATFORM_FILEPATH_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Platform
{


/**
Returns true if the specified path is an absolute path.
\remarks On Win32 this is true when 'path' begins with a big letter and a colon (e.g. "C:/Program Files/" or "D:\\Backup").
On Posix this is true when 'path' begins with '/' (e.g. "/home/").
*/
FORK_EXPORT bool IsAbsolutePath(const std::string& path);


} // /namespace Platform

} // /namespace Fork


#endif



// ========================