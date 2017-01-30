/*
 * Engine asset library header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_ASSET_LIBRARY_H__
#define __FORK_ENGINE_ASSET_LIBRARY_H__


#include "Core/Export.h"
#include "Engine/Asset/Asset.h"

#include <vector>
#include <memory>


namespace Fork
{

namespace Engine
{


//! Main class for the game's asset library.
class FORK_EXPORT AssetLibrary
{
    
    public:
        
        AssetLibrary() = default;
        virtual ~AssetLibrary();

        AssetLibrary(const AssetLibrary&) = delete;
        AssetLibrary& operator = (const AssetLibrary&) = delete;

    protected:
        
        std::vector<std::unique_ptr<Asset>> assets;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================