/*
 * Integer property file
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


Component::Property::Types Component::IntProperty::Type() const
{
    return Types::Int;
}

void Component::IntProperty::WriteToFile(IO::File& file) const
{
    file.Write<int>(value);
}

void Component::IntProperty::ReadFromFile(IO::File& file)
{
    value = file.Read<int>();
}

void Component::IntProperty::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::IntProperty::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetInt();
}


} // /namespace Engine

} // /namespace Fork



// ========================