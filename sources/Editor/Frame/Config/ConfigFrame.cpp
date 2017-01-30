/*
 * Editor config frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConfigFrame.h"
#include "ConfigPanelCommon.h"
#include "ConfigPanelUI.h"
#include "ConfigPanelGraphics.h"

#include "../../Core/StaticLayoutConfig.h"
#include "../../Core/WxHelper.h"
#include "../../Language/LanguageLoader.h"

#include <wx/bookctrl.h>


namespace Fork
{

namespace Editor
{


using namespace StaticLayout::ConfigFrameLayout;

ConfigFrame::ConfigFrame(wxWindow* parent, long style) :
    wxPropertySheetDialog
    {
        parent, wxID_ANY, LANG_STRING( EditorPreferences ),
        wxDefaultPosition, defaultSize, style
    }
{
    BuildFrameLayout();
    
    SetSizeHints(StaticLayout::ConfigFrameLayout::minSize);

    Bind(wxEVT_CLOSE_WINDOW, &ConfigFrame::OnClose, this);

    SetSize(defaultSize); // <-- Window does not has correct initial size without this???
    Center();
}

void ConfigFrame::OnClose(wxCloseEvent& event)
{
    event.Veto();
    Show(false);
}

bool ConfigFrame::Show(bool show)
{
    bool result = wxWindow::Show(show);
    
    if (show)
        Raise();
    
    return result;
}


/*
 * ======= Private: =======
 */

void ConfigFrame::BuildFrameLayout()
{
    CreateButtons(wxOK | wxCANCEL);

    auto bookCtrl = GetBookCtrl();
    {
        /* Panel "common" */
        panelCommon_ = MakeWxObject<ConfigPanelCommon>(bookCtrl);
        bookCtrl->AddPage(panelCommon_, LANG_STRING(Common), true);

        /* Panel "UI" */
        panelUI_ = MakeWxObject<ConfigPanelUI>(bookCtrl);
        bookCtrl->AddPage(panelUI_, LANG_STRING(UserInterface));

        /* Panel "graphics" */
        panelGraphics_ = MakeWxObject<ConfigPanelGraphics>(bookCtrl);
        bookCtrl->AddPage(panelGraphics_, LANG_STRING(Graphics));
    }
    LayoutDialog();
}


} // /namespace Editor

} // /namespace Fork



// ========================