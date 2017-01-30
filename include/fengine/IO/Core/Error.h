/*
 * Error header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_ERROR_H__
#define __FORK_IO_ERROR_H__


#include "IO/Core/ErrorTypes.h"
#include "Core/Exception/DefaultException.h"
#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace IO
{


//! Base error container class.
class FORK_EXPORT Error : public DefaultException
{
    
    public:
        
        Error(const ErrorTypes type, const std::string& message);
        virtual ~Error();

        static std::string TypeString(const ErrorTypes type);

        inline ErrorTypes Type() const
        {
            return type_;
        }

    private:

        ErrorTypes type_;
        
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================