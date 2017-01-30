/*
 * Editor hierarchy viewer frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_HIERARCHY_VIEWER_FRAME_H__
#define __FORK_EDITOR_HIERARCHY_VIEWER_FRAME_H__


#include "../EditorFrame.h"

#include <wx/statusbr.h>


namespace Fork
{

namespace Editor
{


class UITreeView;

class HierarchyViewerFrame : public EditorFrame
{
    
    public:
        
        HierarchyViewerFrame(const wxString& caption, wxWindow* parent = nullptr);

        //! Clears all items from the hierarchy tree view.
        void ClearHierarchy();

        inline UITreeView* GetTreeView() const
        {
            return treeView_;
        }

    private:
        
        bool BuildFrameLayout();

        wxStatusBar*    statusBar_  = nullptr;
        UITreeView*     treeView_   = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================