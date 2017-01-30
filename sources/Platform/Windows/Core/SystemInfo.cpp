/*
 * WIN32: System info file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Platform/Core/SystemInfo.h"
#include "Platform/Core/OS.h"
#include "Core/StringModifier.h"
#include "Core/STLHelper.h"

#include <Windows.h>
//#include <VersionHelpers.h>


namespace Fork
{

namespace Platform
{


#if defined(FORK_MSVC_COMPILER)
#   pragma warning(disable : 4996) // Warning occurs for deprecated Win32 functions like "GetVersionExW".
#endif

/* === Internal functions === */

static DWORD QueryProcessorSpeed()
{
    /* Open register key */
    HKEY key;
    auto error = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &key);

    if (error != ERROR_SUCCESS)
        return 0;

    /* Query processor speed */
    DWORD speed = 0;
    DWORD size = sizeof(speed);
    error = RegQueryValueExA(key, "~MHz", 0, 0, reinterpret_cast<LPBYTE>(&speed), &size);

    /* Close register key */
    RegCloseKey(key);

    return speed;
}

static void QueryOSVersion(SystemInfo& sysInfo)
{
    sysInfo.osFamily = SystemInfo::OSFamily::Windows;

    /* Initialize OS version info structure */
    OSVERSIONINFOEX versionInfo;
    InitRawMemory(versionInfo);

    versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    const BOOL osVersionInfoEx = GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&versionInfo));

    if (!osVersionInfoEx)
    {
        versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&versionInfo)))
            return;
    }

    switch (versionInfo.dwPlatformId)
    {
        case VER_PLATFORM_WIN32_NT:
        {
            if (versionInfo.dwMajorVersion <= 4)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::WindowsNT;
                sysInfo.osName = "Microsoft Windows NT";
            }
            else if (versionInfo.dwMajorVersion == 5 && versionInfo.dwMinorVersion == 0)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::Windows2000;
                sysInfo.osName = "Microsoft Windows 2000";
            }
            else if (versionInfo.dwMajorVersion == 5 && versionInfo.dwMinorVersion == 1)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::WindowsXP;
                sysInfo.osName = "Microsoft Windows XP";
            }
            else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 0)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::WindowsVista;
                sysInfo.osName = "Microsoft Windows Vista";
            }
            else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 1)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::Windows7;
                sysInfo.osName = "Microsoft Windows 7";
            }
            else if (versionInfo.dwMajorVersion == 6 && versionInfo.dwMinorVersion == 2)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::Windows8;
                sysInfo.osName = "Microsoft Windows 8";
            }

            if (osVersionInfoEx)
            {
                #ifdef VER_SUITE_ENTERPRISE
                if (versionInfo.wProductType == VER_NT_WORKSTATION)
                {
                    if (versionInfo.wSuiteMask & VER_SUITE_PERSONAL)
                        sysInfo.osProductType = "Personal";
                    else
                        sysInfo.osProductType = "Professional";
                }
                else if (versionInfo.wProductType == VER_NT_SERVER)
                {
                    if (versionInfo.wSuiteMask & VER_SUITE_DATACENTER)
                        sysInfo.osProductType = "DataCenter Server";
                    else if (versionInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
                        sysInfo.osProductType = "Advanced Server";
                    else
                        sysInfo.osProductType = "Server";
                }
                #endif
            }
            else
            {
                char szProductType[80];
                DWORD dwBufLen = 0;

                /* Query product type via register key */
                HKEY key;
                RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ProductOptions", 0, KEY_QUERY_VALUE, &key);

                RegQueryValueExA(key, "ProductType", 0, 0, reinterpret_cast<LPBYTE>(szProductType), &dwBufLen);
                RegCloseKey(key);

                /* Compare string case insensitive */
                if (lstrcmpiA("WINNT", szProductType) == 0)
                    sysInfo.osProductType = "Professional";
                if (lstrcmpiA("LANMANNT", szProductType) == 0)
                    sysInfo.osProductType = "Server";
                if (lstrcmpiA("SERVERNT", szProductType) == 0)
                    sysInfo.osProductType = "Advanced Server";
            }

            /* Display version, service pack (if any), and build number. */
            sysInfo.osProductType += " (Version ";
            sysInfo.osProductType += ToStr(versionInfo.dwMajorVersion);
            sysInfo.osProductType += ".";
            sysInfo.osProductType += ToStr(versionInfo.dwMinorVersion);
            sysInfo.osProductType += ")";

            sysInfo.osExtensions = ToStr(versionInfo.szCSDVersion);

            sysInfo.osBuild = "Build " + ToStr(versionInfo.dwBuildNumber & 0xFFFF);
        }
        break;

        case VER_PLATFORM_WIN32_WINDOWS:
        {
            if (versionInfo.dwMajorVersion == 4 && versionInfo.dwMinorVersion == 0)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::Windows95;
                sysInfo.osName = "Microsoft Windows 95";
                if (versionInfo.szCSDVersion[1] == 'C' || versionInfo.szCSDVersion[1] == 'B')
                    sysInfo.osName += " OSR2";
            }

            if (versionInfo.dwMajorVersion == 4 && versionInfo.dwMinorVersion == 10)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::Windows98;
                sysInfo.osName = "Microsoft Windows 98";
                if (versionInfo.szCSDVersion[1] == 'A')
                    sysInfo.osName += " SE";
            }

            if (versionInfo.dwMajorVersion == 4 && versionInfo.dwMinorVersion == 90)
            {
                sysInfo.osFamily = SystemInfo::OSFamily::WindowsMe;
                sysInfo.osName = "Microsoft Windows Me";
            }
        }
        break;

        case VER_PLATFORM_WIN32s:
        {
            sysInfo.osName = "Microsoft Win32s ";
        }
        break;
    }
}

