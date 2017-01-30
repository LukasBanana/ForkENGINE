/*
 * Main frame asset descriptor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_ASSET_DESC_H__
#define __FORK_EDITOR_MAIN_FRAME_ASSET_DESC_H__


#include <wx/window.h>
#include <wx/textctrl.h>


namespace Fork
{

namespace Editor
{


class UIPropertyGrid;

class MainFrameAssetDescriptor
{
    
    public:
        
        inline UIPropertyGrid* GetAttribView() const
        {
            return attribView_;
        }

        void HideAllAttribCategories();

    protected:
        
        friend class MainFrame;

        void BuildAttribView(wxWindow* parent);

    private:
        
        UIPropertyGrid* attribView_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================