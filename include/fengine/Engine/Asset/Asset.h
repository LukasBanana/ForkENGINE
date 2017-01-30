/*
 * Engine asset header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_ASSET_H__
#define __FORK_ENGINE_ASSET_H__


#include "Core/Export.h"

#include <vector>
#include <memory>


namespace Fork
{

namespace Engine
{


//! Base class for all assets.
class FORK_EXPORT Asset
{
    
    public:
        
        /* === Enumerations === */

        //! Asset types.
        enum class Types
        {
            __Unknown__,
            Texture,    //!< Asset for Video::Texture
            Geometry,   //!< Asset for Scene::Geometry
            Sound,      //!< (Asset for Audio::SoundSource)
            //Script,     //!< (Asset for Script::Program)
            //Shader,     //!< Asset for Video::ShaderComposition
        };

        /* === Functions === */

        virtual ~Asset();

        Asset(const Asset&) = delete;
        Asset& operator = (const Asset&) = delete;

        //! Returns the asset type.
        virtual Types Type() const = 0;

    protected:
        
        Asset() = default;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================