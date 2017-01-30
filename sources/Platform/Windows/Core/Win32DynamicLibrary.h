/*
 * WIN32: Win32 dynamic library header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_WIN32_DYNAMIC_LIBRARY_H__
#define __FORK_PLATFORM_WIN32_DYNAMIC_LIBRARY_H__


#include "Platform/Core/DynamicLibrary.h"

#include <Windows.h>


namespace Fork
{

namespace Platform
{


class Win32DynamicLibrary : public DynamicLibrary
{
    
    public:
        
        Win32DynamicLibrary(const Win32DynamicLibrary&) = delete;
        Win32DynamicLibrary& operator = (const Win32DynamicLibrary&) = delete;

        Win32DynamicLibrary(std::wstring libraryName);
        ~Win32DynamicLibrary();

        void* LoadProc(const std::string& procName);

    private:
        
        HMODULE moduleHandle_ = 0;

};


} // /namespace Platform

} // /namespace Fork


#endif



// ========================