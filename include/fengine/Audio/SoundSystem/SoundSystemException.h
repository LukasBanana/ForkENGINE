/*
 * Sound system exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOUND_SYSTEM_EXCEPTION_H__
#define __FORK_SOUND_SYSTEM_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Sound system exception.
\ingroup fork_exception
*/
class FORK_EXPORT SoundSystemException : public DefaultException
{
    
    public:
        
        SoundSystemException(const std::string& errorDesc) :
            DefaultException
            {
                "SoundSystemException",
                "Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================