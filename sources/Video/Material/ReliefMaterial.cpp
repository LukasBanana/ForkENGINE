/*
 * Relief material file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Material/ReliefMaterial.h"


namespace Fork
{

namespace Video
{


Material::Types ReliefMaterial::Type() const
{
    return Types::Relief;
}

void ReliefMaterial::SetupColor(const ColorRGBAf& color)
{
    if (actualMaterial)
        actualMaterial->SetupColor(color);
}

ColorRGBAf ReliefMaterial::Color() const
{
    return actualMaterial != nullptr ? actualMaterial->Color() : ColorRGBAf();
}

void ReliefMaterial::SetupRoughness(float roughness)
{
    if (actualMaterial)
        actualMaterial->SetupRoughness(roughness);
}

float ReliefMaterial::Roughness() const
{
    return actualMaterial != nullptr ? actualMaterial->Roughness() : 0.0f;
}


} // /namespace Video

} // /namespace Fork



// ========================