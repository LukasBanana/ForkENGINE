/*
 * Editor geometry UI component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GeometryUIComponent.h"
#include "UIPropertyGridBuilder.h"


namespace Fork
{

namespace Editor
{


void GeometryUIComponent::BuildPropertyGrid(UIPropertyGridBuilder& builder)
{
    builder.InitCategory(LangStrings::Geometry, GeometryUIComponent::GetIDName());

    builder.Asset( LangStrings::Asset );
}


} // /namespace Editor

} // /namespace Fork



// ========================