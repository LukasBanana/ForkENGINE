/*
 * Editor frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_FRAME_H__
#define __FORK_EDITOR_FRAME_H__


#include <wx/frame.h>
#include <wx/icon.h>


namespace Fork
{

namespace Editor
{


class EditorFrame : public wxFrame
{
    
    public:
        
        EditorFrame(
            wxWindow* parent, const wxString& title, const wxSize& size, const wxSize& minSize = wxSize(0, 0),
            /*const wxIcon& icon, */long style = wxDEFAULT_FRAME_STYLE
        );
        virtual ~EditorFrame();

        /* === Callback functions === */

        void OnClose(wxCloseEvent& event);

        /* === Functions === */

        virtual bool Show(bool show = true);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================