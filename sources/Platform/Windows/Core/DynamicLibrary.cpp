/*
 * WIN32: Dynamic library file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/DynamicLibrary.h"
#include "Win32DynamicLibrary.h"
#include "Core/StaticConfig.h"


namespace Fork
{

namespace Platform
{


DynamicLibrary::DynamicLibrary(const std::wstring& name) :
    name_{ name }
{
}
DynamicLibrary::~DynamicLibrary()
{
}

DynamicLibraryPtr DynamicLibrary::Open(const std::wstring& libraryName, bool appendDebugPostfix)
{
    #ifdef FORK_DEBUG
    if (appendDebugPostfix)
    {
        return std::make_shared<Win32DynamicLibrary>(libraryName + L"D");
    }
    else
    #endif
    {
        return std::make_shared<Win32DynamicLibrary>(libraryName);
    }
}


} // /namespace Platform

} // /namespace Fork



// ========================