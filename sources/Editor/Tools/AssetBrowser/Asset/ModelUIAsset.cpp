/*
 * Editor model UI asset file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ModelUIAsset.h"
#include "ModelPreviewRenderer.h"
#include "GeometryHierarchyBuilder.h"
#include "Core/Exception/NullPointerException.h"
#include "Scene/Geometry/Node/MeshGeometry.h"
#include "../AssetBrowser.h"
#include "../../../Frame/Main/MainFrame.h"
#include "../../../Frame/HierarchyViewer/HierarchyViewerFrame.h"
#include "../../../Core/Devices.h"
#include "../../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../../Core/Component/UIProperty/UIPropertyVector3.h"
#include "../../../Language/LanguageLoader.h"
#include "../../../Entity/Entity.h"
#include "../../../Entity/EntitySelector.h"
#include "../../../Project/ProjectFolder.h"


namespace Fork
{

namespace Editor
{


struct PropertyIDs
{
    enum : int
    {
        InitialSize,
    };
};

ModelUIAsset::ModelUIAsset(
    const PreviewPartition& previewPartition, const Scene::GeometryPtr& geometryResource, const std::string& assetLabel) :
        UIAsset             { previewPartition, assetLabel           },
        previewRotation     { ModelUIAsset::InitialPreviewRotation() },
        geometryResource_   { geometryResource                       }
{
    ASSERT_POINTER(geometryResource);

    DrawPreview();

    /* Setup initial model size */
    const auto clampSize = 1.0f / Math::Size3f(geometryResource->boundingVolume.box.Size()).Max();
    initialSize = { clampSize, clampSize, clampSize };
}

UIAsset::Types ModelUIAsset::Type() const
{
    return Types::Geometry;
}

UIAsset::Flags::DataType ModelUIAsset::Flags() const
{
    return Flags::HasRealtimePreview;
}

void ModelUIAsset::Instantiate()
{
    if (ProjectFolder::Active())
    {
        /* Instantiate new model entity */
        auto entity = ProjectFolder::Active()->CreateModelEntity(this);
        Devices::entitySelector->Select(entity);

        /* Setup initial scaling */
        entity->Transform([&](Scene::Transform& transform) { transform.SetScale(initialSize); });
    }
    /*else
        Error("Can not instantiate asset with no active project folder");*/
}

void ModelUIAsset::ShowGeometryHierarchy()
{
    /* Reset hierarchy viewer */
    auto hierarchyViewer = MainFrame::Instance()->GetHierarchyViewerFrame();
    hierarchyViewer->ClearHierarchy();

    /* Build geometry hierarchy */
    auto treeView = hierarchyViewer->GetTreeView();
    GeometryHierarchyBuilder hierarchyBuilder;

    hierarchyBuilder.BuildHierarchy(treeView, GetGeometryResource());

    /* Preset hierarchy viewer */
    hierarchyViewer->Show();
}

Math::Quaternionf ModelUIAsset::InitialPreviewRotation()
{
    Math::Matrix3f rotation;
    rotation.RotateX(Math::deg2rad * -45.0f);
    rotation.RotateY(Math::deg2rad * -45.0f);
    return Math::Quaternionf(rotation);
}

void ModelUIAsset::BuildPropertyGrid(UIPropertyGrid& propGrid, const wxString& namePrefix)
{
    propGrid.AddPropertyCategory(LANG_STRING(ModelAsset), namePrefix);

    propGrid.AddVector3Property(LANG_STRING(InitialSize), UIPropertyGrid::GetID(namePrefix, PropertyIDs::InitialSize))->Enable(false);
}


/*
 * ======= Private: =======
 */

void ModelUIAsset::DrawPreviewContent(const Math::Size2i& previewSize)
{
    auto previewRenderer = AssetBrowser::Instance()->GetModelPreviewRenderer();
    if (previewRenderer)
    {
        previewRenderer->RenderModelAssetPreview(
            *this, previewSize, previewRotation
        );
    }
}

void ModelUIAsset::OnShowAttributes(UIPropertyGrid& propGrid, const wxString& namePrefix)
{
    propGrid.SetPropertyVector3(UIPropertyGrid::GetID(namePrefix, PropertyIDs::InitialSize), initialSize);
}


} // /namespace Editor

} // /namespace Fork



// ========================