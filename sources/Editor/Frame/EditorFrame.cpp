/*
 * Editor frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EditorFrame.h"


namespace Fork
{

namespace Editor
{


EditorFrame::EditorFrame(
    wxWindow* parent, const wxString& title, const wxSize& size,
    const wxSize& minSize, /*const wxIcon& icon, */long style) :
        wxFrame(parent, wxID_ANY, title, wxDefaultPosition, size, style)
{
    Bind(wxEVT_CLOSE_WINDOW, &EditorFrame::OnClose, this);
    
    if (minSize.GetWidth() != 0 && minSize.GetHeight() != 0)
        SetSizeHints(minSize);

    //SetIcon(wxICON(IcoForkEdLogo));

    Center();
}
EditorFrame::~EditorFrame()
{
}

void EditorFrame::OnClose(wxCloseEvent& event)
{
    event.Veto();
    Show(false);
}

bool EditorFrame::Show(bool show)
{
    bool result = wxWindow::Show(show);
    
    if (show)
        Raise();
    
    return result;
}


} // /namespace Editor

} // /namespace Fork



// ========================