/*
 * File open exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FILE_OPEN_EXCEPTION_H__
#define __FORK_FILE_OPEN_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
File open exception.
\ingroup fork_exception
*/
class FORK_EXPORT FileOpenException : public DefaultException
{
    
    public:
        
        FileOpenException(
            const std::string& filename, const std::string& errorDesc) :
                DefaultException
                {
                    "FileOpenException",
                    "Filename: " + filename + ", Description: " + errorDesc
                }
        {
        }

};


} // /namespace Fork


#endif



// ========================