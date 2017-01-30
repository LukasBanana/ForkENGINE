/*
 * Vector4 property file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/Component.h"
#include "IO/FileSystem/File.h"


namespace Fork
{

namespace Engine
{


Component::Property::Types Component::Vector4Property::Type() const
{
    return Types::Vector4;
}

void Component::Vector4Property::WriteToFile(IO::File& file) const
{
    file.Write<float>(value.x);
    file.Write<float>(value.y);
    file.Write<float>(value.z);
    file.Write<float>(value.w);
}

void Component::Vector4Property::ReadFromFile(IO::File& file)
{
    value.x = file.Read<float>();
    value.y = file.Read<float>();
    value.z = file.Read<float>();
    value.w = file.Read<float>();
}

void Component::Vector4Property::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::Vector4Property::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetVector4();
}


} // /namespace Engine

} // /namespace Fork



// ========================