/*
 * Standard call header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STDCALL_H__
#define __FORK_STDCALL_H__


#if defined(_MSC_VER)
#   define FORK_STDCALL __stdcall
#elif defined(__GNUC__) || defined(__llvm__)
#   define FORK_STDCALL __attribute__((stdcall))
#else
#   define FORK_STDCALL
#endif


#endif



// ========================