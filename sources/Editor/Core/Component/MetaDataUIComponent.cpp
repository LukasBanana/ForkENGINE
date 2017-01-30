/*
 * Editor meta data UI component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MetaDataUIComponent.h"
#include "UIPropertyGridBuilder.h"


namespace Fork
{

namespace Editor
{


void MetaDataUIComponent::BuildPropertyGrid(UIPropertyGridBuilder& builder)
{
    builder.InitCategory(LangStrings::MetaData, MetaDataUIComponent::GetIDName());

    builder.String  ( LangStrings::Name    );
    builder.Bool    ( LangStrings::Enabled );
}


} // /namespace Editor

} // /namespace Fork



// ========================