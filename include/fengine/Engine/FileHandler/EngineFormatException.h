/*
 * Engine format exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_FORMAT_EXCEPTION_H__
#define __FORK_ENGINE_FORMAT_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Engine format exception.
\ingroup fork_exception
*/
class FORK_EXPORT EngineFormatException : public DefaultException
{
    
    public:
        
        EngineFormatException(
            const std::string& procName, const std::string& errorDesc) :
                DefaultException
                {
                    "EngineFormatException",
                    "Function: " + procName + ", Description: " + errorDesc
                }
        {
        }

};


} // /namespace Fork


#endif



// ========================