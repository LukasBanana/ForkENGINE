/*
 * Main frame tool bar composition file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameToolBarComposition.h"
#include "MainFrameMenuBar.h"
#include "MainEventTable.h"
#include "MainFrame.h"
#include "../../Core/UIExtensions/UIToolBar.h"
#include "../../Core/UIExtensions/UIMenuList.h"
#include "../../Core/UIExtensions/UIComboBox.h"
#include "../../Core/WxHelper.h"
#include "../../Language/LanguageLoader.h"


namespace Fork
{

namespace Editor
{


MainFrameToolBarComposition::MainFrameToolBarComposition(wxWindow* parent)
{
    /* Create toolbar objects */
    for (auto& toolBar : toolBars_)
        toolBar = MakeWxObject<UIToolBar>(parent);

    /* Build toolbar layouts */
    BuildToolBarMain        (toolBars_[ ToolBars::Main      ]);
    BuildToolBarTransform   (toolBars_[ ToolBars::Transform ]);
    BuildToolBarView        (toolBars_[ ToolBars::View      ]);

    /* Finish toolbars */
    for (auto& toolBar : toolBars_)
        toolBar->Realize();
}

void MainFrameToolBarComposition::Show(bool show)
{
    for (auto& toolBar : toolBars_)
        toolBar->Show(show);
}


/*
 * ======= Private: =======
 */

void MainFrameToolBarComposition::BuildToolBarMain(UIToolBar* toolBar)
{
    /* Add toolbar items */
    toolBar->AddBitmapTool(MainMenuIDs::NewProject,     LANG_STRING( Menu_New_Project  ), wxART_NEW,       wxITEM_NORMAL, LANG_STRING( Menu_GeneratesNewProject        ), LANG_STRING( Menu_GeneratesNewProject        ));
    toolBar->AddBitmapTool(MainMenuIDs::OpenProject,    LANG_STRING( Menu_Open_Project ), wxART_FILE_OPEN, wxITEM_NORMAL, LANG_STRING( Menu_OpensProjectFromFile       ), LANG_STRING( Menu_OpensProjectFromFile       ));
    toolBar->AddBitmapTool(MainMenuIDs::SaveProject,    LANG_STRING( Menu_Save_Project ), wxART_FILE_SAVE, wxITEM_NORMAL, LANG_STRING( Menu_SavesCurrentProject        ), LANG_STRING( Menu_SavesCurrentProject        ));
    toolBar->AddSeparator();
    toolBar->AddBitmapTool(MainMenuIDs::Undo,           LANG_STRING( Menu_Undo         ), wxART_UNDO,      wxITEM_NORMAL, LANG_STRING( Menu_UndoPrevTask               ), LANG_STRING( Menu_UndoPrevTask               ));
    toolBar->AddBitmapTool(MainMenuIDs::Redo,           LANG_STRING( Menu_Redo         ), wxART_REDO,      wxITEM_NORMAL, LANG_STRING( Menu_RedoPrevTask               ), LANG_STRING( Menu_RedoPrevTask               ));
    toolBar->AddSeparator();
    toolBar->AddBitmapTool(MainMenuIDs::Cut,            LANG_STRING( Menu_Cut          ), wxART_CUT,       wxITEM_NORMAL, LANG_STRING( Menu_CutsSelectedObjects        ), LANG_STRING( Menu_CutsSelectedObjects        ));
    toolBar->AddBitmapTool(MainMenuIDs::Copy,           LANG_STRING( Menu_Copy         ), wxART_COPY,      wxITEM_NORMAL, LANG_STRING( Menu_CopysSelectedObjects       ), LANG_STRING( Menu_CopysSelectedObjects       ));
    toolBar->AddBitmapTool(MainMenuIDs::Paste,          LANG_STRING( Menu_Paste        ), wxART_PASTE,     wxITEM_NORMAL, LANG_STRING( Menu_PastesObjectsFromClipboard ), LANG_STRING( Menu_PastesObjectsFromClipboard ));

    /* Create drop down menus */
    MainFrameMenuBar::BuildMenuOpenRecent(toolBar->AddDropDownMenu(MainMenuIDs::OpenProject, true));
}

