/*
 * Editor UI toolbar header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_TOOL_BAR_H__
#define __FORK_EDITOR_UI_TOOL_BAR_H__


#include <wx/srchctrl.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/frame.h>
#include <wx/aui/auibar.h>
#include <map>


namespace Fork
{

namespace Editor
{


class UIComboBox;
class UIMenuList;

class UIToolBar : public wxAuiToolBar
{
    
    public:
        
        //! Toolbar drop down menu structure.
        struct DropDownMenu
        {
            DropDownMenu() = default;
            DropDownMenu(UIMenuList* menuList, bool isButtonExt = false) :
                menuList    { menuList    },
                isButtonExt { isButtonExt }
            {
            }

            UIMenuList* menuList    = nullptr;
            /**
            Specifies whether the toolbar item button has extended functionality.
            If false, only the drop down menu will appear when the button is pushed.
            */
            bool        isButtonExt = false;
        };

        UIToolBar(
            wxWindow* parent, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL
        );
        virtual ~UIToolBar();

        wxAuiToolBarItem* AddBitmapTool(
            int itemId, const wxString& label, const wxBitmap& bitmap,
            wxItemKind kind = wxITEM_NORMAL, const wxString& shortHelpText = "", const wxString& longHelpText = ""
        );
        wxAuiToolBarItem* AddBitmapTool(
            int itemId, const wxString& label, const wxArtID& artId,
            wxItemKind kind = wxITEM_NORMAL, const wxString& shortHelpText = "", const wxString& longHelpText = ""
        );

        wxSearchCtrl* AddSearchField(int size, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink, const wxString& hint);
        wxSpinCtrl* AddSpinCtrl(int size, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink);
        wxSlider* AddSlider(int size, int value, int min, int max, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink);

        UIComboBox* AddComboBox(
            int size, wxObjectEventFunction callbackProc, wxEvtHandler* eventSink,
            long style = wxCB_DROPDOWN | wxCB_READONLY
        );

        //! Creates a new drop down menu.
        UIMenuList* AddDropDownMenu(int itemId, bool isButtonExt = false);
        //! Returns the drop down menu for the specified item ID.
        DropDownMenu FindDropDownMenu(int itemId) const;

        /**
        Shows the specified drop down menu onto this toolbar.
        If 'dropDownMenu' is null, the respective menu is searched by the item ID.
        */
        bool ShowDropDownMenu(wxFrame* window, int itemId, UIMenuList* dropDownMenu = nullptr);
        
        //! Toggles an items and refreshes the toolbar.
        void SwitchItem(int toolId, bool state = true);

        static wxBitmap GetBitmap(const wxArtID &Id);

    private:

        std::map<int, DropDownMenu> dropDownMenus_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================