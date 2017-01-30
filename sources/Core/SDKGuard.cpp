/*
 * SDK guard file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Core/SDKGuard.h"


extern "C"
{

FORK_EXPORT bool __MatchSDKVersion__(int version)
{
    /*
    Check if the input version has the same value as the 'FORK_SDK_VERSION' macro.
    This will fail, when the client programmer uses a different SDK version,
    or rather a different compiler version.
    */
    return version == FORK_SDK_VERSION;
}

}



// ========================