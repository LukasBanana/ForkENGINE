/*
 * Stride mismatch exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STRIDE_MISMATCH_EXCEPTION_H__
#define __FORK_STRIDE_MISMATCH_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Stride buffer mismatch exception.
\see StrideBuffer
\ingroup fork_exception
*/
class FORK_EXPORT StrideMismatchException : public DefaultException
{
    
    public:
        
        StrideMismatchException(const std::string& procName, const size_t stride) :
            DefaultException
            {
                "StrideMismatchException",
                "Function: " + procName + ", Stride: " + ToStr(stride)
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================