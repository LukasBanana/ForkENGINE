/*
 * Constant buffer not found exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CONSTANT_BUFFER_NOT_FOUND_EXCEPTION_H__
#define __FORK_CONSTANT_BUFFER_NOT_FOUND_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Constant buffer not found exception.
\ingroup fork_exception
*/
class FORK_EXPORT ConstantBufferNotFoundException : public DefaultException
{
    
    public:
        
        ConstantBufferNotFoundException(const std::string& errorDesc) :
            DefaultException
            {
                "ConstantBufferNotFoundException",
                "Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================