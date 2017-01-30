/*
 * Pack pop header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

/* <--- THIS FILE MUST NOT HAVE A HEADER GUARD! ---> */

#ifndef PACK_STRUCT
#   error PACK_STRUCT undefined: "PackPop.h" included but no preceding "PackPush.h"!
#endif

#ifdef _MSC_VER
#   pragma pack(pop, packing)
#endif

#undef PACK_STRUCT



// ========================