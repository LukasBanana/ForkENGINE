/*
 * Network interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_NETWORK_INTERFACE_H__
#define __FORK_PLATFORM_NETWORK_INTERFACE_H__


#include "Core/Export.h"
#include "Network/Core/Adapter.h"

#include <string>
#include <vector>


namespace Fork
{

namespace Platform
{

namespace NetworkInterface
{


//! Network start-up information structure.
struct FORK_EXPORT StartUpInfo
{
    std::string desc;   //!< Network interface description.
    std::string status; //!< Network interface status.
};


/**
Initializes the network interface.
\param[out] info Optional output raw-pointer to the start-up information. By default null.
\see StartUpInfo
*/
FORK_EXPORT bool StartUp(StartUpInfo* info = nullptr);

//! Releases the network interface.
FORK_EXPORT void CleanUp();

//! Returns a list of all network adapters.
FORK_EXPORT std::vector<Network::Adapter> QueryAdapters();


} // /namespace NetworkInterface

} // /namespace Platform

} // /namespace Fork


#endif



// ========================