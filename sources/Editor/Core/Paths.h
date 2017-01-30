/*
 * Editor paths header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_PATHS_H__
#define __FORK_EDITOR_PATHS_H__


#include <string>


namespace Fork
{

namespace Editor
{

namespace Paths
{


//! Configuration directory path.
static const std::string dirConfig = "config/";

//! Images directory path.
static const std::string dirImages = "images/";

//! Misc directory path.
static const std::string dirMisc = "misc/";

//! Default projects path.
static const std::string dirProjects = "projects/";

/**
Scripts directory path.
\todo This is not the correct path for the final release version!
*/
static const std::string dirScripts = "../trunk/scripts/";


} // /namespace Paths

} // /namespace Editor

} // /namespace Fork


#endif



// ========================