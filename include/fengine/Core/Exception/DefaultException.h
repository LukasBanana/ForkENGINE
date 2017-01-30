/*
 * Default exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DEFAULT_EXCEPTION_H__
#define __FORK_DEFAULT_EXCEPTION_H__


#include "Core/StaticConfig.h"
#include "Core/StringModifier.h"
#include "Core/Export.h"

#include <exception>
#include <string>


namespace Fork
{


//! Base class for all exceptions.
class FORK_EXPORT DefaultException : public std::exception
{
    
    public:
        
        DefaultException(const std::string& name, const std::string& message) :
            name_   { name    },
            message_{ message }
        {
        }
        virtual ~DefaultException()
        {
        }

        const char* what() const throw() override
        {
            return message_.c_str();
        }

        //! Returns the exception name.
        inline const std::string& GetName() const
        {
            return name_;
        }

        //! Returns the exception error message.
        inline const std::string& GetMessage() const
        {
            return message_;
        }

        /**
        Returns the entire exception message (including the exception name).
        \see GetName
        \see GetMessage
        */
        inline std::string FullMessage() const
        {
            return "[" + GetName() + "] " + GetMessage();
        }

    private:
        
        std::string name_;      //!< Exception name.
        std::string message_;   //!< Error message.

};


} // /namespace Fork


#endif



// ========================