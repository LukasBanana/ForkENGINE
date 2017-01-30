/*
 * Editor tree view builder file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "TreeViewBuilder.h"
#include "UITreeView.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Editor
{


TreeViewBuilder::TreeViewBuilder(UITreeView* treeView) :
    treeView_{ treeView }
{
    ASSERT_POINTER(treeView);
    
    /*
    Set entry point to root entry;
    if there is no root item -> create one.
    */
    if (!treeView_->GetRootItem().IsOk())
        treeView_->AddRoot("");

    currentParent_ = prevItem_ = treeView_->GetRootItem();
}
TreeViewBuilder::TreeViewBuilder(UITreeView* treeView, const wxTreeItemId& entryPoint) :
    treeView_{ treeView }
{
    ASSERT_POINTER(treeView);

    /* Validate entry point */
    if (!entryPoint.IsOk())
        throw InvalidArgumentException(__FUNCTION__, "entryPoint", "'entryPoint.IsOk()' must not be <false>");

    currentParent_ = prevItem_ = entryPoint;
}

wxTreeItemId TreeViewBuilder::AddItem(const wxString& label, int image)
{
    /*
    Append new item to the current parent tree node
    and store it as the new 'previous' item
    */
    prevItem_ = treeView_->AppendItem(currentParent_, label, image);
    return prevItem_;
}

void TreeViewBuilder::PushParent()
{
    /*
    Push current parent tree node onto the stack
    and set previous item as the new parent tree node.
    */
    parentItemStack_.push(currentParent_);
    currentParent_ = prevItem_;
}

void TreeViewBuilder::PopParent()
{
    if (!parentItemStack_.empty())
    {
        /* Pop previous parrent tree node from the stack */
        currentParent_ = parentItemStack_.top();
        parentItemStack_.pop();
    }
}


} // /namespace Editor

} // /namespace Fork



// ========================