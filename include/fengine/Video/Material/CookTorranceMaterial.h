/*
 * Cook-torrance material header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COOK_TORRANCE_MATERIAL_H__
#define __FORK_COOK_TORRANCE_MATERIAL_H__


#include "Video/Material/Material.h"


namespace Fork
{

namespace Video
{


/**
Material implementation for the blinn-phong shading model.
\remarks This is a base material.
\see http://ruh.li/GraphicsCookTorrance.html
\ingroup std_materials
*/
class FORK_EXPORT CookTorranceMaterial : public Material
{
    
    public:
        
        /* === Interface implementation === */

        //! Returns 'Types::CookTorrance'.
        Types Type() const override;

        //! \see color
        void SetupColor(const ColorRGBAf& color) override;
        //! \see color
        ColorRGBAf Color() const override;

        //! \see roughness
        void SetupRoughness(float roughness) override;
        //! \see roughness
        float Roughness() const override;

        /* === Members === */

        //! Material color. By default { 1.0, 1.0, 1.0, 1.0 }.
        ColorRGBAf  color       { 1.0f, 1.0f, 1.0f, 1.0f };

        //! Shininess factor, or rather specular power. By default 0.0.
        float       roughness   { 0.0f };

        //! Fresnel reflectance at normal incidence. By default 0.8.
        float       reflectance { 0.8f };

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================