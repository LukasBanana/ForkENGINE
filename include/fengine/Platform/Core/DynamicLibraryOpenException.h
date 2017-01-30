/*
 * Dynamic library open exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DYNAMIC_LIBRARY_OPEN_EXCEPTION_H__
#define __FORK_DYNAMIC_LIBRARY_OPEN_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Dynamic library open exception.
\see Platform::DynamicLibrary
\ingroup fork_exception
*/
class FORK_EXPORT DynamicLibraryOpenException : public DefaultException
{
    
    public:
        
        DynamicLibraryOpenException(const std::wstring& libraryName, const std::string& errorDesc) :
            DefaultException
            {
                "DynamicLibraryOpenException",
                "Description: " + errorDesc
            },
            libraryName_{ libraryName }
        {
        }

        //! Returns the library name.
        inline const std::wstring& GetLibraryName() const
        {
            return libraryName_;
        }

    private:
        
        std::wstring libraryName_;

};


} // /namespace Fork


#endif



// ========================