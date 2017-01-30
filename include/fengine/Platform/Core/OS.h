/*
 * OS macros header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_OS_MACROS_H__
#define __FORK_OS_MACROS_H__


/* --- Platform Identification --- */

#if defined(_WIN32)
#   define FORK_WINDOWS_PLATFORM
#elif defined(__linux__) || defined(__unix__)
#   define FORK_POSIX_PLATFORM
#elif defined(__APPLE__)
#   include "TargetConditionals.h"
#   if defined(__MACOSX__)//TARGET_OS_MAC)
#       define FORK_OSX_PLATFORM
#   elif defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#       define FORK_IOS_PLATFORM
#   endif
#elif defined(_XBOX)
#   define FORK_XBOX_PLATFORM
#else
#   error Platform not supported!
#endif


/* --- Compiler Identification --- */

#if defined(_MSC_VER)
#   define FORK_MSVC_COMPILER
#elif defined(__GNUC__)
#   define FORK_GCC_COMPILER
#elif defined(__llvm__)
#   define FORK_LLVM_COMPILER
#elif defined(__INTEL_COMPILER)
#   define FORK_INTEL_COMPILER
#else
#   error Compiler not supported!
#endif


/* --- Architecture Environment --- */

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
#   define FORK_ARCH_ENV_64BIT
#else
#   define FORK_ARCH_ENV_32BIT
#endif


/* --- Dynamic library helper macros --- */

#if defined(FORK_WINDOWS_PLATFORM)
#   define DEFINE_PROC_INTERFACE(ret, name, args) typedef ret (__cdecl *name) args
#else
#   define DEFINE_PROC_INTERFACE(ret, name, args) typedef ret (*name) args
#endif


#endif



// ========================