/*
 * Context description header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CONTEXT_DESCRIPTION_H__
#define __FORK_CONTEXT_DESCRIPTION_H__


#include "Core/Export.h"
#include "Core/Version.h"

#include <string>


namespace Fork
{

namespace Video
{


/* --- Enumerations --- */

/**
Direct3D feature levels.
\see FeatureLevelToStr*/
enum class D3DFeatureLevels
{
    Default,    //!< Default Direct3D 11 feature level. This will use highest available feature level.
    Level_9_1,  //!< Direct3D 9.1 will be used with shader model 2.
    Level_9_2,  //!< Direct3D 9.2 will be used with shader model 2.
    Level_9_3,  //!< Direct3D 9.3 will be used with shader model 3.
    Level_10_0, //!< Direct3D 10.0 will be used with shader model 4.
    Level_10_1, //!< Direct3D 10.1 will be used with shader model 4.
    Level_11_0, //!< Direct3D 11.0 will be used with shader model 5.
    Level_11_1, //!< Direct3D 11.1 for Windows 8.0 and later (Not supported yet).
    Level_11_2, //!< Direct3D 11.2 for Windows 8.1 and later (Not supported yet).
};

/* --- Structures --- */

//! Vertical-synchronisation description structure.
struct FORK_EXPORT VSyncDescription
{
    VSyncDescription(
        bool vsEnabled = true, unsigned int vsRefreshRate = 60, unsigned int vsInterval = 1
    );

    //! Returns the actual VSync interval.
    unsigned int ActualInternal() const;

    bool            isEnabled   = true; //!< Specifies whether vertical-synchronisation is to be enabled or disabled. By default enabled.
    unsigned int    refreshRate = 60;   //!< Specifies the refresh rate (in Hz). By default 60.
    unsigned int    interval    = 1;    //!< Specifies the synchronisation interval. Can be 1, 2, 3 or 4. By default 1. If VSync is disabled the interval is automatically zero.
};

FORK_EXPORT bool operator == (const VSyncDescription& lhs, const VSyncDescription& rhs);
FORK_EXPORT bool operator != (const VSyncDescription& lhs, const VSyncDescription& rhs);


//! Anti-aliasing description structure.
struct FORK_EXPORT AntiAliasingDescription
{
    AntiAliasingDescription(bool aaEnabled = false, unsigned int aaMultiSamples = 4);

    //! Returns the actual number of samples. This is at least 1.
    unsigned int ActualSamples() const;

    bool            isEnabled       = false;    //!< Enables or disables the anti-aliasing flag.
    unsigned int    multiSamples    = 4;        //!< Specifies the number of multi-samples. By default 4.
};

FORK_EXPORT bool operator == (const AntiAliasingDescription& lhs, const AntiAliasingDescription& rhs);
FORK_EXPORT bool operator != (const AntiAliasingDescription& lhs, const AntiAliasingDescription& rhs);


//! Renderer profile description structure.
struct FORK_EXPORT RendererProfileDescription
{
    RendererProfileDescription(
        bool rpUseExtProfile = false, bool rpUseGLCoreProfile = false,
        const VersionNumber& rpVersionGL = VersionNumber(0, 0, 0, 0)
    );
    RendererProfileDescription(const D3DFeatureLevels rpFeatureLevelD3D);

    //! Returns true if 'useExtProfile' and 'useGLCoreProfile' are true.
    bool HasGLCoreProfile() const;

    /**
    Specifies whether the hardware renderer will produce debug dump at start up. By default false.
    \remarks This setting can be changed dynamically with "RenderSystem::EnableDebugDump".
    \note This feature is only available in debug mode!
    \note The output of the debug dump is totally renderer and driver implementation dependent.
    */
    bool enableDebugDump = false;

    //! Specifies whether an extended renderer profile is to be used. By default false.
    bool useExtProfile = false;
    
    /**
    Specifies whether the OpenGL "Core Profile" or the "Compatibility Profile" is to be used.
    This requires "UseExtProfile" to be true. By default false.
    */
    bool useGLCoreProfile = false;

    /**
    Specifies the OpenGL renderer version. If "useGLCoreProfile" is false, this member will be ignored.
    Only the major and minor version will be used to select an OpenGL profile. By default the latest supported OpenGL is used.
    The last OpenGL version which can be selected with this feature is version 3.2.
    \see VersionNumber
    \see http://www.opengl.org/registry/specs/ARB/wgl_create_context.txt
    */
    VersionNumber versionGL;
    
    /**
    Specifies the Direct3D 11 renderer feature level. This can be used for shader
    backwards compatibility. By default D3DFeatureLevels::Default.
    \see D3DFeatureLevels
    */
    D3DFeatureLevels featureLevelD3D = D3DFeatureLevels::Default;
};


//! Render context description settings.
struct FORK_EXPORT ContextDescription
{
    ContextDescription(bool enableVSync = true);
    ContextDescription(bool enableVSync, bool enableAntiAliasing, unsigned int multiSamples);

    VSyncDescription            vsyncDesc;              //!< V-Sync description.
    AntiAliasingDescription     antiAliasingDesc;       //!< Multi-Sample Anti-Aliasing description.
    RendererProfileDescription  rendererProfileDesc;    //!< Renderer profile description.
};

/* --- Functions --- */

//! Returns the string to the respective D3D feature level.
FORK_EXPORT std::string FeatureLevelToStr(const D3DFeatureLevels featureLevel);


} // /namespace Video

} // /namespace Fork


#endif



// ========================