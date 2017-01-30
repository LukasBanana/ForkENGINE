/*
 * Version file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Core/Version.h"
#include "Core/StringModifier.h"
#include "Core/StaticConfig.h"


namespace Fork
{


/* === Global functions === */

FORK_EXPORT const char* GetEngineIDName()
{
    return "ForkENGINE";
}

FORK_EXPORT int GetVersionMajor()
{
    return 0;
}

FORK_EXPORT int GetVersionMinor()
{
    return 1;
}

FORK_EXPORT int GetVersionRevision()
{
    return 0;
}

FORK_EXPORT const char* GetVersionStatus()
{
    return "Alpha";
}

FORK_EXPORT const char* GetLicenseInfo()
{
    #ifdef FORK_COMMERCIAL_USE
    return "ForkENGINE COMMERCIAL Version -- Copyright (c) 2014 by Lukas Hermanns";
    #else
    return "ForkENGINE NON-COMMERCIAL Version -- Copyright (c) 2014 by Lukas Hermanns";
    #endif
}

FORK_EXPORT std::string EngineVersion()
{
    std::string version;

    version += ToStr(GetVersionMajor());
    version += ".";
    version += ToStr(GetVersionMinor());

    if (GetVersionRevision())
    {
        version += " Rev. ";
        version += ToStr(GetVersionRevision());
    }

    if (GetVersionStatus())
    {
        version += " ";
        version += GetVersionStatus();
    }

    return version;
}


/* === Structures === */

VersionNumber::VersionNumber(
    unsigned int maj, unsigned int min, unsigned int rev, unsigned int bld) :
        major   { maj },
        minor   { min },
        revision{ rev },
        build   { bld }
{
}

unsigned int VersionNumber::operator [] (size_t index) const
{
    switch (index)
    {
        case 0: return major;
        case 1: return minor;
        case 2: return revision;
        case 3: return build;
    }
    return 0;
}

std::string VersionNumber::ToString(bool printSimplified, bool minorTenSpot) const
{
    if (printSimplified)
        return ToStr(major) + "." + (minorTenSpot ? NumberOffset(minor, 99, '0') : ToStr(minor));
    return ToString(true, minorTenSpot) + ", Rev. " + ToStr(revision) + ", Build. " + ToStr(build);
}

bool VersionNumber::IsValid() const
{
    return (major | minor | revision | build) != 0;
}


/* === Global operators === */

bool FORK_EXPORT operator == (const VersionNumber& a, const VersionNumber& b)
{
    return
        a.major     == b.major &&
        a.minor     == b.minor &&
        a.revision  == b.revision &&
        a.build     == b.build;
}

bool FORK_EXPORT operator != (const VersionNumber& a, const VersionNumber& b)
{
    return !(a == b);
}

bool FORK_EXPORT operator > (const VersionNumber& a, const VersionNumber& b)
{
    for (size_t i = 0; i < 4; ++i)
    {
        if (a[i] > b[i])
            return true;
        if (a[i] < b[i])
            return false;
    }
    return false;
}

bool FORK_EXPORT operator >= (const VersionNumber& a, const VersionNumber& b)
{
    return a > b || a == b;
}

bool FORK_EXPORT operator < (const VersionNumber& a, const VersionNumber& b)
{
    for (size_t i = 0; i < 4; ++i)
    {
        if (a[i] > b[i])
            return false;
        if (a[i] < b[i])
            return true;
    }
    return false;
}

bool FORK_EXPORT operator <= (const VersionNumber& a, const VersionNumber& b)
{
    return a < b || a == b;
}


} // /namespace Fork



// ========================