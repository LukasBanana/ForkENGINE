/*
 * Editor geometry UI component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_GEOMETRY_UI_COMPONENT_H__
#define __FORK_EDITOR_GEOMETRY_UI_COMPONENT_H__


#include "../../Core/Component/UIComponent.h"
#include "Engine/Component/GeometryComponent.h"


namespace Fork
{

namespace Editor
{


class GeometryUIComponent : public Engine::GeometryComponent, public UIComponent
{
    DECL_UICOMPONENT_INTERFACE(Geometry);
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================