/*
 * String property file
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


Component::Property::Types Component::StringProperty::Type() const
{
    return Types::String;
}

void Component::StringProperty::WriteToFile(IO::File& file) const
{
    file.WriteStringSized(value);
}

void Component::StringProperty::ReadFromFile(IO::File& file)
{
    value = file.ReadStringSized();
}

void Component::StringProperty::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::StringProperty::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetString();
}


} // /namespace Engine

} // /namespace Fork



// ========================