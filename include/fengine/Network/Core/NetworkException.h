/*
 * Network exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NETWORK_EXCEPTION_H__
#define __FORK_NETWORK_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Network exception (amonst other: when a connection failed).
\ingroup fork_exception
*/
class FORK_EXPORT NetworkException : public DefaultException
{
    
    public:
        
        NetworkException(const std::string& procName, const std::string& errorDesc) :
            DefaultException
            {
                "NetworkException",
                "Function: " + procName + ", Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================