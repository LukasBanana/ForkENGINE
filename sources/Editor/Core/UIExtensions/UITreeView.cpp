/*
 * Editor UI tree view file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UITreeView.h"


namespace Fork
{

namespace Editor
{


/*
 * Entry class
 */

UITreeView::Entry::Entry(
    UITreeView* treeView, const wxTreeItemId& parent, const wxString& text, int image, int selImage) :
        treeView_   { treeView                                            },
        item_       { treeView->AppendItem(parent, text, image, selImage) }
{
}
UITreeView::Entry::~Entry()
{
    treeView_->Delete(item_);
}

void UITreeView::Entry::ChangeText(const wxString& text)
{
    treeView_->SetItemText(item_, text);
}


/*
 * UITreeView class
 */

UITreeView::UITreeView(
    wxWindow* parent, const wxPoint& position, const wxSize& size, long style) :
        wxTreeCtrl{ parent, wxID_ANY, position, size, style }
{
}
UITreeView::~UITreeView()
{
}

std::unique_ptr<UITreeView::Entry> UITreeView::CreateEntry(const wxString& text, int image, int selImage)
{
    /*
    Create "Entry" object manually with 'new' because the constructor is private.
    This is an exception to the memory allocation convention in this engine!
    */
    return GetRootItem().IsOk() ?
        std::unique_ptr<Entry>(new Entry(this, GetRootItem(), text, image, selImage)) :
        nullptr;
}


} // /namespace Editor

} // /namespace Fork



// ========================