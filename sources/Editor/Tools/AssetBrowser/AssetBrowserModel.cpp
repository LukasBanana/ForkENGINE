/*
 * Editor asset browser model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AssetBrowserModel.h"
#include "../../Core/Devices.h"
#include "../../Frame/Main/MainFrame.h"
#include "../../Frame/Main/MainFrameComponentController.h"
#include "../../Frame/Main/MainFrameAssetDescriptor.h"
#include "IO/Core/Log.h"
#include "Core/STLHelper.h"

#include "Asset/TextureUIAsset.h"
#include "Asset/ModelUIAsset.h"
#include "Asset/SoundUIAsset.h"

#include <algorithm>


namespace Fork
{

namespace Editor
{


AssetBrowserModel::AssetBrowserModel()
{
    CreatePreviewMask();
}

/* --- Selection --- */

bool AssetBrowserModel::IsAssetSelected(const UIAsset* asset) const
{
    return std::find(selectedAssets_.begin(), selectedAssets_.end(), asset) != selectedAssets_.end();
}

UIAsset* AssetBrowserModel::LastSelectedAsset() const
{
    return !selectedAssets_.empty() ? selectedAssets_.back() : nullptr;
}

void AssetBrowserModel::SelectAsset(UIAsset* asset, bool select)
{
    if (asset)
    {
        if (select)
        {
            /* Select asset */
            if (!IsAssetSelected(asset))
                selectedAssets_.push_back(asset);
            asset->Select();
        }
        else
        {
            /* Deselect asset */
            RemoveFromList(selectedAssets_, asset);
            asset->Deselect();
        }
    }

    /* Show attributes of last selected asset */
    ShowAssetAttributes();
}

void AssetBrowserModel::SelectWithModifier(UIAsset* asset)
{
    if (asset)
    {
        /* Apply selection */
        if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyShift))
        {
            /* Always add asset*/
            SelectAsset(asset);
        }
        else if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
        {
            /* Add asset if not already selected, otherwise remove it */
            SelectAsset(asset, !IsAssetSelected(asset));
        }
        else
        {
            /* Only select new asset */
            DeselectAllAssets();
            SelectAsset(asset);
        }
    }
    else
        DeselectAllAssets();

    /* Show attributes of last selected asset */
    ShowAssetAttributes();
}

void AssetBrowserModel::Select(UIAsset* asset)
{
    /* Select new asset */
    DeselectAllAssets();
    SelectAsset(asset);

    /* Show attributes of last selected asset */
    ShowAssetAttributes();
}

void AssetBrowserModel::DeselectAllAssets()
{
    for (auto asset : selectedAssets_)
        asset->Deselect();
    selectedAssets_.clear();
}

void AssetBrowserModel::SelectLeft()
{
    Select(MoveAndAccessEntryPosition({ -1, 0 }));
}

void AssetBrowserModel::SelectRight()
{
    Select(MoveAndAccessEntryPosition({ 1, 0 }));
}

void AssetBrowserModel::SelectUp()
{
    Select(MoveAndAccessEntryPosition({ 0, -1 }));
}

void AssetBrowserModel::SelectDown()
{
    Select(MoveAndAccessEntryPosition({ 0, 1 }));
}

void AssetBrowserModel::SelectRowBegin()
{
    selectionGrid_.LocateSelection({ 0, selectionGrid_.GetSelectionPosition().y });
    Select(AssetFromEntryPosition());
}

void AssetBrowserModel::SelectRowEnd()
{
    if (selectionGrid_.GetGridSize().width > 0)
    {
        selectionGrid_.LocateSelection({ selectionGrid_.GetGridSize().width - 1, selectionGrid_.GetSelectionPosition().y });
        Select(AssetFromEntryPosition());
    }
}

void AssetBrowserModel::SelectColumnBegin()
{
    selectionGrid_.LocateSelection({ selectionGrid_.GetSelectionPosition().x, 0 });
    Select(AssetFromEntryPosition());
}

