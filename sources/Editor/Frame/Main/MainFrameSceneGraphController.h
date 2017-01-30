/*
 * Main frame scene graph controller header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_SCENE_GRAPH_CTRL_H__
#define __FORK_EDITOR_MAIN_FRAME_SCENE_GRAPH_CTRL_H__


#include <wx/window.h>
#include <vector>


namespace Fork
{

namespace Editor
{


class UITreeView;

class MainFrameSceneGraphController
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
        
        UITreeView* treeView_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================