/*
 * Editor entity selector model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EntitySelectorModel.h"
#include "Entity.h"
#include "Core/STLHelper.h"
#include "Core/CastToConcreteObject.h"


namespace Fork
{

namespace Editor
{


/*
 * ======= Private: =======
 */

#define CAST_TO_ENTITY(src) CastToConcreteObject<Entity>(src, __FUNCTION__, "Selectable", "Entity")

void EntitySelectorModel::OnAddNode(Utility::Selectable* node)
{
    selectedEntities_.push_back(CAST_TO_ENTITY(node));
}

void EntitySelectorModel::OnRemoveNode(Utility::Selectable* node)
{
    RemoveFromList(selectedEntities_, node);
}

void EntitySelectorModel::OnRemoveAllNodes()
{
    selectedEntities_.clear();
}

void EntitySelectorModel::OnActivateNode(Utility::Selectable* node)
{
    activeEntity_ = CAST_TO_ENTITY(node);
}

void EntitySelectorModel::OnDeactivateNode(Utility::Selectable* node)
{
    activeEntity_ = nullptr;
}

#undef CAST_TO_ENTITY


} // /namespace Editor

} // /namespace Fork



// ========================