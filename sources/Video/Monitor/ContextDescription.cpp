/*
 * Context description header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Monitor/ContextDescription.h"

#include <algorithm>


namespace Fork
{

namespace Video
{


/* --- VSyncDescription --- */

VSyncDescription::VSyncDescription(bool vsEnabled, unsigned int vsRefreshRate, unsigned int vsInterval) :
    isEnabled   { vsEnabled     },
    refreshRate { vsRefreshRate },
    interval    { vsInterval    }
{
}

unsigned int VSyncDescription::ActualInternal() const
{
    return isEnabled ? interval : 0u;
}

FORK_EXPORT bool operator == (const VSyncDescription& lhs, const VSyncDescription& rhs)
{
    return
        lhs.isEnabled   == rhs.isEnabled    &&
        lhs.refreshRate == rhs.refreshRate  &&
        lhs.interval    == rhs.interval;
}

FORK_EXPORT bool operator != (const VSyncDescription& lhs, const VSyncDescription& rhs)
{
    return !(rhs == lhs);
}


/* --- AntiAliasingDescription --- */

AntiAliasingDescription::AntiAliasingDescription(bool aaEnabled, unsigned int aaMultiSamples) :
    isEnabled   { aaEnabled      },
    multiSamples{ aaMultiSamples }
{
}

unsigned int AntiAliasingDescription::ActualSamples() const
{
    return isEnabled ? std::max(1u, multiSamples) : 1u;
}

FORK_EXPORT bool operator == (const AntiAliasingDescription& lhs, const AntiAliasingDescription& rhs)
{
    return
        lhs.isEnabled       == rhs.isEnabled &&
        lhs.multiSamples    == rhs.multiSamples;
}

FORK_EXPORT bool operator != (const AntiAliasingDescription& lhs, const AntiAliasingDescription& rhs)
{
    return !(rhs == lhs);
}


/* --- RendererProfileDescription --- */

RendererProfileDescription::RendererProfileDescription(
    bool rpUseExtProfile, bool rpUseGLCoreProfile, const VersionNumber& rpVersionGL) :
    useExtProfile   { rpUseExtProfile           },
    useGLCoreProfile{ rpUseGLCoreProfile        },
    versionGL       { rpVersionGL               },
    featureLevelD3D { D3DFeatureLevels::Default }
{
}
RendererProfileDescription::RendererProfileDescription(const D3DFeatureLevels rpFeatureLevelD3D) :
    useExtProfile   { false             },
    useGLCoreProfile{ false             },
    versionGL       { 0, 0, 0, 0        },
    featureLevelD3D { rpFeatureLevelD3D }
{
}

bool RendererProfileDescription::HasGLCoreProfile() const
{
    return useExtProfile && useGLCoreProfile;
}


/* --- ContextDescription --- */

ContextDescription::ContextDescription(bool enableVSync) :
    vsyncDesc{ enableVSync }
{
}
ContextDescription::ContextDescription(bool enableVSync, bool enableAntiAliasing, unsigned int multiSamples) :
    vsyncDesc       { enableVSync                      },
    antiAliasingDesc{ enableAntiAliasing, multiSamples }
{
}


/* --- Global functions --- */

FORK_EXPORT std::string FeatureLevelToStr(const D3DFeatureLevels featureLevel)
{
    switch (featureLevel)
    {
        case D3DFeatureLevels::Default:
            return "Direct3D";
        case D3DFeatureLevels::Level_9_1:
            return "Direct3D 9.0a";
        case D3DFeatureLevels::Level_9_2:
            return "Direct3D 9.0b";
        case D3DFeatureLevels::Level_9_3:
            return "Direct3D 9.0c";
        case D3DFeatureLevels::Level_10_0:
            return "Direct3D 10.0";
        case D3DFeatureLevels::Level_10_1:
            return "Direct3D 10.1";
        case D3DFeatureLevels::Level_11_0:
            return "Direct3D 11.0";
        case D3DFeatureLevels::Level_11_1:
            return "Direct3D 11.1";
        case D3DFeatureLevels::Level_11_2:
            return "Direct3D 11.2";
    }
    return "";
}


} // /namespace Video

} // /namespace Fork



// ========================