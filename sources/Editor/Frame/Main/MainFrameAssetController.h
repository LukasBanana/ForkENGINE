/*
 * Main frame asset controller header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_ASSET_CTRL_H__
#define __FORK_EDITOR_MAIN_FRAME_ASSET_CTRL_H__


#include <wx/window.h>
#include <wx/treectrl.h>
#include <vector>


namespace Fork
{

namespace Editor
{


class UITreeView;

class MainFrameAssetController
{
    
    public:
        
        inline UITreeView* GetTreeView() const
        {
            return treeView_;
        }

    protected:

        friend class MainFrame;

        void BuildTreeView(wxWindow* parent);

    private:
        
        void OnBeginDrag(wxTreeEvent& event);

        UITreeView* treeView_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================