/*
 * System info header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_SYSTEM_INFO_H__
#define __FORK_PLATFORM_SYSTEM_INFO_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{

namespace Platform
{


/* === Classes === */

//! System information structure.
struct FORK_EXPORT SystemInfo
{
    /* === Enumerations === */

    enum class CPUTypes
    {
        Unknown,    //!< Unknown CPU type.
        CPU32Bit,   //!< 32-bit CPU.
        CPU64Bit,   //!< 64-bit CPU.
    };

    enum class CPUArchitectures
    {
        Unknown,    //!< Unknown CPU architecture.
        IA32,       //!< Intel IA-32 (also x86) architecture.
        IA64,       //!< Intel IA-64 (also itanium-based) architecture.
        ARM,        //!< ARM architecture.
        AMD64,      //!< AMD64 (also x64) architecture.
    };

    enum class OSFamily
    {
        Unknown,        //!< Unkown OS family.

        Windows,        //!< Any MS Windows system.
        Windows95,      //!< MS Windows 95.
        Windows98,      //!< MS Windows 98.
        WindowsMe,      //!< MS Windows Millenium.

        WindowsNT,      //!< MS Windows NT.
        Windows2000,    //!< MS Windows 2000.
        WindowsXP,      //!< MS Windows XP.
        WindowsVista,   //!< MS Windows Vista.
        Windows7,       //!< MS Windows 7.
        Windows8,       //!< MS Windows 8.

        Posix,          //!< Any Posix system (e.g. GNU/Linux, UNIX etc.).
    };

    /* === Functions === */

    std::string OSVersion() const;

    static std::string ToStr(const CPUTypes val);
    static std::string ToStr(const CPUArchitectures val);
    static std::string ToStr(const OSFamily val);

    /* === Members === */

    CPUTypes            cpuType         = CPUTypes::Unknown;            //!< CPU type.
    CPUArchitectures    cpuArch         = CPUArchitectures::Unknown;    //!< CPU arhictecture.
    OSFamily            osFamily        = OSFamily::Unknown;            //!< OS family type.
    
    unsigned int        numProcessors   = 0;                            //!< Number of processors or rather CPU cores (virtual cores inclusive).
    unsigned int        processorSpeed  = 0;                            //!< Processor speed (in MHz).

    std::string         osName;                                         //!< Name of the running operating system (e.g. "Microsoft Windows 7").
    std::string         osProductType;                                  //!< OS product type (e.g. "Home Edition").
    std::string         osExtensions;                                   //!< OS extensions (e.g. "Service Pack 1").
    std::string         osBuild;                                        //!< Build number as string (e.g. "Build 1234").

    std::string         compilerVersion;                                //!< Version of the C++ compiler used to compile this engine.
};

//! Compiler information structure
struct FORK_EXPORT CompilerInfo
{
    std::string compilerVersion;        //!< Compiler version used to compile this engine.
    std::string latestBuild;            //!< Latest build date.
    bool        hasDebugInfo = false;   //!< True if the engine was compiled with debug information.
};


/* === Functions === */

//! Returns the system information.
FORK_EXPORT SystemInfo QuerySystemInfo();

//! Returns the compiler information.
FORK_EXPORT CompilerInfo QueryCompilerInfo();

//! Returns the current working directory.
FORK_EXPORT std::wstring QueryWorkingDirectory();
//! Changes the current working directory.
FORK_EXPORT void ChangeWorkingDirectory(const std::wstring& dirPath);


} // /namespace Platform

} // /namespace Fork


#endif



// ========================