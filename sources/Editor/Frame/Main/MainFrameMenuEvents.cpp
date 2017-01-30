/*
 * Editor main frame menu events file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameMenuEvents.h"
#include "MainEventTable.h"
#include "MainFrameComponentController.h"
#include "Core/STLHelper.h"
#include "Core/Version.h"
#include "Core/StaticConfig.h"
#include "../Config/ConfigFrame.h"
#include "../Main/MainFrame.h"
#include "../../Language/LanguageLoader.h"
#include "../../Core/Devices.h"
#include "../../Core/UIExtensions/UIToolBar.h"
#include "../../Core/UIExtensions/UIMenuList.h"
#include "../../Entity/Entity.h"
#include "../../Entity/EntitySelector.h"
#include "../../Project/ProjectFolder.h"

#include <wx/tipdlg.h>
#include <wx/aboutdlg.h>
#include <wx/filedlg.h>

#include <random>


namespace Fork
{

namespace Editor
{

namespace MainFrameMenuEvents
{


using namespace Utility;
using namespace Devices;

/*
 * Internal functions
 */

/* --- Files --- */

static wxString ProjectFileWildcard()
{
    return "ForkGame Project (*.forkgame)|*.forkgame";
}

static void SaveProjectToFile(ProjectFolder* project, std::string filename)
{
    auto ext = ToLower(ExtractFileExt(filename));
    if (ext != "forkgame")
        filename += ".forkgame";
    project->SaveProject(filename);
}

static void SaveProjectAs(wxWindow* parent)
{
    auto project = ProjectFolder::Active();
    if (project)
    {
        /* Show <save file> dialog */
        wxFileDialog dialog(
            parent, "Save Project", project->GetDirectory(),
            project->GetFilename(), ProjectFileWildcard(),
            (wxFD_SAVE | wxFD_OVERWRITE_PROMPT)
        );

        if (dialog.ShowModal() == wxID_CANCEL)
            return;

        /* Save project */
        SaveProjectToFile(project, dialog.GetPath().ToStdString());
    }
}

static void SaveProject(wxWindow* parent)
{
    auto project = ProjectFolder::Active();
    if (project)
    {
        if (project->GetFilename().empty())
            SaveProjectAs(parent);
        else
            SaveProjectToFile(project, project->GetFilename());
    }
}

/* --- Extras --- */

static void OpenEditorPreferencesWindow()
{
    MainFrame::Instance()->GetConfigFrame()->Show();
}

/* --- View --- */

static void ShowEditorSubView(int subView, bool show)
{
    MainFrame::Instance()->ShowEditorSubView(subView, show);
}

/* --- Help --- */

static void ShowAuxiliaryManual()
{
    //todo...
}

static void ShowOnlineHelp()
{
    wxLaunchDefaultBrowser("http://www.fork-engine.com/forum/");
}

static void ShowTipDialog(wxWindow* parent)
{
    /*showTipsOnStartup = */wxShowTip(
        parent,
        wxCreateFileTipProvider(
            "misc/EditorUserTips.txt",
            RandomUniformDistribution<size_t>(0, 2)
        ),
        false/*showTipsOnStartup*/
    );
}

static void ShowAboutDialog(wxWindow* parent)
{
    wxAboutDialogInfo info;
    {
        //info.SetIcon(wxICON(page_anim));
        info.SetName(GetEngineIDName());
        info.SetVersion(EngineVersion());
        info.SetDescription(LANG_STRING(AboutDescription));
        info.SetCopyright("Copyright (c) 2014  Lukas Hermanns");
        info.SetWebSite("http://www.fork-engine.com/");
        info.AddDeveloper("Lukas Hermanns");
        #ifdef FORK_COMMERCIAL_USE
        info.SetLicence("Pro Version (Commercial Use Allowed)");
        #else
        info.SetLicence("Basic Version (Non Commercial Use Only)");
        #endif
    }
    wxAboutBox(info, parent);
}

/* --- Popup: Component ---*/

static void RemoveComponent()
{
    /* Remove the previously selected component type from all selected entities */
    auto componentType = MainFrame::Instance()->GetComponentCtrl()->GetSelectedComponentType();
    for (auto entity : entitySelector->GetSelectedEntities())
        entity->DeleteComponent(componentType);

    /* Update component view */
    entitySelector->UpdateComponentView();
}


/*
 * Global functions
 */

