/*
 * Render system exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_SYSTEM_EXCEPTION_H__
#define __FORK_RENDER_SYSTEM_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Render system exception.
\ingroup fork_exception
*/
class FORK_EXPORT RenderSystemException : public DefaultException
{
    
    public:
        
        RenderSystemException(const std::string& errorDesc) :
            DefaultException
            {
                "RenderSystemException",
                "Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================