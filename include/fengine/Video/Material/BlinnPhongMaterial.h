/*
 * Blinn-phong material header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BLINN_PHONG_MATERIAL_H__
#define __FORK_BLINN_PHONG_MATERIAL_H__


#include "Video/Material/Material.h"


namespace Fork
{

namespace Video
{


/**
Material implementation for the blinn-phong shading model.
\remarks This is a base material.
\see http://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_shading_model
\ingroup std_materials
*/
class FORK_EXPORT BlinnPhongMaterial : public Material
{
    
    public:
        
        //! Minimal shininess factor. This is 0.1.
        static const float minShininess;
        //! Maximal shininess factor. This is 90.0.
        static const float maxShininess;

        /* === Interface implementation === */

        //! Returns 'Types::BlinnPhong'.
        Types Type() const override;

        /**
        Sets the diffuse and specular color.
        \see diffuse
        \see specular
        */
        void SetupColor(const ColorRGBAf& color) override;
        //! Returns the diffuse color.
        ColorRGBAf Color() const override;

        //! \see RoughnessToSpecularPower
        void SetupRoughness(float roughness) override;
        //! \see SpecularPowerToRoughness
        float Roughness() const override;

        /* === Static functions === */

        /**
        Converts the specified roughness factor to specular power (or rather 'shininess').
        \param[in] roughness Specifies the material roughness. Must be in the range [0.0 .. 1.0].
        \return Specular power in the range [0.1 .. 90.0].
        \see SpecularPowerToRoughness
        \see shininess
        */
        static float RoughnessToSpecularPower(float roughness);
        /**
        Converts the specified specular power (or rather 'shininess') to roughness factor.
        \param[in] specularPower Specifies the material specular power. Must be in the range [0.1 .. 90.0].
        \return Roughness factor in the range [0.0 .. 1.0].
        \see RoughnessToSpecularPower
        \see shininess
        */
        static float SpecularPowerToRoughness(float specularPower);

        /**
        Converts the specifies specular power to a cone angle (in radians).
        \param[in] specularPower Specifies the specular power (or rather 'shininess').
        \see shininess
        */
        static float SpecularPowerToConeAngle(float specularPower);

        /* === Members === */

        //! Ambient color. By default { 0.2, 0.2, 0.2, 1.0 }.
        ColorRGBAf  ambient     { 0.2f, 0.2f, 0.2f, 1.0f };

        //! Diffuse color. By default { 1.0, 1.0, 1.0, 1.0 }.
        ColorRGBAf  diffuse     { 1.0f, 1.0f, 1.0f, 1.0f };

        //! Specular color. By default { 1.0, 1.0, 1.0, 1.0 }.
        ColorRGBAf  specular    { 1.0f, 1.0f, 1.0f, 1.0f };

        //! Emissive color. By default { 0.0, 0.0, 0.0, 1.0 }.
        ColorRGBAf  emissive    { 0.0f, 0.0f, 0.0f, 1.0f };

        //! Shininess factor, or rather specular power. By default 90.0.
        float       shininess   { 90.0f };

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================