/*
 * ColorRGB property file
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


Component::Property::Types Component::ColorRGBProperty::Type() const
{
    return Types::ColorRGB;
}

void Component::ColorRGBProperty::WriteToFile(IO::File& file) const
{
    file.Write<unsigned char>(value.r);
    file.Write<unsigned char>(value.g);
    file.Write<unsigned char>(value.b);
}

void Component::ColorRGBProperty::ReadFromFile(IO::File& file)
{
    value.r = file.Read<unsigned char>();
    value.g = file.Read<unsigned char>();
    value.b = file.Read<unsigned char>();
}

void Component::ColorRGBProperty::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::ColorRGBProperty::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetColorRGB();
}


} // /namespace Engine

} // /namespace Fork



// ========================