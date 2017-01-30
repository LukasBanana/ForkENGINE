/*
 * Boolean property file
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


Component::Property::Types Component::BoolProperty::Type() const
{
    return Types::Bool;
}

void Component::BoolProperty::WriteToFile(IO::File& file) const
{
    file.Write<char>(value ? 1 : 0);
}

void Component::BoolProperty::ReadFromFile(IO::File& file)
{
    value = (file.Read<char>() != 0);
}

void Component::BoolProperty::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::BoolProperty::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetBool();
}


} // /namespace Engine

} // /namespace Fork



// ========================