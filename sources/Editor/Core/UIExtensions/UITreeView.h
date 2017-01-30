/*
 * Editor UI tree view header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_TREE_VIEW_H__
#define __FORK_EDITOR_UI_TREE_VIEW_H__


#include <memory>
#include <wx/treectrl.h>


namespace Fork
{

namespace Editor
{


class UITreeView : public wxTreeCtrl
{
    
    public:
        
        /**
        Tree view entry class. This item will be removed
        from its tree view if this object is destroyed.
        */
        class Entry
        {
            
            public:
                
                ~Entry();

                void ChangeText(const wxString& text);

                inline const wxTreeItemId& GetItem() const
                {
                    return item_;
                }
                inline UITreeView* GetTreeView() const
                {
                    return treeView_;
                }

            protected:
                
                friend class UITreeView;

                Entry(
                    UITreeView* treeView, const wxTreeItemId& parent,
                    const wxString& text, int image, int selImage
                );

            private:
                
                UITreeView*     treeView_;
                wxTreeItemId    item_;

        };

        UITreeView(
            wxWindow* parent, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = (wxTR_NO_LINES | wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT | wxTR_HAS_BUTTONS | wxTR_TWIST_BUTTONS | wxTR_FULL_ROW_HIGHLIGHT)
        );
        virtual ~UITreeView();

        /**
        Creates a new entry for this tree view.
        \return The new entry object or null if this tree view has no root item.
        */
        std::unique_ptr<Entry> CreateEntry(const wxString& text, int image = -1, int selImage = -1);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================