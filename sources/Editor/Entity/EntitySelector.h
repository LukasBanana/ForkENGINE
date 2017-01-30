/*
 * Editor entity selector header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ENTITY_SELECTOR_H__
#define __FORK_EDITOR_ENTITY_SELECTOR_H__


#include "Utility/Selector/Selector.h"


namespace Fork
{

namespace Editor
{


class Entity;
class EntitySelectorModel;

//! Entity selector class.
class EntitySelector : public Utility::Selector
{
    
    public:
        
        static const std::array<float, 6> rasterSizeTable;

        //! Entity selector drawing configuration structure.
        struct Config
        {
            bool drawSelectedAxesOnly   = false; //!< Refers to Utility::SelectorView::DrawFlags::DrawSelectedAxesOnly.
            bool drawPrevTransform      = false; //!< Refers to Utility::SelectorView::DrawFlags::DrawPrevTransform.
            bool drawAxesLabels         = false; //!< Refers to Utility::SelectorView::DrawFlags::DrawAxesLabels.
        };

        EntitySelector();

        EntitySelector(const EntitySelector&) = delete;
        EntitySelector& operator = (const EntitySelector&) = delete;

        /* --- Configuration --- */

        void SetupTransformMode(const Utility::SelectorModel::TransformModes mode);
        void SetupOrientationAlignment(const Utility::SelectorModel::OrientationAlignments alignment);
        void SetupPivotAlignment(const Utility::SelectorModel::PivotAlignments alignment);
        void SetupRaster(const Utility::SelectorModel::Raster::Modes mode);
        
        /**
        Switches to the next raster mode
        \see SetupRaster
        */
        void SwitchRaster();

        //! Sets the raster size. The index selects the size from a pre-defined set.
        void SetupRasterSize(size_t sizeIndex);

        /* --- Selection --- */

        //! Selects an entity explicit.
        void SelectEntity(Entity* entity, bool select = true);
        /**
        Selects the specified entity. The selection behavior depends on
        the current keyboard state modifier (if SHIFT or CONTROL is pressed down).
        \param[in] entity Specifies the entity which is to be selected.
        If this is null, all scene nodes will be deselected.
        */
        void SelectWithModifier(Entity* entity);
        /**
        Selects the specified entity and shows its components.
        This will deselect all previously selected entities.
        \param[in] entity Raw pointer to the entity which is to be selected
        or null if all selected entities are to be deselected only.
        */
        void Select(Entity* entity);

        /**
        Deselects all entities.
        \note Don't call 'GetModel()->RemoveAllNodes()', because all deselected property grid
        observers (the entities) must be unregistered from the property grid!
        */
        void DeselectAllEntities();

        //! Returns the list of all selected entities.
        const std::vector<Entity*>& GetSelectedEntities() const;
        //! Returns the active selected entity or null if no entity is selected.
        Entity* GetActiveEntity() const;

        /* --- Entity Instantiation --- */

        //! Copies all selected entities into the clipboard.
        void CopySelectedEntities();
        //! Copies all selected entities into the clipboard and then deletes the selected entities.
        void CutSelectedEntities();
        //! Pastes the previously copied entities from the clipboard.
        void PasteSelectedEntities();
        //! Deletes all selected entities.
        void DeleteSelectedEntities();

        /* --- Misc --- */

        //! Returns true if this selector is visible.
        bool IsVisible() const;

        //! Draws the entity selector with the internal flags.
        void DrawSelector();

        //! Shows and hides the respective categories in the property grid of the component view.
        void UpdateComponentView() const;

        /* === Members === */

        //! Drawing configuration.
        Config config;

    private:
        
        /**
        Override default mouse event handler to update update selector projection by editor view.
        \see Utility::SelectorModel::SetupView
        \see EditorView
        */
        class SelectorMouseEventHandler : public MouseEventHandler
        {
            
            public:
                
                SelectorMouseEventHandler(Selector* selector);

                bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;

                bool OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame) override;

            private:
                
                //! Sets the selector view for the specified frame, that the entity selector can pick an entity.
                bool SetupSelectorViewForFrame(const Platform::Frame* frame);

        };

        EntitySelectorModel* entitySelectorModel_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================