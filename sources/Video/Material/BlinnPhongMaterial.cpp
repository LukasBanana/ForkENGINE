/*
 * Blinn-phong material file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Material/BlinnPhongMaterial.h"
#include "Math/Core/BaseMath.h"

#include <cmath>


namespace Fork
{

namespace Video
{


const float BlinnPhongMaterial::minShininess = 0.1f;
const float BlinnPhongMaterial::maxShininess = 90.0f;

Material::Types BlinnPhongMaterial::Type() const
{
    return Types::BlinnPhong;
}

void BlinnPhongMaterial::SetupColor(const ColorRGBAf& color)
{
    diffuse = color;
    specular = color;
}

ColorRGBAf BlinnPhongMaterial::Color() const
{
    return diffuse;
}

void BlinnPhongMaterial::SetupRoughness(float roughness)
{
    shininess = BlinnPhongMaterial::RoughnessToSpecularPower(roughness);
}

float BlinnPhongMaterial::Roughness() const
{
    return BlinnPhongMaterial::SpecularPowerToRoughness(shininess);
}

float BlinnPhongMaterial::RoughnessToSpecularPower(float roughness)
{
    return Math::Lerp(BlinnPhongMaterial::maxShininess, BlinnPhongMaterial::minShininess, roughness);
}

float BlinnPhongMaterial::SpecularPowerToRoughness(float specularPower)
{
    return (specularPower - BlinnPhongMaterial::minShininess) / (BlinnPhongMaterial::maxShininess - BlinnPhongMaterial::minShininess);
}

float BlinnPhongMaterial::SpecularPowerToConeAngle(float specularPower)
{
    static const float xi = 0.244f;
    float exponent = 1.0f / (specularPower + 1.0f);
    return std::acos(std::pow(xi, exponent));
}


} // /namespace Video

} // /namespace Fork



// ========================