void ProcessMenuEvent(wxWindow* parent, wxCommandEvent& event)
{
    auto eventID = event.GetId();

    switch (eventID)
    {
        /* --- Editor --- */
        case MainMenuIDs::Preferences:
            OpenEditorPreferencesWindow();
            break;
        case MainMenuIDs::Quit:
            break;

        /* --- Project --- */
        case MainMenuIDs::NewProject:
            break;
        case MainMenuIDs::OpenProject:
            break;
        case MainMenuIDs::SaveProject:
            SaveProject(parent);
            break;
        case MainMenuIDs::SaveProjectAs:
            SaveProjectAs(parent);
            break;
        case MainMenuIDs::ProjectSettings:
            break;

        /* --- Edit --- */
        case MainMenuIDs::Undo:
            break;
        case MainMenuIDs::Redo:
            break;

        case MainMenuIDs::Cut:
            entitySelector->CutSelectedEntities();
            break;
        case MainMenuIDs::Copy:
            entitySelector->CopySelectedEntities();
            break;
        case MainMenuIDs::Paste:
            entitySelector->PasteSelectedEntities();
            break;
        case MainMenuIDs::Delete:
            entitySelector->DeleteSelectedEntities();
            break;

        /* --- Transform --- */
        case MainMenuIDs::TransformTranslate:
        case MainMenuIDs::TransformRotate:
        case MainMenuIDs::TransformScale:
            entitySelector->SetupTransformMode(
                static_cast<SelectorModel::TransformModes>(
                    eventID - MainMenuIDs::TransformTranslate +
                    static_cast<int>(SelectorModel::TransformModes::Translate)
                )
            );
            break;

        case MainMenuIDs::SelectorOrientationGlobal:
        case MainMenuIDs::SelectorOrientationLocal:
        case MainMenuIDs::SelectorOrientationView:
            entitySelector->SetupOrientationAlignment(
                static_cast<SelectorModel::OrientationAlignments>(
                    eventID - MainMenuIDs::SelectorOrientationGlobal +
                    static_cast<int>(SelectorModel::OrientationAlignments::Global)
                )
            );
            break;
        
        case MainMenuIDs::SelectorPivotMedianPoint:
        case MainMenuIDs::SelectorPivotIndividualOrigins:
        case MainMenuIDs::SelectorPivotActiveElement:
        case MainMenuIDs::SelectorPivotBoundingBox:
        case MainMenuIDs::SelectorPivotMinimum:
        case MainMenuIDs::SelectorPivotMaximum:
        case MainMenuIDs::SelectorPivotGlobalCursor:
            entitySelector->SetupPivotAlignment(
                static_cast<SelectorModel::PivotAlignments>(
                    eventID - MainMenuIDs::SelectorPivotMedianPoint +
                    static_cast<int>(SelectorModel::PivotAlignments::MedianPoint)
                )
            );
            break;

        case MainMenuIDs::TransformRasterNone:
        case MainMenuIDs::TransformRasterRelative:
        case MainMenuIDs::TransformRasterAbsolute:
            entitySelector->SetupRaster(
                static_cast<SelectorModel::Raster::Modes>(
                    eventID - MainMenuIDs::TransformRasterNone +
                    static_cast<int>(SelectorModel::Raster::Modes::None)
                )
            );
            break;

        case MainMenuIDs::TransformRaster_0_1:
        case MainMenuIDs::TransformRaster_0_25:
        case MainMenuIDs::TransformRaster_0_5:
        case MainMenuIDs::TransformRaster_1:
        case MainMenuIDs::TransformRaster_2_5:
        case MainMenuIDs::TransformRaster_5:
            entitySelector->SetupRasterSize(
                static_cast<size_t>(eventID - MainMenuIDs::TransformRaster_0_1)
            );
            break;

        case MainMenuIDs::EnableAxisX:
        case MainMenuIDs::EnableAxisY:
        case MainMenuIDs::EnableAxisZ:
            entitySelector->GetModel()->EnableAxis(
                static_cast<size_t>(eventID - MainMenuIDs::EnableAxisX), event.IsChecked()
            );
            break;

        /* --- View --- */
        case MainMenuIDs::ViewRenderMode:
            break;

        case MainMenuIDs::ViewEditorViews:
            break;
        case MainMenuIDs::ViewEditorViewsSub1:
        case MainMenuIDs::ViewEditorViewsSub2:
        case MainMenuIDs::ViewEditorViewsSub3:
            ShowEditorSubView(eventID - MainMenuIDs::ViewEditorViewsSub1 + 1, event.IsChecked());
            break;

        /* --- Tools --- */
        //...

        /* --- Help --- */
        case MainMenuIDs::Manual:
            ShowAuxiliaryManual();
            break;
        case MainMenuIDs::OnlineHelp:
            ShowOnlineHelp();
            break;
        case MainMenuIDs::ShowTip:
            ShowTipDialog(parent);
            break;
        case MainMenuIDs::About:
            ShowAboutDialog(parent);
            break;

        /* --- Popup: Component --- */
        case MainMenuIDs::RemoveComponent:
            RemoveComponent();
            break;
    }
}


} // /namespace MainFrameMenuEvents

} // /namespace Editor

} // /namespace Fork



// ========================