/*
 * Editor meta data UI component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_META_DATA_UI_COMPONENT_H__
#define __FORK_EDITOR_META_DATA_UI_COMPONENT_H__


#include "../../Core/Component/UIComponent.h"
#include "Engine/Component/MetaDataComponent.h"


namespace Fork
{

namespace Editor
{


class MetaDataUIComponent : public Engine::MetaDataComponent, public UIComponent
{
    DECL_UICOMPONENT_INTERFACE(MetaData);
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================