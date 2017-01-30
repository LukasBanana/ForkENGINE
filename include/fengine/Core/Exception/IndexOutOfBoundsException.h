/*
 * Index out of bounds exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_INDEX_OUT_OF_BOUNDS_EXCEPTION_H__
#define __FORK_INDEX_OUT_OF_BOUNDS_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Index out of bounds exception.
\ingroup fork_exception
*/
class FORK_EXPORT IndexOutOfBoundsException : public DefaultException
{
    
    public:
        
        IndexOutOfBoundsException(const std::string& procName, const size_t index) :
            DefaultException
            {
                "IndexOutOfBoundsException",
                "Function: " + procName + ", Index: " + ToStr(index)
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================