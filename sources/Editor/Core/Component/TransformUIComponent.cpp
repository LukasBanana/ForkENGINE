/*
 * Editor transform UI component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TransformUIComponent.h"
#include "UIPropertyGridBuilder.h"


namespace Fork
{

namespace Editor
{


void TransformUIComponent::BuildPropertyGrid(UIPropertyGridBuilder& builder)
{
    builder.InitCategory(LangStrings::Transform, TransformComponent::GetIDName());

    builder.Vector3( LangStrings::Position );
    builder.Vector3( LangStrings::Rotation );
    builder.Vector3( LangStrings::Scaling  );
}


} // /namespace Editor

} // /namespace Fork



// ========================