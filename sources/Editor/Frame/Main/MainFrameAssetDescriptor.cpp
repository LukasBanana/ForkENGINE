/*
 * Main frame asset descriptor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameAssetDescriptor.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Core/WxHelper.h"
#include "../../Tools/AssetBrowser/Asset/UIAsset.h"


namespace Fork
{

namespace Editor
{


void MainFrameAssetDescriptor::HideAllAttribCategories()
{
    attribView_->HideAllCategories();
}


/*
 * ======= Protected: =======
 */

void MainFrameAssetDescriptor::BuildAttribView(wxWindow* parent)
{
    attribView_ = MakeWxObject<UIPropertyGrid>(parent, wxDefaultPosition, wxSize(400, 400));
    //attribView_->Enable(false);
    UIAsset::BuildPropertyGrid(*attribView_);
    attribView_->HideAllCategories();
}


} // /namespace Editor

} // /namespace Fork



// ========================