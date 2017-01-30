/*
 * Main frame menu bar file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameMenuBar.h"
#include "MainEventTable.h"
#include "../../Core/UIExtensions/UIMenuList.h"
#include "../../Language/LanguageLoader.h"
#include "../../Core/WxHelper.h"

#include <wx/artprov.h>


namespace Fork
{

namespace Editor
{


MainFrameMenuBar::MainFrameMenuBar()
{
    for (auto& menuList : menuLists_)
        menuList = MakeWxObject<UIMenuList>();

    BuildMenuListEditor (MenuList( MenuLists::Editor  ));
    BuildMenuListProject(MenuList( MenuLists::Project ));
    BuildMenuListEntity (MenuList( MenuLists::Entity  ));
    BuildMenuListEdit   (MenuList( MenuLists::Edit    ));
    BuildMenuListHelp   (MenuList( MenuLists::Help    ));
}

UIMenuList* MainFrameMenuBar::MenuList(const MenuLists menuList) const
{
    return menuLists_[static_cast<size_t>(menuList)];
}

void MainFrameMenuBar::BuildMenuOpenRecent(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::OpenProject, wxART_FILE_OPEN, LANG_STRING(Menu_Open_Project), LANG_STRING(Menu_OpensProjectFromFile) },
    });
}

void MainFrameMenuBar::BuildMenuSelectorOrientation(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::SelectorOrientationGlobal, wxBITMAP_PNG( PngOrientationGlobal ), LANG_STRING( Menu_SelectorOrientation_Global ) },
        { MainMenuIDs::SelectorOrientationLocal,  wxBITMAP_PNG( PngOrientationLocal  ), LANG_STRING( Menu_SelectorOrientation_Local  ) },
        { MainMenuIDs::SelectorOrientationView,   wxBITMAP_PNG( PngOrientationView   ), LANG_STRING( Menu_SelectorOrientation_View   ) },
    });
}

void MainFrameMenuBar::BuildMenuSelectorPivot(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::SelectorPivotMedianPoint,       wxBITMAP_PNG( PngPivotMedianPoint       ), LANG_STRING( Menu_SelectorPivot_MedianPoint       ) },
        { MainMenuIDs::SelectorPivotIndividualOrigins, wxBITMAP_PNG( PngPivotIndividualOrigins ), LANG_STRING( Menu_SelectorPivot_IndividualOrigins ) },
        { MainMenuIDs::SelectorPivotActiveElement,     wxBITMAP_PNG( PngPivotActiveElement     ), LANG_STRING( Menu_SelectorPivot_ActiveElements    ) },
        { MainMenuIDs::SelectorPivotBoundingBox,       wxBITMAP_PNG( PngPivotBoundingBox       ), LANG_STRING( Menu_SelectorPivot_BoundingBox       ) },
        { MainMenuIDs::SelectorPivotMinimum,           wxBITMAP_PNG( PngPivotMinimum           ), LANG_STRING( Menu_SelectorPivot_Minimum           ) },
        { MainMenuIDs::SelectorPivotMaximum,           wxBITMAP_PNG( PngPivotMaximum           ), LANG_STRING( Menu_SelectorPivot_Maximum           ) },
        { MainMenuIDs::SelectorPivotGlobalCursor,      wxBITMAP_PNG( PngPivotGlobalCursor      ), LANG_STRING( Menu_SelectorPivot_GlobalCursor      ) },
    });
}

void MainFrameMenuBar::BuildMenuRaster(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::TransformRasterNone,     wxBITMAP_PNG( PngTransformNoRaster       ), LANG_STRING( Menu_TransformRaster_None     ) },
        { MainMenuIDs::TransformRasterRelative, wxBITMAP_PNG( PngTransformRelativeRaster ), LANG_STRING( Menu_TransformRaster_Relative ) },
        { MainMenuIDs::TransformRasterAbsolute, wxBITMAP_PNG( PngTransformAbsoluteRaster ), LANG_STRING( Menu_TransformRaster_Absolute ) },
        {},
        { MainMenuIDs::TransformRaster_0_1,     "0.1 / 1°",  "", wxITEM_RADIO },
        { MainMenuIDs::TransformRaster_0_25,    "0.25 / 5°", "", wxITEM_RADIO },
        { MainMenuIDs::TransformRaster_0_5,     "0.5 / 15°", "", wxITEM_RADIO },
        { MainMenuIDs::TransformRaster_1,       "1.0 / 30°", "", wxITEM_RADIO },
        { MainMenuIDs::TransformRaster_2_5,     "2.5 / 45°", "", wxITEM_RADIO },
        { MainMenuIDs::TransformRaster_5,       "5.0 / 90°", "", wxITEM_RADIO },
    });
}

void MainFrameMenuBar::BuildMenuWireframe(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::ViewRenderModeShaded,      wxBITMAP_PNG( PngRenderModeShaded      ), LANG_STRING( Menu_ViewRenderMode_Shaded      ) },
        { MainMenuIDs::ViewRenderModeSolid,       wxBITMAP_PNG( PngRenderModeSolid       ), LANG_STRING( Menu_ViewRenderMode_Solid       ) },
        { MainMenuIDs::ViewRenderModeWireframe,   wxBITMAP_PNG( PngRenderModeWireframe   ), LANG_STRING( Menu_ViewRenderMode_Wireframe   ) },
        { MainMenuIDs::ViewRenderModeBoundingBox, wxBITMAP_PNG( PngRenderModeBoundingBox ), LANG_STRING( Menu_ViewRenderMode_BoundingBox ) },
    });
}

void MainFrameMenuBar::BuildMenuEditorViews(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::ViewEditorViewsSub1, LANG_STRING(Menu_ViewEditorViews_SubView) + " ( &1 )", "", wxITEM_CHECK },
        { MainMenuIDs::ViewEditorViewsSub2, LANG_STRING(Menu_ViewEditorViews_SubView) + " ( &2 )", "", wxITEM_CHECK },
        { MainMenuIDs::ViewEditorViewsSub3, LANG_STRING(Menu_ViewEditorViews_SubView) + " ( &3 )", "", wxITEM_CHECK },
    });
}

void MainFrameMenuBar::BuildMenuComponent(UIMenuList& menuList)
{
    menuList.AddItems({
        { MainMenuIDs::RemoveComponent, LANG_STRING(Menu_RemoveComponent), LANG_STRING(Menu_RemovesSelectedComponent) },
    });
}


/*
 * ======= Private: =======
 */

