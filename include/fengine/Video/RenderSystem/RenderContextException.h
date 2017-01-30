/*
 * Render context exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_CONTEXT_EXCEPTION_H__
#define __FORK_RENDER_CONTEXT_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Render context creation exception.
\ingroup fork_exception
*/
class FORK_EXPORT RenderContextException : public DefaultException
{
    
    public:
        
        RenderContextException(const std::string& errorDesc) :
            DefaultException
            {
                "RenderContextException",
                "Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================