/*
 * Editor UI view manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIViewManager.h"
#include "Core/STLHelper.h"

#include <list>
#include <algorithm>


namespace Fork
{

namespace Editor
{


static std::list<UIViewManager*> globalUIViewMngrList;

UIViewManager::UIViewManager(wxWindow* managedWindow, long flags) :
    wxAuiManager{ managedWindow, flags }
{
    globalUIViewMngrList.push_back(this);
}
UIViewManager::~UIViewManager()
{
    RemoveFromList(globalUIViewMngrList, this);
}

int UIViewManager::ArtIconSize() const
{
    return GetArtProvider()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE);
}

void UIViewManager::ChangeColorScheme(const ColorScheme& scheme)
{
    /* Get AUI art provider */
    wxAuiDockArt* artProvider = GetArtProvider();

    if (!artProvider)
        return;

    /* Update color scheme */
    artProvider->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,           scheme.captionColor );
    artProvider->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,  scheme.gradientColor);
    artProvider->SetColour(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR,      scheme.cellTextColor);
    artProvider->SetColour(wxAUI_DOCKART_SASH_COLOUR,                       scheme.sashColor    );

    Update();
}

void UIViewManager::ChangeColorSchemeForAll(const ColorScheme& scheme)
{
    for (auto viewMngr : globalUIViewMngrList)
        viewMngr->ChangeColorScheme(scheme);
}


} // /namespace Editor

} // /namespace Fork



// ========================