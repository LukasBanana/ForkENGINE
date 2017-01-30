/*
 * Editor entity selector model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ENTITY_SELECTOR_MODEL_H__
#define __FORK_EDITOR_ENTITY_SELECTOR_MODEL_H__


#include "Utility/Selector/SelectorModel.h"


namespace Fork
{

namespace Editor
{


class Entity;

//! Entity selector model class.
class EntitySelectorModel : public Utility::SelectorModel
{
    
    public:
        
        EntitySelectorModel() = default;

        EntitySelectorModel(const EntitySelectorModel&) = delete;
        EntitySelectorModel& operator = (const EntitySelectorModel&) = delete;

        //! Returns the list of all selected entities.
        inline const std::vector<Entity*>& GetSelectedEntities() const
        {
            return selectedEntities_;
        }
        //! Returns the active selected entity or null if no entity is selected.
        inline Entity* GetActiveEntity() const
        {
            return activeEntity_;
        }

    private:
        
        void OnAddNode(Utility::Selectable* node) override;
        void OnRemoveNode(Utility::Selectable* node) override;
        void OnRemoveAllNodes() override;
        void OnActivateNode(Utility::Selectable* node) override;
        void OnDeactivateNode(Utility::Selectable* node) override;

        std::vector<Entity*>    selectedEntities_;
        Entity*                 activeEntity_       = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================