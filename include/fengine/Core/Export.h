/*
 * Export header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EXPORT_H__
#define __FORK_EXPORT_H__


#ifdef _MSC_VER
#   define FORK_EXPORT __declspec(dllexport)
#else
#   define FORK_EXPORT
#endif


#endif



// ========================