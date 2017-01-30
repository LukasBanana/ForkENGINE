/*
 * Invalid argument exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_INVALID_ARGUMENT_EXCEPTION_H__
#define __FORK_INVALID_ARGUMENT_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Invalid argument exception.
\ingroup fork_exception
*/
class FORK_EXPORT InvalidArgumentException : public DefaultException
{
    
    public:
        
        InvalidArgumentException(
            const std::string& procName, const std::string& paramName, const std::string& errorDesc) :
                DefaultException
                {
                    "InvalidArgumentException",
                    "Function: " + procName + ", Parameter: " + paramName + ", Description: " + errorDesc
                }
        {
        }

};


} // /namespace Fork


#endif



// ========================