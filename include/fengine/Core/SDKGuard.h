/*
 * SDK guard header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SDK_GUARD_H__
#define __FORK_SDK_GUARD_H__


#include "Core/Export.h"

#include <iostream>


#if defined(_MSC_VER)
#   ifdef _DEBUG
#       define FORK_SDK_VERSION (_MSC_VER + 1)
#   else
#       define FORK_SDK_VERSION (_MSC_VER)
#   endif
#elif defined(__GNUC__)
#   define FORK_SDK_VERSION (1)
#else
#   error Compiler not supported!
#endif

#ifndef __FORK_DOXYGEN__

extern "C"
{

/**
Internal function. Do not call this function! Call "MatchSDKVersion" instead.
\see MatchSDKVersion
*/
FORK_EXPORT bool __MatchSDKVersion__(int);

}

#endif

/**
Checks whether the SDK was compiled with the current compiler or not.
If not, the application must be terminated, otherwise the program will run into undefined behaviour.
Use this as follows:
\code
int main()
{
    assert(MatchSDKVersion(), "Wrong compiler!");
    // ...
}
\endcode
Or use the pre-defined macro:
\code
int main()
{
    FORK_MATCH_SDK_VERSION

    // ...
}
\endcode
*/
inline bool MatchSDKVersion()
{
    return __MatchSDKVersion__(FORK_SDK_VERSION);
}

#define FORK_MATCH_SDK_VERSION                                              \
    if (!MatchSDKVersion())                                                 \
    {                                                                       \
        std::cerr << "Wrong SDK version for your compiler!" << std::endl;   \
        return 1;                                                           \
    }


#endif



// ========================