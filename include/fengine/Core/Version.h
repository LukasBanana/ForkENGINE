/*
 * Version header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_VERSION_H__
#define __FORK_VERSION_H__


#include "Core/Export.h"

#include <string>


namespace Fork
{


/* === Macros === */

#define __FORK_ENGINE__


/* === Global functions === */

//! Returns the engine ID name.
FORK_EXPORT const char* GetEngineIDName();

//! Returns the engine major version.
FORK_EXPORT int GetVersionMajor();
//! Returns the engine minor version.
FORK_EXPORT int GetVersionMinor();
//! Returns the engine version revision.
FORK_EXPORT int GetVersionRevision();
//! Returns the engine status. This is "Alpha", "Beta" or null.
FORK_EXPORT const char* GetVersionStatus();

//! Returns the engine's license information.
FORK_EXPORT const char* GetLicenseInfo();

//! Returns the engine version as string, e.g. "0.1 Rev. 2 Alpha".
FORK_EXPORT std::string EngineVersion();


/* === Structures === */

//! Version number structure which contains four unsigned integers: major, minor, revision and build number.
struct FORK_EXPORT VersionNumber
{
    VersionNumber(unsigned int maj = 1, unsigned int min = 0, unsigned int rev = 0, unsigned int bld = 0);

    unsigned int operator [] (size_t index) const;

    /**
    Converts this version number into a string.
    \param[in] printSimplified Specifies whether the version should be simplified, i.e. only major and minor version are meant to be printed.
    \param[in] minorTenSpot Specifies whether the minor version can have a ten-spot (e.g. false -> "3.2"; true -> "3.02").
    \return This version number converted as string.
    */
    std::string ToString(bool printSimplified = false, bool minorTenSpot = false) const;

    //! Returns true if this version number is valid. If all numbers are zero the return value is false.
    bool IsValid() const;

    unsigned int major;     //!< Major version number. Should start with 1. By default 1.
    unsigned int minor;     //!< Minor version number. Should start with 0. By default 0.
    unsigned int revision;  //!< Revision number. Commonly used for bug-fixes. By default 0.
    unsigned int build;     //!< Build number or rather project compilation counter. By default 0.
};


/* === Global operators === */

bool FORK_EXPORT operator == (const VersionNumber& a, const VersionNumber& b);
bool FORK_EXPORT operator != (const VersionNumber& a, const VersionNumber& b);

bool FORK_EXPORT operator > (const VersionNumber& a, const VersionNumber& b);
bool FORK_EXPORT operator >= (const VersionNumber& a, const VersionNumber& b);

bool FORK_EXPORT operator < (const VersionNumber& a, const VersionNumber& b);
bool FORK_EXPORT operator <= (const VersionNumber& a, const VersionNumber& b);


} // /namespace Fork


#endif



// ========================