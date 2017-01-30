/*
 * New line header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_NEWLINE_H__
#define __FORK_PLATFORM_NEWLINE_H__


#include "Core/Export.h"


namespace Fork
{

namespace Platform
{


/**
Returns the platform dependent 'new-line' character sequence.
- Win32: "\r\n"
- Posix and OSX: "\n"
*/
FORK_EXPORT const char* NLSeq();


} // /namespace Platform

} // /namespace Fork


#endif



// ========================