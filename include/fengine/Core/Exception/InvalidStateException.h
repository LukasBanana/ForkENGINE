/*
 * Invalid state exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_INVALID_STATE_EXCEPTION_H__
#define __FORK_INVALID_STATE_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Invalid state exception.
\ingroup fork_exception
*/
class FORK_EXPORT InvalidStateException : public DefaultException
{
    
    public:
        
        InvalidStateException(const std::string& procName, const std::string& errorDesc) :
            DefaultException
            {
                "InvalidStateException",
                "Function: " + procName + ", Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================