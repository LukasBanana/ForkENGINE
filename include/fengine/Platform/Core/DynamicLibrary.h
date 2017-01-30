/*
 * Dynamic library header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_DYNAMIC_LIBRARY_H__
#define __FORK_PLATFORM_DYNAMIC_LIBRARY_H__


#include "Core/Export.h"
#include "Platform/Core/DynamicLibraryOpenException.h"

#include <string>
#include <memory>


namespace Fork
{

namespace Platform
{


class DynamicLibrary;

typedef std::shared_ptr<DynamicLibrary> DynamicLibraryPtr;

//! Dynamic library interface.
class FORK_EXPORT DynamicLibrary
{
    
    public:
        
        virtual ~DynamicLibrary();

        /**
        Opens a new dynamic libary.
        \param[in] libraryName Specifies the dynamic library (also called "shared library") name.
        This is not OS dependent. Internally the name will be extended for the OS specific dynamic library file extension.
        \param[in] appendDebugPostfix Specifies whether the debug postix is to be appended or not.
        This has no effect if the engine was compiled in release mode. The postfix string is "D".
        \remarks On MS/Windows, the library name will be extended to 'libraryName + ".dll"'
        and on Posix systems it will be extended to 'libraryName + ".so"'.
        \remarks When compiling in debug mode, by default the postfix string "D" will be appended,
        e.g. on MS/Windows, instead of "ForkRendererGL.dll", "ForkRendererGLD.dll" will be loaded.
        \throws DynamicLibraryOpenException If opening the library failed.
        */
        static DynamicLibraryPtr Open(const std::wstring& libraryName, bool appendDebugPostfix = true);

        /**
        Loads a procedure from this dynamic library.
        \return Raw-pointer to the loaded function object or null if the function could not be loaded.
        Cast the return value to your the respective function interface. Use the "DEFINE_PROC_INTERFACE"
        helper macro (defined in "Platform/Core/OS.h" file) to define the procedure interface.
        \code
        // Example function "int MyTestProc(float, int)":
        DEFINE_PROC_INTERFACE(int, PFNMYTESTPROC, (float, int));
        PFNMYTESTPROC TestProcAddr = reinterpret_cast<PFNMYTESTPROC>(dynamicLib->LoadProc("MyTestProc"));
        int result = TestProcAddr(anyFloatValue, anyIntValue);
        \endcode
        \note You can only use the loaded procedures while this library exists!
        */
        virtual void* LoadProc(const std::string& procName) = 0;

        //! Returns the library name.
        inline const std::wstring& GetName() const
        {
            return name_;
        }

    protected:
        
        DynamicLibrary(const std::wstring& name);

    private:
        
        std::wstring name_;

};


} // /namespace Platform

} // /namespace Fork


#endif



// ========================