void MainFrameToolBarComposition::BuildToolBarTransform(UIToolBar* toolBar)
{
    /* Add toolbar items */
    toolBar->AddBitmapTool(MainMenuIDs::TransformTranslate,  "Translate",       wxBITMAP_PNG( PngTransformTranslate ), wxITEM_RADIO,  LANG_STRING( Menu_TranslatesAllSelectedEntities       ), LANG_STRING( Menu_TranslatesAllSelectedEntities       ));
    toolBar->AddBitmapTool(MainMenuIDs::TransformRotate,     "Rotate",          wxBITMAP_PNG( PngTransformRotate    ), wxITEM_RADIO,  LANG_STRING( Menu_RotatesAllSelectedEntities          ), LANG_STRING( Menu_RotatesAllSelectedEntities          ));
    toolBar->AddBitmapTool(MainMenuIDs::TransformScale,      "Scale",           wxBITMAP_PNG( PngTransformScale     ), wxITEM_RADIO,  LANG_STRING( Menu_ScalesAllSelectedEntities           ), LANG_STRING( Menu_ScalesAllSelectedEntities           ));
    toolBar->AddSeparator();
    toolBar->AddBitmapTool(MainMenuIDs::SelectorOrientation, "Orientation",     wxBITMAP_PNG( PngOrientationGlobal  ), wxITEM_NORMAL, LANG_STRING( Menu_ChangesSelectorOrientationAlignment ), LANG_STRING( Menu_ChangesSelectorOrientationAlignment ));
    toolBar->AddBitmapTool(MainMenuIDs::SelectorPivot,       "Pivot",           wxBITMAP_PNG( PngPivotMedianPoint   ), wxITEM_NORMAL, LANG_STRING( Menu_ChangesSelectorPivotAlignment       ), LANG_STRING( Menu_ChangesSelectorPivotAlignment       ));
    toolBar->AddBitmapTool(MainMenuIDs::TransformRaster,     "Raster",          wxBITMAP_PNG( PngTransformNoRaster  ), wxITEM_NORMAL, LANG_STRING( Menu_ConfiguresTheTransformationRaster   ), LANG_STRING( Menu_ConfiguresTheTransformationRaster   ));
    toolBar->AddSeparator();
    toolBar->AddBitmapTool(MainMenuIDs::EnableAxisX,         "EnableAxisX",     wxBITMAP_PNG( PngAxisX              ), wxITEM_CHECK,  LANG_STRING( Menu_EnablesTheAxesXYZ                   ), LANG_STRING( Menu_EnablesTheAxesXYZ                   ));
    toolBar->AddBitmapTool(MainMenuIDs::EnableAxisY,         "EnableAxisY",     wxBITMAP_PNG( PngAxisY              ), wxITEM_CHECK,  LANG_STRING( Menu_EnablesTheAxesXYZ                   ), LANG_STRING( Menu_EnablesTheAxesXYZ                   ));
    toolBar->AddBitmapTool(MainMenuIDs::EnableAxisZ,         "EnableAxisZ",     wxBITMAP_PNG( PngAxisZ              ), wxITEM_CHECK,  LANG_STRING( Menu_EnablesTheAxesXYZ                   ), LANG_STRING( Menu_EnablesTheAxesXYZ                   ));
    toolBar->AddSeparator();
    toolBar->AddBitmapTool(MainMenuIDs::SelectionEnable,     "EnableSelection", wxBITMAP_PNG( PngSelectionEnable    ), wxITEM_CHECK,  LANG_STRING( Menu_EnablesOrDisablesEntitySelection    ), LANG_STRING( Menu_EnablesOrDisablesEntitySelection    ));
    toolBar->AddBitmapTool(MainMenuIDs::TransformPinCamera,  "PinCamera",       wxBITMAP_PNG( PngTransformPinCamera ), wxITEM_CHECK,  LANG_STRING( Menu_PinsTheCameraWhenTranslatingEntites ), LANG_STRING( Menu_PinsTheCameraWhenTranslatingEntites ));
    toolBar->AddBitmapTool(MainMenuIDs::SelectorShow,        "ShowSelector",    wxBITMAP( BmpSelectorShow           ), wxITEM_CHECK,  LANG_STRING( Menu_ShowsOrHidesTheSelector             ), LANG_STRING( Menu_ShowsOrHidesTheSelector             ));

    /* Create drop down menus */
    MainFrameMenuBar::BuildMenuSelectorOrientation  (toolBar->AddDropDownMenu( MainMenuIDs::SelectorOrientation ));
    MainFrameMenuBar::BuildMenuSelectorPivot        (toolBar->AddDropDownMenu( MainMenuIDs::SelectorPivot       ));
    MainFrameMenuBar::BuildMenuRaster               (toolBar->AddDropDownMenu( MainMenuIDs::TransformRaster     ));
}

void MainFrameToolBarComposition::BuildToolBarView(UIToolBar* toolBar)
{
    /* Add toolbar items */
    toolBar->AddBitmapTool(MainMenuIDs::ViewRenderMode,  LANG_STRING( Menu_ViewRenderMode  ), wxBITMAP_PNG( PngRenderModeShaded ), wxITEM_NORMAL, LANG_STRING( Menu_ChangesTheRenderMode  ), LANG_STRING( Menu_ChangesTheRenderMode  ));
    toolBar->AddBitmapTool(MainMenuIDs::ViewEditorViews, LANG_STRING( Menu_ViewEditorViews ), wxBITMAP( BmpViewEditorViews      ), wxITEM_NORMAL, LANG_STRING( Menu_ShowsHidesEditorViews ), LANG_STRING( Menu_ShowsHidesEditorViews ));

    auto moveSpeedSpin = toolBar->AddSpinCtrl(50, wxCommandEventHandler(MainFrame::OnMoveSpeed), MainFrame::Instance());
    moveSpeedSpin->SetRange(1, 100);
    moveSpeedSpin->SetToolTip(MakeWxObject<wxToolTip>(LANG_STRING(Menu_CameraMoveSpeed)));

    /* Create drop down menus */
    MainFrameMenuBar::BuildMenuWireframe    (toolBar->AddDropDownMenu( MainMenuIDs::ViewRenderMode        ));
    MainFrameMenuBar::BuildMenuEditorViews  (toolBar->AddDropDownMenu( MainMenuIDs::ViewEditorViews, true ));
}


} // /namespace Editor

} // /namespace Fork



// ========================