void AssetBrowserModel::SelectColumnEnd()
{
    if (selectionGrid_.GetGridSize().height > 0)
    {
        selectionGrid_.LocateSelection({ selectionGrid_.GetSelectionPosition().x, selectionGrid_.GetGridSize().height - 1 });
        Select(AssetFromEntryPosition());
    }
}

//!TODO! optimize this selection tests!!!
void AssetBrowserModel::ProcessSelection(const Math::Point2f& pointerPosition, const Math::Point2f& offsetPosition)
{
    /* Get relative pointer position */
    const auto relativePosition = pointerPosition - offsetPosition;

    /* Find selected asset */
    size_t i = 0;
    for (const auto& asset : assets_)
    {
        /* Check if current asset preview entry is selected */
        const auto& pos = asset->previewPosition;
        Math::Rectf rect { pos, previewSize };

        if (rect.Overlap(relativePosition))
        {
            /* Select current asset and store selection position on the grid */
            selectionGrid_.LocateSelection(selectionGrid_.PositionByIndex(i));
            SelectWithModifier(asset.get());
            return;
        }

        ++i;
    }

    /* Deselect asset */
    Select(nullptr);
}

/* --- Asset instantiation --- */

UIAssetPtr AssetBrowserModel::AddTextureAsset(const Video::TexturePtr& textureResource, const std::string& assetLabel)
{
    if (textureResource)
    {
        return AddAssetToList(
            std::make_shared<TextureUIAsset>(GenerateAssetPreviewPartition(), textureResource, assetLabel)
        );
    }
    return nullptr;
}

UIAssetPtr AssetBrowserModel::AddModelAsset(const Scene::GeometryPtr& geometryResource, const std::string& assetLabel)
{
    if (geometryResource)
    {
        return AddAssetToList(
            std::make_shared<ModelUIAsset>(GenerateAssetPreviewPartition(), geometryResource, assetLabel)
        );
    }
    return nullptr;
}

UIAssetPtr AssetBrowserModel::AddSoundAsset(const Audio::SoundSourcePtr& soundResource, const std::string& assetLabel)
{
    if (soundResource)
    {
        return AddAssetToList(
            std::make_shared<SoundUIAsset>(GenerateAssetPreviewPartition(), soundResource, assetLabel)
        );
    }
    return nullptr;
}

/* --- Misc --- */

void AssetBrowserModel::TurnSelectedModelAsset(float pitch, float yaw)
{
    for (auto selectedAsset : GetSelectedAssets())
    {
        if (selectedAsset->Type() == UIAsset::Types::Geometry)
        {
            auto modelAsset = static_cast<ModelUIAsset*>(selectedAsset);

            /* Change preview rotation */
            Math::Matrix3f rotation;
            rotation.RotateX(pitch);
            rotation.RotateY(yaw);
            modelAsset->previewRotation = Math::Quaternionf(rotation * modelAsset->previewRotation.Mat3());

            /* Update preview image */
            UpdateAssetPreview(modelAsset);
        }
    }
}


/*
 * ======= Protected: =======
 */

void AssetBrowserModel::UnregisterAsset(UIAsset* asset)
{
    //RemoveAssetPreviewPartition(asset->GetPreviewPartition());
    //UpdateAssetPreviewLocations();
}

void AssetBrowserModel::UpdateAssetPreviewLocations(
    const Math::Size2f& viewportSize, Math::Point2f& lowestPosition, const Video::Font* labelFont)
{
    /* First update asset label configurations */
    for (auto& asset : assets_)
        asset->ChangeLabelConfig(labelFont, static_cast<int>(previewSize.width));

    /* Update image entry positions for all preview managers */
    lowestPosition = Math::Point2f{ previewBorder };

    size_t numEntries = 0;
    Math::Size2i grid;

    ext::const_range_forward_iterator<std::vector<UIAssetPtr>> assetIterator{ GetAssets() };

    for (auto previewMngr : assetPreviewMngrs_)
    {
        previewMngr->UpdateImageEntryPositions(
            assetIterator, lowestPosition, grid, viewportSize, previewSize, previewBorder
        );
    }

    /* Resize preview selection grid */
    selectionGrid_.ResizeGrid(assets_.size(), static_cast<unsigned int>(grid.width));

    /* Store preview position for each asset */
    for (auto& asset : assets_)
    {
        /* Update preview position */
        const auto& previewPartition = asset->GetPreviewPartition();
        auto imageEntry = previewPartition.previewMngr->ImageEntry(previewPartition.layer);
        if (imageEntry)
            asset->previewPosition = imageEntry->Position();
    }

    /* Update asset preview mask */
    assetPreviewMask_->UpdateImageEntryPositions(assetPreviewMngrs_);
}


