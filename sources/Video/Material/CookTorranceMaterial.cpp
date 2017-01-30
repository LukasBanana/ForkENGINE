/*
 * Cook-torrance material file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Material/CookTorranceMaterial.h"


namespace Fork
{

namespace Video
{


Material::Types CookTorranceMaterial::Type() const
{
    return Types::CookTorrance;
}

void CookTorranceMaterial::SetupColor(const ColorRGBAf& color)
{
    this->color = color;
}

ColorRGBAf CookTorranceMaterial::Color() const
{
    return color;
}

void CookTorranceMaterial::SetupRoughness(float roughness)
{
    this->roughness = roughness;
}

float CookTorranceMaterial::Roughness() const
{
    return roughness;
}


} // /namespace Video

} // /namespace Fork



// ========================