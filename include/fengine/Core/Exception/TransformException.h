/*
 * Transform exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TRANFORM_EXCEPTION_H__
#define __FORK_TRANFORM_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Transform exception (also for matrix inversion exceptions).
\ingroup fork_exception
*/
class FORK_EXPORT TransformException : public DefaultException
{
    
    public:
        
        TransformException(const std::string& procName, const std::string& errorDesc) :
            DefaultException
            {
                "TransformException", "Function: " + procName + ", Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================