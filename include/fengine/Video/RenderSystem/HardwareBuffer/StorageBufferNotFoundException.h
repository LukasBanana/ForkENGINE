/*
 * Storage buffer not found exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STORAGE_BUFFER_NOT_FOUND_EXCEPTION_H__
#define __FORK_STORAGE_BUFFER_NOT_FOUND_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Storage buffer not found exception.
\ingroup fork_exception
*/
class FORK_EXPORT StorageBufferNotFoundException : public DefaultException
{
    
    public:
        
        StorageBufferNotFoundException(const std::string& errorDesc) :
            DefaultException
            {
                "StorageBufferNotFoundException",
                "Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================