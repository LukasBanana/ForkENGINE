/*
 * Main frame ribbon bar file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameRibbonBar.h"
#include "MainEventTable.h"
#include "../../Core/WxHelper.h"
#include "../../Core/WxObjectIDs.h"
#include "../../Language/LanguageLoader.h"
#include "../../Core/UIExtensions/UIRibbonToolBar.h"


namespace Fork
{

namespace Editor
{


MainFrameRibbonBar::MainFrameRibbonBar(wxWindow* parent) :
    wxRibbonBar
    {
        parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS | wxRIBBON_BAR_SHOW_TOGGLE_BUTTON
    }
{
    /* Build toolbar layouts */
    BuildPageMain   (MakeWxObject<wxRibbonPage>(this, wxID_ANY, LANG_STRING( Ribbon_Edit    )));
    BuildPageTerrain(MakeWxObject<wxRibbonPage>(this, wxID_ANY, LANG_STRING( Ribbon_Terrain )));
    BuildPageGame   (MakeWxObject<wxRibbonPage>(this, wxID_ANY, LANG_STRING( Ribbon_Game    )));

    Realize();
}


/*
 * ======= Private: =======
 */

static wxBitmap StdBitmap(const wxArtID& id)
{
    return wxArtProvider::GetBitmap(id, wxART_OTHER, wxSize(16, 16));
};

static UIRibbonToolBar* CreateRibbonBar(wxRibbonPanel* panel)
{
    static wxWindowID winIdCounter = WxObjectIDs::RibbonEditorToolbar;
    return MakeWxObject<UIRibbonToolBar>(panel, winIdCounter++);
}

void MainFrameRibbonBar::BuildPageMain(wxRibbonPage* page)
{
    BuildPageMainPanelToolbar       (MakeWxObject<wxRibbonPanel>(page, wxID_ANY, LANG_STRING( Ribbon_Edit_Toolbar        )));
    BuildPageMainPanelTransformation(MakeWxObject<wxRibbonPanel>(page, wxID_ANY, LANG_STRING( Ribbon_Edit_Transformation )));
    BuildPageMainPanelView          (MakeWxObject<wxRibbonPanel>(page, wxID_ANY, LANG_STRING( Ribbon_Edit_View           )));
}

void MainFrameRibbonBar::BuildPageTerrain(wxRibbonPage* page)
{
}

void MainFrameRibbonBar::BuildPageGame(wxRibbonPage* page)
{
}

void MainFrameRibbonBar::BuildPageMainPanelToolbar(wxRibbonPanel* panel)
{
    auto toolBar = CreateRibbonBar(panel);

    toolBar->AddTool(MainMenuIDs::NewProject,       StdBitmap( wxART_NEW          ), LANG_STRING( Menu_GeneratesNewProject                 ));
    toolBar->AddTool(MainMenuIDs::OpenProject,      StdBitmap( wxART_FILE_OPEN    ), LANG_STRING( Menu_OpensProjectFromFile                ));
    toolBar->AddTool(MainMenuIDs::SaveProject,      StdBitmap( wxART_FILE_SAVE    ), LANG_STRING( Menu_SavesCurrentProject                 ));
    toolBar->AddTool(MainMenuIDs::SaveProjectAs,    StdBitmap( wxART_FILE_SAVE_AS ), LANG_STRING( Menu_SavesCurrentProjectUnderNewFilename ));
    toolBar->AddTool(MainMenuIDs::ProjectSettings,  StdBitmap( wxART_REPORT_VIEW  ), LANG_STRING( Menu_OpenProjectSettingsWindow           ));
    toolBar->AddSeparator();
    toolBar->AddTool(MainMenuIDs::Undo,             StdBitmap( wxART_UNDO         ), LANG_STRING( Menu_UndoPrevTask                        ));
    toolBar->AddTool(MainMenuIDs::Redo,             StdBitmap( wxART_REDO         ), LANG_STRING( Menu_RedoPrevTask                        ));
    toolBar->AddSeparator();
    toolBar->AddTool(MainMenuIDs::Cut,              StdBitmap( wxART_CUT          ), LANG_STRING( Menu_CutsSelectedObjects                 ));
    toolBar->AddTool(MainMenuIDs::Copy,             StdBitmap( wxART_COPY         ), LANG_STRING( Menu_CopysSelectedObjects                ));
    toolBar->AddTool(MainMenuIDs::Paste,            StdBitmap( wxART_PASTE        ), LANG_STRING( Menu_PastesObjectsFromClipboard          ));

    toolBar->SetRows(2, 3);

    toolBar->Realize();
}

void MainFrameRibbonBar::BuildPageMainPanelTransformation(wxRibbonPanel* panel)
{
    auto toolBar = CreateRibbonBar(panel);

    toolBar->AddTool        (MainMenuIDs::TransformTranslate,  wxBITMAP_PNG( PngTransformTranslate32 ), LANG_STRING( Menu_TranslatesAllSelectedEntities       ), wxRIBBON_BUTTON_TOGGLE);
    toolBar->AddTool        (MainMenuIDs::TransformRotate,     wxBITMAP_PNG( PngTransformRotate32    ), LANG_STRING( Menu_RotatesAllSelectedEntities          ), wxRIBBON_BUTTON_TOGGLE);
    toolBar->AddTool        (MainMenuIDs::TransformScale,      wxBITMAP_PNG( PngTransformScale32     ), LANG_STRING( Menu_ScalesAllSelectedEntities           ), wxRIBBON_BUTTON_TOGGLE);
    toolBar->AddSeparator();
    toolBar->AddDropdownTool(MainMenuIDs::SelectorOrientation, wxBITMAP_PNG( PngOrientationGlobal    ), LANG_STRING( Menu_ChangesSelectorOrientationAlignment ));
    toolBar->AddDropdownTool(MainMenuIDs::SelectorPivot,       wxBITMAP_PNG( PngPivotMedianPoint     ), LANG_STRING( Menu_ChangesSelectorPivotAlignment       ));
    toolBar->AddDropdownTool(MainMenuIDs::TransformRaster,     wxBITMAP_PNG( PngTransformNoRaster    ), LANG_STRING( Menu_ConfiguresTheTransformationRaster   ));

    toolBar->SetRows(2, 3);

    toolBar->Realize();
}

void MainFrameRibbonBar::BuildPageMainPanelView(wxRibbonPanel* panel)
{
}


} // /namespace Editor

} // /namespace Fork



// ========================