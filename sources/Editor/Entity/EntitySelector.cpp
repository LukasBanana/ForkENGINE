/*
 * Editor entity selector file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EntitySelector.h"
#include "EntitySelectorModel.h"
#include "Entity.h"
#include "../EditorView.h"
#include "../Core/Devices.h"
#include "../Core/DefaultColors.h"
#include "../Core/UIExtensions/UIToolBar.h"
#include "../Core/UIExtensions/UIMenuList.h"
#include "../Core/UIExtensions/AbstractFrame.h"
#include "../Frame/Main/MainFrame.h"
#include "../Frame/Main/MainFrameToolBarComposition.h"
#include "../Frame/Main/MainFrameComponentController.h"
#include "../Frame/Main/MainEventTable.h"
#include "../Project/ProjectFolder.h"


namespace Fork
{

namespace Editor
{


/*
 * Internals
 */

using namespace Utility;

typedef SelectorModel::Raster::Modes RasterModes;

static UIToolBar* TransformToolBar()
{
    return MainFrame::Instance()->GetToolBarComposition()->GetToolBars()[MainFrameToolBarComposition::ToolBars::Transform];
};

static void HideComponents()
{
    MainFrame::Instance()->GetComponentCtrl()->HideAllComponentCategories();
}


/*
 * EntitySelector class
 */

const std::array<float, 6> EntitySelector::rasterSizeTable =
{
    0.1f, 0.25f, 0.5f, 1.0f, 2.5f, 5.0f
};

EntitySelector::EntitySelector() :
    Selector
    {
        Devices::primitiveRenderer,
        Devices::sceneManager.get(),
        nullptr,
        std::make_shared<EntitySelectorModel>()
    }
{
    /* Store reference to entity selector model */
    entitySelectorModel_ = static_cast<EntitySelectorModel*>(GetModel());

    /* Use specialized mouse event handler to update selector projection by editor view */
    ExchangeMouseEventHandler(std::make_shared<SelectorMouseEventHandler>(this));

    /* Setup initial color configuration */
    GetView()->config.axes.x.normalColor = DefaultColors::xAxis;
    GetView()->config.axes.y.normalColor = DefaultColors::yAxis;
    GetView()->config.axes.z.normalColor = DefaultColors::zAxis;
}

/* --- Configuration --- */

void EntitySelector::SetupTransformMode(const SelectorModel::TransformModes mode)
{
    GetModel()->transformMode = mode;
    TransformToolBar()->SwitchItem(MainMenuIDs::TransformTranslate + static_cast<int>(mode));
}

void EntitySelector::SetupOrientationAlignment(const SelectorModel::OrientationAlignments alignment)
{
    /* Setup alignment for selector model */
    GetModel()->orientationAlignment = alignment;

    /* Change toolbar item bitmap */
    auto toolBar = TransformToolBar();
    switch (alignment)
    {
        case SelectorModel::OrientationAlignments::Global:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorOrientation, wxBITMAP_PNG( PngOrientationGlobal ));
            break;
        case SelectorModel::OrientationAlignments::Local:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorOrientation, wxBITMAP_PNG( PngOrientationLocal  ));
            break;
        case SelectorModel::OrientationAlignments::View:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorOrientation, wxBITMAP_PNG( PngOrientationView   ));
            break;
    }
    toolBar->Refresh();
}

void EntitySelector::SetupPivotAlignment(const SelectorModel::PivotAlignments alignment)
{
    /* Setup alignment for selector model */
    GetModel()->pivotAlignment = alignment;

    /* Change toolbar item bitmap */
    auto toolBar = TransformToolBar();
    switch (alignment)
    {
        case SelectorModel::PivotAlignments::MedianPoint:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorPivot, wxBITMAP_PNG( PngPivotMedianPoint       ));
            break;
        case SelectorModel::PivotAlignments::IndividualOrigins:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorPivot, wxBITMAP_PNG( PngPivotIndividualOrigins ));
            break;
        case SelectorModel::PivotAlignments::ActiveElement:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorPivot, wxBITMAP_PNG( PngPivotActiveElement     ));
            break;
        case SelectorModel::PivotAlignments::BoundingBox:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorPivot, wxBITMAP_PNG( PngPivotBoundingBox       ));
            break;
        case SelectorModel::PivotAlignments::Minimum:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorPivot, wxBITMAP_PNG( PngPivotMinimum           ));
            break;
        case SelectorModel::PivotAlignments::Maximum:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorPivot, wxBITMAP_PNG( PngPivotMaximum           ));
            break;
        case SelectorModel::PivotAlignments::GlobalCursor:
            toolBar->SetToolBitmap(MainMenuIDs::SelectorPivot, wxBITMAP_PNG( PngPivotGlobalCursor      ));
            break;
    }
    toolBar->Refresh();
}

void EntitySelector::SetupRaster(const SelectorModel::Raster::Modes mode)
{
    /* Setup raster mode for selector model */
    GetModel()->raster.mode = mode;

    /* Change toolbar item bitmap */
    auto toolBar = TransformToolBar();
    switch (mode)
    {
        case RasterModes::None:
            toolBar->SetToolBitmap(MainMenuIDs::TransformRaster, wxBITMAP_PNG( PngTransformNoRaster       ));
            break;
        case RasterModes::Relative:
            toolBar->SetToolBitmap(MainMenuIDs::TransformRaster, wxBITMAP_PNG( PngTransformRelativeRaster ));
            break;
        case RasterModes::Absolute:
            toolBar->SetToolBitmap(MainMenuIDs::TransformRaster, wxBITMAP_PNG( PngTransformAbsoluteRaster ));
            break;
    }
    toolBar->Refresh();
}

