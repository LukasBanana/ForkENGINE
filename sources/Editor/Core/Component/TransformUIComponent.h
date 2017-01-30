/*
 * Editor transform UI component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_TRANSFORM_UI_COMPONENT_H__
#define __FORK_EDITOR_TRANSFORM_UI_COMPONENT_H__


#include "../../Core/Component/UIComponent.h"
#include "Engine/Component/TransformComponent.h"


namespace Fork
{

namespace Editor
{


class TransformUIComponent : public Engine::TransformComponent, public UIComponent
{
    DECL_UICOMPONENT_INTERFACE(Transform);
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================