/*
 * Editor auto UV map UI component file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AutoUVMapUIComponent.h"
#include "UIPropertyGridBuilder.h"


namespace Fork
{

namespace Editor
{


void AutoUVMapUIComponent::BuildPropertyGrid(UIPropertyGridBuilder& builder)
{
    builder.InitCategory(LangStrings::AutoUVMap, AutoUVMapComponent::GetIDName());

    builder.Bool    ( LangStrings::Update  );
    builder.Bool    ( LangStrings::Global  );
    builder.Float   ( LangStrings::OffsetU );
    builder.Float   ( LangStrings::OffsetV );
    builder.Float   ( LangStrings::ScaleU  );
    builder.Float   ( LangStrings::ScaleV  );
}


} // /namespace Editor

} // /namespace Fork



// ========================