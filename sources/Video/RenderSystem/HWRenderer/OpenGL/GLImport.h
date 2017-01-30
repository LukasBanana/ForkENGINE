/*
 * OpenGL import header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_IMPORT_H__
#define __FORK_GL_IMPORT_H__


#include "Platform/Core/OS.h"

#if defined(FORK_WINDOWS_PLATFORM)
#   include <Windows.h>
#elif defined(FORK_POSIX_PLATFORM)
#   include <sys/utsname.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>

#if defined(FORK_WINDOWS_PLATFORM)
#   include <GL/wglext.h>
#elif defined(FORK_POSIX_PLATFORM)
#   include <GL/glx.h>
#endif


#endif



// ========================