/*
 * ======= Private: =======
 */

void AssetBrowserModel::CreatePreviewMask()
{
    /* Create asset preview mask container */
    assetPreviewMask_ = std::make_unique<AssetPreviewMask>();
}

UIAsset::PreviewPartition AssetBrowserModel::GenerateAssetPreviewPartition()
{
    static const Video::Texture2D::SizeType assetPreviewSize { 256, 256 };

    /* Generate layer in preview texture manager */
    UIAsset::PreviewPartition previewPartition;

    for (auto it = assetPreviewMngrs_.begin(); it != assetPreviewMngrs_.end(); ++it)
    {
        auto previewTexMngr = it->get();

        if (previewTexMngr->GenerateLayer(previewPartition.layer))
        {
            /* Found texture with free layer -> return preview partition */
            previewPartition.texture = previewTexMngr->GetSharedTexture();
            previewPartition.previewMngr = previewTexMngr;
            return previewPartition;
        }
    }

    /* Create new preview texture manager */
    auto previewTexMngr = std::make_shared<AssetPreviewManager>(assetPreviewSize);
    assetPreviewMngrs_.push_back(previewTexMngr);

    previewPartition.texture = previewTexMngr->GetSharedTexture();
    previewPartition.previewMngr = previewTexMngr.get();
    previewTexMngr->GenerateLayer(previewPartition.layer);

    return previewPartition;
}

void AssetBrowserModel::RemoveAssetPreviewPartition(const UIAsset::PreviewPartition& previewPartition)
{
    for (auto it = assetPreviewMngrs_.begin(); it != assetPreviewMngrs_.end(); ++it)
    {
        auto previewTexMngr = *it;
        if (previewTexMngr->GetTexture() == previewPartition.texture.get())
        {
            /* Remove layer */
            if (previewTexMngr->RemoveLayer(previewPartition.layer))
            {
                /* If this was the last layer -> remove preview texture manager */
                assetPreviewMngrs_.erase(it);
            }
            break;
        }
    }
}

UIAsset* AssetBrowserModel::AssetFromEntryPosition() const
{
    const auto assetIndex = selectionGrid_.SelectionIndex();
    return assetIndex < assets_.size() ? assets_[assetIndex].get() : nullptr;
}

UIAsset* AssetBrowserModel::MoveAndAccessEntryPosition(const Math::Vector2i& direction)
{
    selectionGrid_.MoveSelection(direction);
    return AssetFromEntryPosition();
}

const UIAssetPtr& AssetBrowserModel::AddAssetToList(const UIAssetPtr& asset)
{
    if (asset)
    {
        /* Add the asset to the list and update preview array texture MIP-maps */
        assets_.push_back(asset);
        asset->GenerateMIPMaps();
    }
    return asset;
}

void AssetBrowserModel::UpdateAssetPreview(UIAsset* asset)
{
    if (asset)
    {
        asset->DrawPreview();
        asset->GenerateMIPMaps();
    }
}

void AssetBrowserModel::ShowAssetAttributes()
{
    MainFrame::Instance()->GetAssetDesc()->HideAllAttribCategories();
    if (!selectedAssets_.empty())
        selectedAssets_.back()->ShowAttributes();
}


} // /namespace Editor

} // /namespace Fork



// ========================