/*
 * Pack push header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* <--- THIS FILE MUST NOT HAVE A HEADER GUARD! ---> */

#ifdef PACK_STRUCT
#   error PACK_STRUCT already defined: "PackPush.h" included but no succeeding "PackPop.h"!
#endif

#if defined(_MSC_VER)
#   pragma pack(push, packing)
#   pragma pack(1)
#   define PACK_STRUCT
#elif defined(__GNUC__)
#   define PACK_STRUCT __attribute__((packed))
#else
#   error Compiler not supported
#endif



// ========================