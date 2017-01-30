/*
 * Partition container exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PARTITION_CONTAINER_EXCEPTION_H__
#define __FORK_PARTITION_CONTAINER_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Partition container exception.
\see PartitionContainerException
\ingroup fork_exception
*/
class FORK_EXPORT PartitionContainerException : public DefaultException
{
    
    public:
        
        PartitionContainerException(const std::string& procName, const std::string& errorDesc) :
            DefaultException
            {
                "PartitionContainerException",
                "Function: " + procName + ", Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================