static BOOL IsWoW64()
{
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    BOOL isWoW64 = FALSE;

    auto procIsWoW64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")), "IsWow64Process"
    );

    if (!procIsWoW64Process || !procIsWoW64Process(GetCurrentProcess(), &isWoW64))
        return FALSE;

    return isWoW64;
}

static std::string QueryCompilerVersion()
{
    #if defined(__GNUC__)
    
    return "GCC " + ToStr(__GNUC__) + "." + ToStr(__GNUC_MINOR__) + "." + ToStr(__GNUC_PATCHLEVEL__);
    
    #elif defined(_MSC_VER)
    
    std::string version = "Microsoft ";
    
    #   if _MSC_VER == 600
    version += "C Compiler 6.0";            // C 6.0
    #   elif _MSC_VER == 700
    version += "C/C++ Compiler 7.0";        // C/C++ 7.0
    #   elif _MSC_VER == 800
    version += "Visual C++ 1.0";            // 1.0
    #   elif _MSC_VER == 900
    version += "Visual C++ 2.0";            // 2.0
    #   elif _MSC_VER == 1000
    version += "Visual C++ 4.0";            // 4.0
    #   elif _MSC_VER == 1100
    version += "Visual C++ 5.0";            // 5.0
    #   elif _MSC_VER == 1200
    version += "Visual C++ 6.0";            // 6.0
    #   elif _MSC_VER == 1300
    version += "Visual C++ .NET (7.0)";     // .NET
    #   elif _MSC_VER == 1310
    version += "Visual C++ 2003 (7.1)";     // 2003
    #   elif _MSC_VER == 1400
    version += "Visual C++ 2005 (8.0)";     // 2005
    #   elif _MSC_VER == 1500
    version += "Visual C++ 2008 (9.0)";     // 2008
    #   elif _MSC_VER == 1600
    version += "Visual C++ 2010 (10.0)";    // 2010
    #   elif _MSC_VER == 1700
    version += "Visual C++ 2012 (11.0)";    // 2012
    #   elif _MSC_VER == 1800
    version += "Visual C++ 2013 (12.0)";    // 2013
    #   else
    version += "Visual C++";
    #   endif
    
    return version;
    
    #else
    
    return "Unknown compiler";
    
    #endif
}


