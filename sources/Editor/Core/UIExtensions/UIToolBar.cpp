/*
 * Editor UI toolbar file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIToolBar.h"
#include "UIComboBox.h"
#include "UIMenuList.h"
#include "../WxHelper.h"


namespace Fork
{

namespace Editor
{


UIToolBar::UIToolBar(
    wxWindow* parent, const wxPoint& position, const wxSize& size, long style) :
        wxAuiToolBar(parent, wxID_ANY, position, size, style)
{
}
UIToolBar::~UIToolBar()
{
}

wxAuiToolBarItem* UIToolBar::AddBitmapTool(
    int itemId, const wxString& label, const wxBitmap& bitmap, wxItemKind kind,
    const wxString& shortHelpText, const wxString& longHelpText)
{
    return AddTool(itemId, label, bitmap, wxNullBitmap, kind, shortHelpText, longHelpText, 0);
    //UIItemController::RegisterToolItem(this, itemId);
}

wxAuiToolBarItem* UIToolBar::AddBitmapTool(
    int itemId, const wxString& label, const wxArtID& artId, wxItemKind kind,
    const wxString& shortHelpText, const wxString& longHelpText)
{
    return AddBitmapTool(itemId, label, wxArtProvider::GetBitmap(artId, wxART_MENU), kind, shortHelpText, longHelpText);
    //UIItemController::RegisterToolItem(this, itemId);
}

wxSearchCtrl* UIToolBar::AddSearchField(int size, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink, const wxString& hint)
{
    auto searchField = MakeWxObject<wxSearchCtrl>(
        this, wxID_ANY, "", wxDefaultPosition, wxSize(size, wxDefaultSize.GetHeight())
    );
    
    searchField->SetDescriptiveText(hint);
    
    if (eventSink)
        searchField->Connect(wxEVT_COMMAND_TEXT_UPDATED, callbackProc, 0, eventSink);
    
    AddControl(searchField);
    
    return searchField;
}

wxSpinCtrl* UIToolBar::AddSpinCtrl(int size, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink)
{
    auto spinCtrl = MakeWxObject<wxSpinCtrl>(
        this, wxID_ANY, "", wxDefaultPosition, wxSize(size, wxDefaultSize.GetHeight())
    );
    
    if (eventSink)
        spinCtrl->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, callbackProc, 0, eventSink);
    
    AddControl(spinCtrl);
    
    return spinCtrl;
}

wxSlider* UIToolBar::AddSlider(int size, int value, int min, int max, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink)
{
    auto sliderCtrl = MakeWxObject<wxSlider>(
        this, wxID_ANY, value, min, max, wxDefaultPosition, wxSize(size, wxDefaultSize.GetHeight())
    );
    
    if (eventSink)
        sliderCtrl->Connect(wxEVT_COMMAND_SLIDER_UPDATED, callbackProc, 0, eventSink);
    
    AddControl(sliderCtrl);
    
    return sliderCtrl;
}

UIComboBox* UIToolBar::AddComboBox(int size, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink, long style)
{
    auto comboBox = MakeWxObject<UIComboBox>(this, wxDefaultPosition, wxSize(size, wxDefaultSize.GetHeight()), "", style);
    
    if (eventSink)
        comboBox->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, callbackProc, 0, eventSink);
    
    AddControl(comboBox);
    
    return comboBox;
}

UIMenuList* UIToolBar::AddDropDownMenu(int itemId, bool isButtonExt)
{
    auto dropDownMenu = FindDropDownMenu(itemId);

    if (!dropDownMenu.menuList)
    {
        /* Create new menu list and store it inside the map */
        auto menuList = MakeWxObject<UIMenuList>();
        dropDownMenus_[itemId] = { menuList, isButtonExt };

        /* Make the specified item to a drop-down item */
        SetToolDropDown(itemId, true);

        return menuList;
    }

    return dropDownMenu.menuList;
}

UIToolBar::DropDownMenu UIToolBar::FindDropDownMenu(int itemId) const
{
    auto it = dropDownMenus_.find(itemId);
    return it != dropDownMenus_.end() ? it->second : DropDownMenu();
}

bool UIToolBar::ShowDropDownMenu(wxFrame* window, int itemId, UIMenuList* dropDownMenu)
{
    if (!window)
        return false;
    
    /* Get drop down menu */
    if (!dropDownMenu)
    {
        dropDownMenu = FindDropDownMenu(itemId).menuList;
        if (!dropDownMenu)
            return false;
    }

    /* Popup position */
    SetToolSticky(itemId, true);
    
    auto rect   = GetToolRect(itemId);
    auto point  = ClientToScreen(rect.GetBottomLeft());
    
    point = window->ScreenToClient(point);
    
    /* Show dropdown menu */
    window->PopupMenu(dropDownMenu, point);
    
    SetToolSticky(itemId, false);

    return true;
}

void UIToolBar::SwitchItem(int toolId, bool state)
{   
    ToggleTool(toolId, state);
    Refresh();
}

wxBitmap UIToolBar::GetBitmap(const wxArtID &Id)
{
    return wxArtProvider::GetBitmap(Id, wxART_MENU, wxSize(16, 16));
}


} // /namespace Editor

} // /namespace Fork



// ========================