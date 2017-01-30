/*
 * ColorRGBA property file
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


Component::Property::Types Component::ColorRGBAProperty::Type() const
{
    return Types::ColorRGBA;
}

void Component::ColorRGBAProperty::WriteToFile(IO::File& file) const
{
    file.Write<unsigned char>(value.r);
    file.Write<unsigned char>(value.g);
    file.Write<unsigned char>(value.b);
    file.Write<unsigned char>(value.a);
}

void Component::ColorRGBAProperty::ReadFromFile(IO::File& file)
{
    value.r = file.Read<unsigned char>();
    value.g = file.Read<unsigned char>();
    value.b = file.Read<unsigned char>();
    value.a = file.Read<unsigned char>();
}

void Component::ColorRGBAProperty::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::ColorRGBAProperty::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetColorRGBA();
}


} // /namespace Engine

} // /namespace Fork



// ========================