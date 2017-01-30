/*
 * Editor auto UV map UI component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_AUTO_UV_MAP_UI_COMPONENT_H__
#define __FORK_EDITOR_AUTO_UV_MAP_UI_COMPONENT_H__


#include "../../Core/Component/UIComponent.h"
#include "Engine/Component/AutoUVMapComponent.h"


namespace Fork
{

namespace Editor
{


class AutoUVMapUIComponent : public Engine::AutoUVMapComponent, public UIComponent
{
    DECL_UICOMPONENT_INTERFACE(AutoUVMap);
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================