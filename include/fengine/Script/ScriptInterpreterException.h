/*
 * Script interpreter exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCRIPT_INTERPRETER_EXCEPTION_H__
#define __FORK_SCRIPT_INTERPRETER_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Script interpreter exception.
\ingroup fork_exception
*/
class FORK_EXPORT ScriptInterpreterException : public DefaultException
{
    
    public:
        
        ScriptInterpreterException(const std::string& errorDesc) :
            DefaultException
            {
                "ScriptInterpreterException",
                "Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================