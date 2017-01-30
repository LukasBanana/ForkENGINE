/*
 * Main frame asset controller file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameAssetController.h"
#include "../../Core/UIExtensions/UITreeView.h"
#include "../../Core/WxHelper.h"
#include "Core/STLHelper.h"
#include "Core/StaticConfig.h"
#include "IO/Core/Log.h"

#include <wx/dataobj.h>
#include <wx/dnd.h>


namespace Fork
{

namespace Editor
{


/*
 * ======= Protected: =======
 */

void MainFrameAssetController::BuildTreeView(wxWindow* parent)
{
    treeView_ = MakeWxObject<UITreeView>(parent, wxDefaultPosition, wxSize(200, 400));
    treeView_->AddRoot("");
    treeView_->ExpandAll();

    treeView_->Bind(wxEVT_TREE_BEGIN_DRAG, &MainFrameAssetController::OnBeginDrag, this);
}


/*
 * ======= Private: =======
 */

void MainFrameAssetController::OnBeginDrag(wxTreeEvent& event)
{
    /* Prepare drag data object */
    auto itemText = treeView_->GetItemText(event.GetItem());
    wxTextDataObject data(itemText);

    /* Prepare drop source */
    wxDropSource source;
    source.SetData(data);

    /* Start drag/drop */
    auto result = source.DoDragDrop();
    #ifdef FORK_DEBUG
    if (result == wxDragError)
        IO::Log::Debug("Starting drag/drop failed (" __FUNCTION__ ")");
    #endif
}


} // /namespace Editor

} // /namespace Fork



// ========================