void EntitySelector::SwitchRaster()
{
    switch (GetModel()->raster.mode)
    {
        case RasterModes::None:
            SetupRaster(RasterModes::Relative);
            break;
        case RasterModes::Relative:
            SetupRaster(RasterModes::Absolute);
            break;
        case RasterModes::Absolute:
            SetupRaster(RasterModes::None);
            break;
    }
}

void EntitySelector::SetupRasterSize(size_t sizeIndex)
{
    if (sizeIndex < EntitySelector::rasterSizeTable.size())
    {
        GetModel()->raster.size = EntitySelector::rasterSizeTable[sizeIndex];
        TransformToolBar()->SwitchItem(MainMenuIDs::TransformRaster_0_1 + sizeIndex);
    }
}

/* --- Selection --- */

void EntitySelector::SelectEntity(Entity* entity, bool select)
{
    if (entity)
    {
        if (select)
        {
            GetModel()->AddNode(entity);
            entity->Select();
        }
        else
        {
            GetModel()->RemoveNode(entity);
            entity->Deselect();
        }
    }
}

void EntitySelector::SelectWithModifier(Entity* entity)
{
    auto model = GetModel();

    if (entity)
    {
        /* Apply selection */
        if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyShift))
        {
            /* Always add scene node */
            SelectEntity(entity);
        }
        else if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
        {
            /* Add scene node if not already selected, otherwise remove it */
            SelectEntity(entity, !model->HasNode(entity));
        }
        else
        {
            /* Only select new scene node */
            DeselectAllEntities();
            SelectEntity(entity);
        }
    }
    else
    {
        /* DeSelect all scene nodes */
        DeselectAllEntities();
    }

    /* Update components view for the currently active entity */
    UpdateComponentView();
}

void EntitySelector::Select(Entity* entity)
{
    /* Select specified entity only */
    DeselectAllEntities();

    if (entity)
        GetModel()->AddNode(entity);

    /* Update components view for the currently active entity */
    UpdateComponentView();
}

void EntitySelector::DeselectAllEntities()
{
    /* First unregister all selected property grid observers */
    for (auto entity : GetSelectedEntities())
        entity->Deselect();

    /* Then remove all selected nodes */
    GetModel()->RemoveAllNodes();
}

const std::vector<Entity*>& EntitySelector::GetSelectedEntities() const
{
    return entitySelectorModel_->GetSelectedEntities();
}

Entity* EntitySelector::GetActiveEntity() const
{
    return entitySelectorModel_->GetActiveEntity();
}

/* --- Entity Instantiation --- */

void EntitySelector::CopySelectedEntities()
{
    //...
}

void EntitySelector::CutSelectedEntities()
{
    //...
}

void EntitySelector::PasteSelectedEntities()
{
    //...
}

void EntitySelector::DeleteSelectedEntities()
{
    /* Get active project from which the entities are to be deleted */
    auto project = ProjectFolder::Active();
    if (!project)
        return;

    /*
    Get copy of selected entity list to allow removing
    entries during deletion (see Entity::~Entity destructor)
    */
    auto selectedEntities = GetSelectedEntities();

    for (auto entity : selectedEntities)
        project->DeleteEntity(entity);

    /* Update component view after deletion */
    UpdateComponentView();
}

/* --- Misc --- */

bool EntitySelector::IsVisible() const
{
    return GetModel()->AreAnyNodesSelected();
}

void EntitySelector::DrawSelector()
{
    SelectorView::DrawFlags::DataType flags = 0;

    if (config.drawSelectedAxesOnly)
        flags |= SelectorView::DrawFlags::DrawSelectedAxesOnly;
    if (config.drawPrevTransform)
        flags |= SelectorView::DrawFlags::DrawPrevTransform;
    if (config.drawAxesLabels)
        flags |= SelectorView::DrawFlags::DrawAxesLabels;

    Draw(flags);
}

void EntitySelector::UpdateComponentView() const
{
    HideComponents();
    if (GetActiveEntity())
        GetActiveEntity()->SelectAndShowUIComponents();
}


/*
 * SelectorMouseEventHandler class
 */

EntitySelector::SelectorMouseEventHandler::SelectorMouseEventHandler(Selector* selector) :
    MouseEventHandler{ selector }
{
}

bool EntitySelector::SelectorMouseEventHandler::OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    if (SetupSelectorViewForFrame(frame))
        return MouseEventHandler::OnButtonDown(keyCode, frame);
    return true;
}

bool EntitySelector::SelectorMouseEventHandler::OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    if (SetupSelectorViewForFrame(frame))
        return MouseEventHandler::OnButtonUp(keyCode, frame);
    return true;
}

bool EntitySelector::SelectorMouseEventHandler::OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame)
{
    if (SetupSelectorViewForFrame(frame))
        return MouseEventHandler::OnLocalMotion(position, frame);
    return true;
}

/*
This function sets the selector's view to the camera which is joined to the specified frame's editor view.
This is required that the selector picking can work, since it depends on the editor view's projection and view orientation.
*/
bool EntitySelector::SelectorMouseEventHandler::SetupSelectorViewForFrame(const Platform::Frame* frame)
{
    auto screenFrame = dynamic_cast<const AbstractFrame*>(frame);
    
    if (screenFrame)
    {
        auto editorView = reinterpret_cast<EditorView*>(screenFrame->userData);
        if (editorView)
        {
            GetSelector()->SetupView(*editorView->GetCamera());
            return true;
        }
    }

    return false;
}


} // /namespace Editor

} // /namespace Fork



// ========================