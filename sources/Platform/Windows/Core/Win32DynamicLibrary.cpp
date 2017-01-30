/*
 * WIN32: Win32 dynamic library file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32DynamicLibrary.h"


namespace Fork
{

namespace Platform
{


Win32DynamicLibrary::Win32DynamicLibrary(std::wstring libraryName) :
    DynamicLibrary{ libraryName }
{
    /* Open library module from DLL file */
    libraryName += L".dll";
    moduleHandle_ = LoadLibrary(libraryName.c_str());

    /* Throw exception if an error occured */
    if (!moduleHandle_)
        throw DynamicLibraryOpenException(libraryName, "Loading dynamic library \"" + ToStr(libraryName) + "\" failed");
}
Win32DynamicLibrary::~Win32DynamicLibrary()
{
    /* Release library module */
    FreeLibrary(moduleHandle_);
}

void* Win32DynamicLibrary::LoadProc(const std::string& procName)
{
    /* Get procedure address from library module and return it as raw-pointer */
    auto procAddr = GetProcAddress(moduleHandle_, procName.c_str());
    return reinterpret_cast<void*>(procAddr);
}


} // /namespace Platform

} // /namespace Fork



// ========================