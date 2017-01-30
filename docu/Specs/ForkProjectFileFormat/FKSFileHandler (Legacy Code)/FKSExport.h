/*
 * FKS export header
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FKS_EXPORT_H__
#define __FKS_EXPORT_H__


#ifdef _MSC_VER
#   define FKS_EXPORT __declspec(dllexport)
#else
#   define FKS_EXPORT
#endif


#endif



// ========================