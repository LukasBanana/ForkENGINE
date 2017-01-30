/*
 * Null pointer exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NULL_POINTER_EXCEPTION_H__
#define __FORK_NULL_POINTER_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Null pointer exception.
\ingroup fork_exception
*/
class FORK_EXPORT NullPointerException : public DefaultException
{
    
    public:
        
        NullPointerException(const std::string& procName, const std::string& paramName) :
            DefaultException
            {
                "NullPointerException",
                "Function: " + procName + ", Parameter: " + paramName
            }
        {
        }

};


/**
Assertion to detect null pointer.
\throws NullPointerException If 'ptr' is null.
*/
#define ASSERT_POINTER(ptr) if ((ptr) == nullptr) throw NullPointerException(__FUNCTION__, #ptr)


} // /namespace Fork


#endif



// ========================