/*
 * Editor asset preview mask header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ASSET_PREVIEW_MASK_H__
#define __FORK_EDITOR_ASSET_PREVIEW_MASK_H__


#include "Core/DeclPtr.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/AdvancedRenderer/ImageArrayContainer.h"
#include "AssetPreviewManager.h"


namespace Fork
{

namespace Editor
{


class AssetPreviewMask : public Video::ImageArrayContainer
{
    public:

        AssetPreviewMask();

        void UpdateImageEntryPositions(const std::vector<AssetPreviewManagerPtr>& previewMngrs);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================