/*
 * Editor asset browser model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ASSET_BROWSER_MODEL_H__
#define __FORK_EDITOR_ASSET_BROWSER_MODEL_H__


#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"
#include "Asset/UIAsset.h"
#include "AssetPreviewManager.h"
#include "AssetPreviewMask.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Utility/SelectionGridModel.h"

#include "Video/RenderSystem/Texture/Texture.h"
#include "Scene/Geometry/Node/MeshGeometry.h"
#include "Audio/SoundSystem/SoundSource.h"


namespace Fork
{

namespace Editor
{


/**
Editor asset browser model.
\note Inherit private from "SelectionGridModel" to hide its selection functionality.
Asset selection is only done via this asset browser model.
*/
class AssetBrowserModel
{
    
    public:
        
        AssetBrowserModel();

        /* --- Selection --- */

        bool IsAssetSelected(const UIAsset* asset) const;
        UIAsset* LastSelectedAsset() const;

        void SelectAsset(UIAsset* asset, bool select = true);
        void SelectWithModifier(UIAsset* asset);
        void Select(UIAsset* asset);
        void DeselectAllAssets();

        void SelectLeft();
        void SelectRight();
        void SelectUp();
        void SelectDown();

        void SelectRowBegin();
        void SelectRowEnd();

        void SelectColumnBegin();
        void SelectColumnEnd();

        void ProcessSelection(const Math::Point2f& pointerPosition, const Math::Point2f& offsetPosition);

        /* --- Asset instantiation --- */

        /**
        Adds the specified texture resource to the assets.
        \param[in] textureResource Shared pointer to the texture resource which is to be managed by this asset browser.
        \return Shared pointer to the new asset object or null if 'textureResource' is null.
        */
        UIAssetPtr AddTextureAsset(const Video::TexturePtr& textureResource, const std::string& assetLabel = "");
        /**
        Adds the specified mesh geometry resource to the assets.
        \param[in] geometryResource Shared pointer to the mesh geometry resource which is to be managed by this asset browser.
        \return Shared pointer to the new asset object
        */
        UIAssetPtr AddModelAsset(const Scene::GeometryPtr& geometryResource, const std::string& assetLabel = "");
        /**
        Adds the specified sound resource to the assets.
        \param[in] soundResource Shared pointer to the sound resource which is to be managed by this asset browser.
        \return Shared pointer to the new asset object.
        */
        UIAssetPtr AddSoundAsset(const Audio::SoundSourcePtr& soundResource, const std::string& assetLabel = "");

        /* --- Misc --- */

        /**
        Turns the selected asset if it's a model asset.
        \param[in] pitch Specifies the pitch angle (x-Axis).
        \param[in] yaw Specifies the yaw angle (y-Axis).
        \remarks If currently no model asset is selected, this function has no effect.
        */
        void TurnSelectedModelAsset(float pitch, float yaw);

        //! Returns the asset list.
        inline const std::vector<UIAssetPtr>& GetAssets() const
        {
            return assets_;
        }

        //! Returns the list of all selected assets.
        inline const std::vector<UIAsset*>& GetSelectedAssets() const
        {
            return selectedAssets_;
        }

        /* === Members === */

        //! Specifies whether the asset previews are to be draw with a mask or not. By default true.
        bool showPreviewMask = true;

        //! Asset preview selection color.
        Video::ColorRGBAf selectionColor { 1, 1, 0, 1 };

        /**
        Asset preview size.
        \todo Make this private.
        */
        Math::Size2f previewSize { 64.0f };

        //! Asset preview border,
        float previewBorder = 15.0f;

    protected:
        
        friend class UIAsset;
        friend class AssetBrowserView;
        friend class AssetBrowser;

        //! \todo Not yet implemented!
        void UnregisterAsset(UIAsset* asset);

        void UpdateAssetPreviewLocations(
            const Math::Size2f& viewportSize, Math::Point2f& lowestPosition, const Video::Font* labelFont
        );

        inline AssetPreviewMask* GetAssetPreviewMask() const
        {
            return assetPreviewMask_.get();
        }
        inline const std::vector<AssetPreviewManagerPtr>& GetAssetPreviewManagers() const
        {
            return assetPreviewMngrs_;
        }

    private:
        
        /* === Functions === */

        void CreatePreviewMask();

        UIAsset::PreviewPartition GenerateAssetPreviewPartition();
        void RemoveAssetPreviewPartition(const UIAsset::PreviewPartition& previewPartition);

        UIAsset* AssetFromEntryPosition() const;
        UIAsset* MoveAndAccessEntryPosition(const Math::Vector2i& direction);

        const UIAssetPtr& AddAssetToList(const UIAssetPtr& asset);
        void UpdateAssetPreview(UIAsset* asset);

        void ShowAssetAttributes();

        /* === Members === */

        Utility::SelectionGridModel         selectionGrid_;

        std::vector<AssetPreviewManagerPtr> assetPreviewMngrs_;
        std::vector<UIAssetPtr>             assets_;

        std::unique_ptr<AssetPreviewMask>   assetPreviewMask_;

        std::vector<UIAsset*>               selectedAssets_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================