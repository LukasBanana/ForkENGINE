/*
 * Editor asset preview mask file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AssetPreviewMask.h"
#include "../../Core/Devices.h"
#include "../../Core/Paths.h"


namespace Fork
{

namespace Editor
{


AssetPreviewMask::AssetPreviewMask()
{
    const std::string maskTexFilename = Paths::dirImages + "AssetPreviewMask.png";
    texture_ = Devices::renderSystem->GetTextureManager()->CreateTexture2DArrayFromFiles({ maskTexFilename });
}

void AssetPreviewMask::UpdateImageEntryPositions(const std::vector<AssetPreviewManagerPtr>& previewMngrs)
{
    /* Get total number of image entries */
    size_t numEntries = 0;

    for (const auto& mngr : previewMngrs)
        numEntries = mngr->GetImageEntries().size();

    /* Update image entry positions */
    imageEntries_.resize(numEntries);

    size_t i = 0;

    for (const auto& mngr : previewMngrs)
    {
        for (const auto& entry : mngr->GetImageEntries())
        {
            imageEntries_[i] = { entry.Position(), 0.0f, entry.Size(), entry.color };
            ++i;
        }
    }
}


} // /namespace Editor

} // /namespace Fork



// ========================