/* === SystemInfo structure === */

std::string SystemInfo::OSVersion() const
{
    std::string version;

    version += osName;
    if (!osProductType.empty())
        version += " " + osProductType;
    if (!osExtensions.empty())
        version += " " + osExtensions;
    if (!osBuild.empty())
        version += " (" + osBuild + ")";

    return version;
}

std::string SystemInfo::ToStr(const CPUTypes val)
{
    switch (val)
    {
        case CPUTypes::CPU32Bit:    return "32-Bit";
        case CPUTypes::CPU64Bit:    return "64-Bit";
        case CPUTypes::Unknown:     break;
    }
    return "Unknown";
}

std::string SystemInfo::ToStr(const CPUArchitectures val)
{
    switch (val)
    {
        case CPUArchitectures::IA32:    return "IA-32 (x86)";
        case CPUArchitectures::IA64:    return "IA-64 (Itanium-based)";
        case CPUArchitectures::ARM:     return "ARM";
        case CPUArchitectures::AMD64:   return "AMD64 (x64)";
        case CPUArchitectures::Unknown: break;
    }
    return "Unknown";
}

std::string SystemInfo::ToStr(const OSFamily val)
{
    switch (val)
    {
        case OSFamily::Windows:         return "Windows";
        case OSFamily::Windows95:       return "Windows 95";
        case OSFamily::Windows98:       return "Windows 98";
        case OSFamily::WindowsMe:       return "Windows Me";

        case OSFamily::WindowsNT:       return "Windows NT";
        case OSFamily::Windows2000:     return "Windows 2000";
        case OSFamily::WindowsXP:       return "Windows XP";
        case OSFamily::WindowsVista:    return "Windows Vista";
        case OSFamily::Windows7:        return "Windows 7";
        case OSFamily::Windows8:        return "Windows 8";

        case OSFamily::Posix:           return "Posix";

        case OSFamily::Unknown:         break;
    }
    return "Unkown";
}


/* === Global functions === */

FORK_EXPORT SystemInfo QuerySystemInfo()
{
    SystemInfo info;

    /* Query CPU type */
    info.cpuType = (IsWoW64() ? SystemInfo::CPUTypes::CPU64Bit : SystemInfo::CPUTypes::CPU32Bit);

    //!TODO! -> query correct CPU infos!!!
    /* Default values */
    info.cpuArch = SystemInfo::CPUArchitectures::IA32;

    /* Query common system info */
    SYSTEM_INFO sysInfo;
    ZeroMemory(&sysInfo, sizeof(sysInfo));
    GetNativeSystemInfo(&sysInfo);

    info.numProcessors = static_cast<unsigned int>(sysInfo.dwNumberOfProcessors);

    switch (sysInfo.wProcessorArchitecture)
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            info.cpuArch = SystemInfo::CPUArchitectures::AMD64;
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            info.cpuArch = SystemInfo::CPUArchitectures::ARM;
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            info.cpuArch = SystemInfo::CPUArchitectures::IA64;
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            info.cpuArch = SystemInfo::CPUArchitectures::IA32;
            break;
    }

    /* Query extended system info */
    info.processorSpeed = static_cast<unsigned int>(QueryProcessorSpeed());

    QueryOSVersion(info);

    return info;
}

FORK_EXPORT CompilerInfo QueryCompilerInfo()
{
    CompilerInfo info;

    info.compilerVersion    = QueryCompilerVersion();
    info.latestBuild        = ToStr(__DATE__) + " at " + ToStr(__TIME__);

    #ifdef _DEBUG
    info.hasDebugInfo = true;
    #endif

    return info;
}

FORK_EXPORT std::wstring QueryWorkingDirectory()
{
    WCHAR dirPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, dirPath);
    return dirPath;
}

FORK_EXPORT void ChangeWorkingDirectory(const std::wstring& dirPath)
{
    SetCurrentDirectory(dirPath.c_str());
}


} // /namespace Platform

} // /namespace Fork



// ========================