/*
 * Main frame scene graph controller file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameSceneGraphController.h"
#include "../../Core/UIExtensions/UITreeView.h"
#include "../../Core/WxHelper.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Editor
{


/*
 * ======= Protected: =======
 */

void MainFrameSceneGraphController::BuildTreeView(wxWindow* parent)
{
    treeView_ = MakeWxObject<UITreeView>(parent, wxDefaultPosition, wxSize(200, 400));
    treeView_->AddRoot("");
    treeView_->ExpandAll();
}


} // /namespace Editor

} // /namespace Fork



// ========================