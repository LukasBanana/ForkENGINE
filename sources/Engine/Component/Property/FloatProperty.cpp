/*
 * Floating-point property file
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


Component::Property::Types Component::FloatProperty::Type() const
{
    return Types::Float;
}

void Component::FloatProperty::WriteToFile(IO::File& file) const
{
    file.Write<float>(value);
}

void Component::FloatProperty::ReadFromFile(IO::File& file)
{
    value = file.Read<float>();
}

void Component::FloatProperty::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::FloatProperty::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetFloat();
}


} // /namespace Engine

} // /namespace Fork



// ========================