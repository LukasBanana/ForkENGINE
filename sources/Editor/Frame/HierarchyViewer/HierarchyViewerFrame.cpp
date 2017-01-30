/*
 * Editor hierarchy viewer frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "HierarchyViewerFrame.h"
#include "../../Core/StaticLayoutConfig.h"
#include "../../Core/WxHelper.h"
#include "../../Core/UIExtensions/UITreeView.h"


namespace Fork
{

namespace Editor
{


using namespace StaticLayout::HierarchyViewerFrameLayout;

HierarchyViewerFrame::HierarchyViewerFrame(const wxString& caption, wxWindow* parent) :
    EditorFrame{ parent, caption, defaultSize, minSize }
{
    BuildFrameLayout();
}

void HierarchyViewerFrame::ClearHierarchy()
{
    treeView_->DeleteAllItems();
}


/*
 * ======= Private: =======
 */

bool HierarchyViewerFrame::BuildFrameLayout()
{
    treeView_ = MakeWxObject<UITreeView>(this);
    statusBar_ = CreateStatusBar(2);
    return true;
}


} // /namespace Editor

} // /namespace Fork



// ========================