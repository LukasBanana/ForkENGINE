/*
 * Vector3 property file
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


Component::Property::Types Component::Vector3Property::Type() const
{
    return Types::Vector3;
}

void Component::Vector3Property::WriteToFile(IO::File& file) const
{
    file.Write<float>(value.x);
    file.Write<float>(value.y);
    file.Write<float>(value.z);
}

void Component::Vector3Property::ReadFromFile(IO::File& file)
{
    value.x = file.Read<float>();
    value.y = file.Read<float>();
    value.z = file.Read<float>();
}

void Component::Vector3Property::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::Vector3Property::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetVector3();
}


} // /namespace Engine

} // /namespace Fork



// ========================