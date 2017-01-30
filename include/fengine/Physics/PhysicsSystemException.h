/*
 * Physics system exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_SYSTEM_EXCEPTION_H__
#define __FORK_PHYSICS_SYSTEM_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Physics system exception.
\ingroup fork_exception
*/
class FORK_EXPORT PhysicsSystemException : public DefaultException
{
    
    public:
        
        PhysicsSystemException(const std::string& errorDesc) :
            DefaultException
            {
                "PhysicsSystemException",
                "Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================