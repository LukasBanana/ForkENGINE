/*
 * Editor tree view builder header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_TREE_VIEW_BUILDER_H__
#define __FORK_EDITOR_TREE_VIEW_BUILDER_H__


#include <wx/treectrl.h>
#include <stack>


namespace Fork
{

namespace Editor
{


class UITreeView;

class TreeViewBuilder
{
    
    public:
        
        //! \throws NullPointerException If 'treeView' is null.
        TreeViewBuilder(UITreeView* treeView);
        /**
        \throws NullPointerException If 'treeView' is null.
        \throws InvalidArgumentException If 'entryPoint' is invalid.
        */
        TreeViewBuilder(UITreeView* treeView, const wxTreeItemId& entryPoint);

        wxTreeItemId AddItem(const wxString& label, int image = -1);

        void PushParent();
        void PopParent();

    private:
        
        UITreeView*                 treeView_ = nullptr;

        wxTreeItemId                currentParent_;
        wxTreeItemId                prevItem_;
        std::stack<wxTreeItemId>    parentItemStack_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================