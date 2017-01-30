/*
 * FKS version header
 * 
 * This file is part of the "FKS-FileHandler" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FKS_VERSION_H__
#define __FKS_VERSION_H__


#ifndef __cplusplus
#   error The FKS-FileHandler must be compiled with C++11
#endif

#define __FKS_FORMAT_VERSION__                  100
#define __FKS_LAST_COMPATIBLE_FORMAT_VERSION__  100

#define __FKS_FORMAT_MAGIC_NUMBER__             (*(const UInt32*)"fksf")


#endif



// ========================