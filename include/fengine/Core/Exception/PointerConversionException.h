/*
 * Pointer conversion exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POINTER_CONVERSION_EXCEPTION_H__
#define __FORK_POINTER_CONVERSION_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Pointer conversion exception.
\ingroup fork_exception
*/
class FORK_EXPORT PointerConversionException : public DefaultException
{
    
    public:
        
        PointerConversionException(const std::string& procName, const std::string& typeFrom, const std::string& typeTo) :
            DefaultException
            {
                "PointerConversionException",
                "Function: " + procName + ", Type From: " + typeFrom + ", Type To: " + typeTo
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================