/*
 * Not yet implemented exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NOT_YET_IMPLEMENTED_EXCEPTION_H__
#define __FORK_NOT_YET_IMPLEMENTED_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Not yet implemented exception. This occures when "FORK_ENABLE_EXCEPTION_NOTYETIMPLEMENTED"
is defined and a function is called, which is not yet implemented.
\ingroup fork_exception
*/
class FORK_EXPORT NotYetImplementedException : public DefaultException
{
    
    public:
        
        NotYetImplementedException(const std::string& procName) :
            DefaultException
            {
                "NotYetImplementedException",
                "Function: " + procName
            }
        {
        }
        NotYetImplementedException(const std::string& procName, const std::string& errorDesc) :
            DefaultException
            {
                "NotYetImplementedException",
                "Function: " + procName + ", Description: " + errorDesc
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================