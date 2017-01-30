/*
 * Editor UI menu list header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_MENU_LIST_H__
#define __FORK_EDITOR_UI_MENU_LIST_H__


#include <wx/menu.h>
#include <wx/artprov.h>

#include <map>
#include <initializer_list>


namespace Fork
{

namespace Editor
{


class UIMenuList : public wxMenu
{
    
    public:
        
        struct ItemDescription
        {
            ItemDescription() = default;
            ItemDescription(int id, const wxString& title, const wxString& helpText = "", wxItemKind kind = wxITEM_NORMAL);
            ItemDescription(int id, const wxBitmap& bitmap, const wxString& title, const wxString& helpText = "");
            ItemDescription(int id, const wxArtID& bitmapId, const wxString& title, const wxString& helpText);

            int             id          = 0;
            wxString        title;
            wxString        helpText    = "";
            wxItemKind      kind        = wxITEM_NORMAL;
            const wxBitmap* bitmap      = nullptr;
            wxArtID         bitmapId;
        };

        virtual ~UIMenuList();

        wxMenuItem* AddBitmapItem(int id, const wxArtID& bitmapId, const wxString& title, const wxString& helpText = "");
        wxMenuItem* AddBitmapItem(int id, const wxBitmap& bitmap, const wxString& title, const wxString& helpText = "");
        
        wxMenuItem* AddItem(int id, const wxString& title, const wxString& helpText = "", wxItemKind kind = wxITEM_NORMAL);
        
        //! Returns the menu item with the given ID.
        wxMenuItem* FindItem(int id) const;

        void AddItems(const std::initializer_list<ItemDescription>& itemDescList);

    private:
        
        std::map<int, wxMenuItem*> menuItems_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================