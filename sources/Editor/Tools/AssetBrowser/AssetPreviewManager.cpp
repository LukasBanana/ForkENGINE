/*
 * Editor asset preview manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AssetPreviewManager.h"
#include "../../Core/Devices.h"
#include "Core/STLHelper.h"

#include <algorithm>


namespace Fork
{

namespace Editor
{


using namespace Video::StandardShader;

const unsigned int AssetPreviewManager::maxNumLayers = ImageArrayDrawingShader::EntryConstBuffer::maxNumImages;

AssetPreviewManager::AssetPreviewManager(const Video::Texture2D::SizeType& textureSize)
{
    texture_ = Devices::renderSystem->CreateTexture2D();
    Devices::renderSystem->WriteTexture(
        GetTexture(), Video::TextureFormats::RGB,
        textureSize, AssetPreviewManager::maxNumLayers,
        Video::ImageColorFormats::RGB, Video::RendererDataTypes::UByte, nullptr
    );
}

bool AssetPreviewManager::GenerateLayer(unsigned int& layer)
{
    if (reservedLayers_.size() == AssetPreviewManager::maxNumLayers)
    {
        /* Cancel if number of layers already reached the maximum */
        return false;
    }

    if (reservedLayers_.empty())
    {
        /* Add first layer */
        layer = 0;
        reservedLayers_.push_back(layer);
        return true;
    }
    
    if (reservedLayers_.back() + 1 < AssetPreviewManager::maxNumLayers)
    {
        /* Add last layer */
        layer = reservedLayers_.back() + 1;
        reservedLayers_.push_back(layer);
        return true;
    }

    /* Search first free layer inside the layer list */
    unsigned int prevLayer = 0;

    for (auto it = reservedLayers_.begin(); it != reservedLayers_.end(); ++it)
    {
        if (prevLayer + 1 < *it)
        {
            layer = prevLayer + 1;
            reservedLayers_.insert(it, layer);
            break;
        }
        prevLayer = *it;
    }

    return false;
}

bool AssetPreviewManager::RemoveLayer(unsigned int layer)
{
    RemoveFromList(reservedLayers_, layer);
    return reservedLayers_.empty();
}

void AssetPreviewManager::UpdateImageEntryPositions(
    ext::const_range_forward_iterator<std::vector<UIAssetPtr>>& assetIterator,
    Math::Point2f& position, Math::Size2i& grid, const Math::Size2f& viewportSize,
    const Math::Size2f& previewSize, float border)
{
    grid = { 0, 0 };
    imageEntries_.resize(reservedLayers_.size());
    int maxLabelHeight = 0;

    for (size_t i = 0, n = reservedLayers_.size(); i < n; ++i)
    {
        /* Check if a new row must be added */
        if (position.x + previewSize.width + border > viewportSize.width)
        {
            /* Increase position Y-axis and grid height */
            position.x = border;
            position.y += previewSize.height + border + static_cast<float>(maxLabelHeight);
            ++grid.height;
            maxLabelHeight = 0;
        }

        /* Store highest asset label */
        auto asset = assetIterator->get();
        ++assetIterator;

        auto labelHeight = asset->LabelHeight();
        maxLabelHeight = std::max(maxLabelHeight, labelHeight);

        /* Store current image entry */
        auto layer = reservedLayers_[i];
        imageEntries_[i] = { position, static_cast<float>(layer) };

        /* Increase position */
        position.x += previewSize.width + border;

        /* Increase grid width */
        if (grid.height == 0)
            ++grid.width;
    }

    if (!reservedLayers_.empty())
        ++grid.height;

    position.y += previewSize.height + border + static_cast<float>(maxLabelHeight);
}

void AssetPreviewManager::GenerateMIPMaps()
{
    Video::RenderContext::Active()->GenerateMIPMaps(texture_.get());
}

void AssetPreviewManager::GenerateMIPMaps(unsigned int layer)
{
    Devices::customMIPMapsGenerator->GenerateMIPMaps(texture_.get(), layer);
}


} // /namespace Editor

} // /namespace Fork



// ========================