void MainFrameMenuBar::BuildMenuListEditor(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::Preferences, wxBITMAP_PNG(PngSettings),  LANG_STRING( Menu_Preferences ), LANG_STRING( Menu_OpenEditorPreferencesWindow ) },
        {},
        { MainMenuIDs::Quit,        wxART_QUIT,                 LANG_STRING( Menu_Quit        ), LANG_STRING( Menu_QuitsApplication            ) },
    });

    Append(menuList, LANG_STRING(Menu_Editor));
}

void MainFrameMenuBar::BuildMenuListProject(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::NewProject,      wxART_NEW,                  LANG_STRING( Menu_New_Project     ), LANG_STRING( Menu_GeneratesNewProject                 ) },
        { MainMenuIDs::OpenProject,     wxART_FILE_OPEN,            LANG_STRING( Menu_Open_Project    ), LANG_STRING( Menu_OpensProjectFromFile                ) },
        { MainMenuIDs::SaveProject,     wxART_FILE_SAVE,            LANG_STRING( Menu_Save_Project    ), LANG_STRING( Menu_SavesCurrentProject                 ) },
        { MainMenuIDs::SaveProjectAs,   wxART_FILE_SAVE_AS,         LANG_STRING( Menu_SaveAs_Project  ), LANG_STRING( Menu_SavesCurrentProjectUnderNewFilename ) },
        {},
        { MainMenuIDs::ProjectSettings, wxBITMAP_PNG(PngSettings),  LANG_STRING( Menu_ProjectSettings ), LANG_STRING( Menu_OpenProjectSettingsWindow           ) },
    });

    Append(menuList, LANG_STRING(Menu_Project));
}

void MainFrameMenuBar::BuildMenuListEdit(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::Undo,        wxART_UNDO,                 LANG_STRING( Menu_Undo        ), LANG_STRING( Menu_UndoPrevTask                ) },
        { MainMenuIDs::Redo,        wxART_REDO,                 LANG_STRING( Menu_Redo        ), LANG_STRING( Menu_RedoPrevTask                ) },
        {},
        { MainMenuIDs::Cut,         wxART_CUT,                  LANG_STRING( Menu_Cut         ), LANG_STRING( Menu_CutsSelectedObjects         ) },
        { MainMenuIDs::Copy,        wxART_COPY,                 LANG_STRING( Menu_Copy        ), LANG_STRING( Menu_CopysSelectedObjects        ) },
        { MainMenuIDs::Paste,       wxART_PASTE,                LANG_STRING( Menu_Paste       ), LANG_STRING( Menu_PastesObjectsFromClipboard  ) },
        {},
        { MainMenuIDs::Delete,      wxART_DELETE,               LANG_STRING( Menu_Delete      ), LANG_STRING( Menu_DeletesSelectedObjects      ) },
    });

    Append(menuList, LANG_STRING(Menu_Edit));
}

void MainFrameMenuBar::BuildMenuListEntity(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::AddComponent, LANG_STRING( Menu_AddComponent ), LANG_STRING( Menu_AddsNewComponent ) },
    });

    Append(menuList, LANG_STRING(Menu_Entity));
}

void MainFrameMenuBar::BuildMenuListHelp(UIMenuList* menuList)
{
    menuList->AddItems({
        { MainMenuIDs::Manual,      wxART_HELP_BOOK,    LANG_STRING( Menu_Manual     ), LANG_STRING( Menu_ShowsManual              ) },
        { MainMenuIDs::OnlineHelp,  wxART_HELP,         LANG_STRING( Menu_OnlineHelp ), LANG_STRING( Menu_OpensOnlineHelpInBrowser ) },
        { MainMenuIDs::ShowTip,     wxART_TIP,          LANG_STRING( Menu_ShowTip    ), LANG_STRING( Menu_ShowsTipDialog           ) },
        {},
        { MainMenuIDs::About,       wxART_INFORMATION,  LANG_STRING( Menu_About      ), LANG_STRING( Menu_ShowsAboutDialog         ) },
    });

    Append(menuList, LANG_STRING(Menu_Help));
}


} // /namespace Editor

} // /namespace Fork



// ========================