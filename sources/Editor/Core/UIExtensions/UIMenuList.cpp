/*
 * Editor UI menu list file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIMenuList.h"
#include "../WxHelper.h"


namespace Fork
{

namespace Editor
{


/*
 * ItemDescription strucutre
 */

UIMenuList::ItemDescription::ItemDescription(
    int id, const wxString& title, const wxString& helpText, wxItemKind kind) :
        id      { id       },
        title   { title    },
        helpText{ helpText },
        kind    { kind     }
{
}
UIMenuList::ItemDescription::ItemDescription(
    int id, const wxBitmap& bitmap, const wxString& title, const wxString& helpText) :
        id      { id       },
        title   { title    },
        helpText{ helpText },
        bitmap  { &bitmap  }
{
}
UIMenuList::ItemDescription::ItemDescription(
    int id, const wxArtID& bitmapId, const wxString& title, const wxString& helpText) :
        id      { id       },
        title   { title    },
        helpText{ helpText },
        bitmapId{ bitmapId }
{
}


/*
 * UIMenuList class
 */

UIMenuList::~UIMenuList()
{
}

wxMenuItem* UIMenuList::AddBitmapItem(
    int id, const wxArtID& bitmapId, const wxString& title, const wxString& helpText)
{
    auto item = MakeWxObject<wxMenuItem>(this, id, title, helpText);

    item->SetBitmap(wxArtProvider::GetBitmap(bitmapId, wxART_MENU, wxSize(16, 16)));
    Append(item);

    //UIItemController::RegisterMenuItem(this, id);
    menuItems_[id] = item;

    return item;
}

wxMenuItem* UIMenuList::AddBitmapItem(
    int id, const wxBitmap& bitmap, const wxString& title, const wxString& helpText)
{
    auto item = MakeWxObject<wxMenuItem>(this, id, title, helpText);

    item->SetBitmap(bitmap);
    Append(item);

    //UIItemController::RegisterMenuItem(this, id);
    menuItems_[id] = item;

    return item;
}

wxMenuItem* UIMenuList::AddItem(
    int id, const wxString& title, const wxString& helpText, wxItemKind kind)
{
    auto item = Append(id, title, helpText, kind);
    
    //UIItemController::RegisterMenuItem(this, id);
    menuItems_[id] = item;
    
    return item;
}

wxMenuItem* UIMenuList::FindItem(int id) const
{
    auto it = menuItems_.find(id);
    return it != menuItems_.end() ? it->second : nullptr;
}

void UIMenuList::AddItems(const std::initializer_list<ItemDescription>& itemDescList)
{
    for (auto desc : itemDescList)
    {
        if (desc.bitmap)
            AddBitmapItem(desc.id, *desc.bitmap, desc.title, desc.helpText);
        else if (!desc.bitmapId.empty())
            AddBitmapItem(desc.id, desc.bitmapId, desc.title, desc.helpText);
        else if (desc.title.empty())
            AppendSeparator();
        else
            AddItem(desc.id, desc.title, desc.helpText, desc.kind);
    }
}


} // /namespace Editor

} // /namespace Fork



// ========================