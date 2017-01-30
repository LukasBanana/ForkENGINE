/*
 * Editor asset preview manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ASSET_PREVIEW_MANAGER_H__
#define __FORK_EDITOR_ASSET_PREVIEW_MANAGER_H__


#include "Core/DeclPtr.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/AdvancedRenderer/ImageArrayContainer.h"
#include "Asset/UIAsset.h"
#include "Plugins/cpplibext/range_iterator.hpp"


namespace Fork
{

namespace Editor
{


DECL_SHR_PTR(AssetPreviewManager);

//! This class manages a single preview array texture.
class AssetPreviewManager : public Video::ImageArrayContainer
{
    public:
        
        static const unsigned int maxNumLayers;

        AssetPreviewManager(const Video::Texture2D::SizeType& textureSize);

        /**
        Generates a new layer.
        \param[out] layer Specifies the resulting layer index. This is in the range [0 .. maxNumLayers - 1].
        \return True if a layer could be generated. Otherwise this array texture is full.
        */
        bool GenerateLayer(unsigned int& layer);
        /**
        Removes the specified layer index from the list.
        \param[in] layer Specifies the layer index which is to be removed.
        \return True if this was the last layer index.
        */
        bool RemoveLayer(unsigned int layer);

        /**
        Updates the position of all image entries.
        \param[in] assets Specifies the list of all assets form the entire asset browser model.
        \param[in,out] position Specifies the start position and it will receive the end position.
        \param[out] grid Specifies the resulting entry grid.
        \param[in] viewportSize Specifies the viewport size.
        \param[in] border Specifies the border between the preview images.
        \see GetImageEntries
        \see AssetBrowserModel::GetAssets
        */
        void UpdateImageEntryPositions(
            ext::const_range_forward_iterator<std::vector<UIAssetPtr>>& assetIterator,
            Math::Point2f& position, Math::Size2i& grid, const Math::Size2f& viewportSize,
            const Math::Size2f& previewSize, float border = 15.0f
        );

        //! Generates the MIP-maps for the preview array texture.
        void GenerateMIPMaps();
        //! Generates the MIP-maps only for the specified layer in the preview array texture.
        void GenerateMIPMaps(unsigned int layer);

        //! Returns a shared pointer to the 2D texture object.
        inline const Video::Texture2DPtr& GetSharedTexture() const
        {
            return texture_;
        }

    private:

        std::vector<unsigned int> reservedLayers_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================