/*
 * Asset property file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/Component/Component.h"
#include "IO/FileSystem/File.h"
#include "Core/Container/Identifiable.h"


namespace Fork
{

namespace Engine
{


Component::Property::Types Component::AssetProperty::Type() const
{
    return Types::Asset;
}

void Component::AssetProperty::WriteToFile(IO::File& file) const
{
    file.Write<Identifiable::IDType>(value);
}

void Component::AssetProperty::ReadFromFile(IO::File& file)
{
    value = file.Read<Identifiable::IDType>();
}

void Component::AssetProperty::WriteToVariant(IO::Variant& variant) const
{
    variant = value;
}

void Component::AssetProperty::ReadFromVariant(const IO::Variant& variant)
{
    value = variant.GetUInt();
}


} // /namespace Engine

} // /namespace Fork



// ========================