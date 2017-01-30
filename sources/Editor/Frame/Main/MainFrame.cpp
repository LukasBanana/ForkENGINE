/*
 * Editor main frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrame.h"
#include "MainFrameMenuBar.h"
#include "MainFrameStatusBar.h"
#include "MainFrameMenuEvents.h"
#include "MainFrameToolBarComposition.h"
#include "MainFrameRibbonBar.h"
#include "MainFrameComponentController.h"
#include "MainFrameSceneGraphController.h"
#include "MainFrameAssetController.h"
#include "MainFrameAssetDescriptor.h"
#include "MainEventTable.h"

#include "../Config/ConfigFrame.h"
#include "../Console/ConsoleFrame.h"
#include "../HierarchyViewer/HierarchyViewerFrame.h"
#include "../../Tools/AssetBrowser/AssetBrowser.h"
#include "../../Language/LanguageLoader.h"
#include "../../Core/DefaultMessageBoxes.h"
#include "../../Core/StaticLayoutConfig.h"
#include "../../Core/WxHelper.h"
#include "../../Core/UIExtensions/UIViewManager.h"
#include "../../Core/UIExtensions/UIToolBar.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Core/UIExtensions/UITreeView.h"
#include "../../Core/UIExtensions/UIScreenFrame.h"
#include "../../Core/UIExtensions/UIMenuList.h"
#include "../../Core/Config.h"
#include "../../Core/Devices.h"
#include "../../EditorView.h"

#include "Platform/Core/OS.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "IO/Core/Console.h"
#include "Core/StaticConfig.h"

#if 1//!!!TESTING!!!
#include "../../Project/ProjectFactory.h"
#include "../../Project/ProjectFolder.h"
#endif


namespace Fork
{

namespace Editor
{


/* --- Internal members --- */

static const wxString paneLabelToolbar      = "PaneToolBar";
static const wxString paneLabelEditorView   = "PaneScreenFrame";


/*
 * MainFrame class
 */

using namespace StaticLayout::MainFrameLayout;

MainFrame* MainFrame::instance_ = nullptr;

MainFrame::MainFrame() :
    wxFrame
    {
        nullptr, wxID_ANY, MainFrame::LabelWithArchEnvironment(),
        wxDefaultPosition, defaultSize
    }
{
    MainFrame::instance_ = this;

    BindCallbacks();
    BuildFrameLayout();
    CreateSubFrames();

    SetSizeHints(minSize);
    SetIcon(wxICON(IcoForkEdLogo));

    ChangeLabelWithProjectName("");
    Center();

    statusBar_->SetupRendererInfo(
        Video::RenderContext::Active()->QueryAdapter().rendererVersion
    );

    SwitchRibbonBar(false);

    #if 1//!!!
    auto project = ProjectFactory::Instance()->CreateProjectFolder();
    project->ChangeName("DevProject-01");
    project->CreateGameScene();
    #endif
}
MainFrame::~MainFrame()
{
    Devices::CleanUpScene();

    Config::Instance()->SaveLayout();
    Config::Instance()->SaveSettings();

    auiMngr_->UnInit();
}

void MainFrame::OnMenuStates(wxCommandEvent& event)
{
    MainFrameMenuEvents::ProcessMenuEvent(this, event);
}

void MainFrame::OnToolbarDropDown(wxAuiToolBarEvent& event)
{
    auto toolBar = dynamic_cast<UIToolBar*>(event.GetEventObject());
    auto eventID = event.GetId();

    if (event.IsDropDownClicked())
    {
        if (toolBar)
            toolBar->ShowDropDownMenu(this, eventID);
    }
    else
    {
        if (toolBar && !toolBar->FindDropDownMenu(eventID).isButtonExt)
            toolBar->ShowDropDownMenu(this, eventID);
        else
            MainFrameMenuEvents::ProcessMenuEvent(this, event);
    }
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    //todo: check if scene has changed ...
    wxFrame::OnCloseWindow(event);
}

void MainFrame::OnResize(wxSizeEvent& event)
{
    //todo...
}

void MainFrame::OnMove(wxMoveEvent& event)
{
    DrawChildren();
}

void MainFrame::OnClosePane(wxAuiManagerEvent& event)
{
    const auto& paneID = event.GetPane()->name;

    if (paneID.Find(paneLabelEditorView) != wxNOT_FOUND)
    {
        auto toolBarView = GetToolBarComposition()->GetToolBars()[MainFrameToolBarComposition::ToolBars::View];
        auto menuEditorViews = toolBarView->FindDropDownMenu(MainMenuIDs::ViewEditorViews).menuList;
        if (menuEditorViews)
        {
            auto subView = StrToNum<int>(paneID.Right(1).ToStdString()) - 2;
            menuEditorViews->Check(MainMenuIDs::ViewEditorViewsSub1 + subView, false);
        }
    }
}

void MainFrame::OnMoveSpeed(wxCommandEvent& event)
{
    //...
}

void MainFrame::LoadLayout(IO::File& file)
{
    /* Load frame layout */
    auto framePosition = file.Read<Math::Point2i>();
    SetPosition({ framePosition.x, framePosition.y });

    auto frameSize = file.Read<Math::Size2i>();
    SetSize({ frameSize.width, frameSize.height });

    /* Load AUI layout */
    auto layoutStr = file.ReadStringSized();
    auiMngr_->LoadPerspective(layoutStr);

    /* Update menu states respective to AUI layout */
    UpdateMenuStateForSubViews();
}

void MainFrame::SaveLayout(IO::File& file)
{
    /* Save frame layout */
    auto framePosition = GetPosition();
    file.Write(Math::Point2i(framePosition.x, framePosition.y));

    auto frameSize = GetSize();
    file.Write(Math::Size2i(frameSize.GetWidth(), frameSize.GetHeight()));

    /* Save AUI layout */
    auto layoutStr = auiMngr_->SavePerspective().ToStdString();
    file.WriteStringSized(layoutStr);
}

void MainFrame::ChangeLabelWithProjectName(const std::string& projectName)
{
    if (projectName.empty())
        SetTitle("<" + Language::String(LangStrings::Unnamed) + "> - " + MainFrame::LabelWithArchEnvironment());
    else
        SetTitle(projectName + " - " + MainFrame::LabelWithArchEnvironment());
}

void MainFrame::UpdateAUI()
{
    if (auiMngr_)
        auiMngr_->Update();
}

void MainFrame::SwitchRibbonBar(bool useRibbonBar)
{
    ShowRibbonBar(useRibbonBar);

    ShowToolBar(!useRibbonBar);
    SetMenuBar(useRibbonBar ? nullptr : menuBar_);

    UpdateAUI();
}

void MainFrame::ShowEditorSubView(int subView, bool show)
{
    auto editorView = EditorSubView(subView);
    if (editorView)
    {
        auto& pane = auiMngr_->GetPane(editorView->GetScreenFrame());
        if (pane.IsOk())
        {
            pane.Show(show);
            UpdateAUI();
        }
    }
}

bool MainFrame::IsEditorSubViewShown(int subView) const
{
    auto editorView = EditorSubView(subView);
    if (editorView)
    {
        auto& pane = auiMngr_->GetPane(editorView->GetScreenFrame());
        if (pane.IsOk())
            return pane.IsShown();
    }
    return false;
}

void MainFrame::DrawChildren()
{
    /* Draw all editor views */
    for (auto& view : GetEditorViews())
        view->Draw();

    /* Draw console frame (if visible) */
    if (GetConsoleFrame() && GetConsoleFrame()->IsShown())
        GetConsoleFrame()->Draw();

    /* Draw asset browser */
    if (GetAssetBrowser() && GetAssetBrowser()->GetScreenFrame()->IsShown())
        GetAssetBrowser()->Draw();
}

std::string MainFrame::Label()
{
    #ifdef FORK_COMMERCIAL_USE
    return "ForkEd Pro";
    #else
    return "ForkEd Basic";
    #endif
}

std::string MainFrame::LabelWithArchEnvironment()
{
    std::string label = Label();

    #ifdef FORK_ARCH_ENV_64BIT
    label += " (64 Bit)";
    #else
    label += " (32 Bit)";
    #endif

    #ifdef FORK_DEBUG
    label += " (DEBUG)";
    #endif

    return label;
}

MainFrame* MainFrame::Instance()
{
    return instance_;
}


/*
 * ======= Private: =======
 */

void MainFrame::BindCallbacks()
{
    Bind( wxEVT_MOVE,         &MainFrame::OnMove,  this );
    Bind( wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this );
}

void MainFrame::BuildFrameLayout()
{
    /* Create main panel and box sizer */
    mainSizer_ = MakeWxObject<wxBoxSizer>(wxVERTICAL);

    /* Build children frames */
    BuildRibbonBar();
    BuildAUI();
    BuildMenuBar();
    BuildToolBar();
    BuildStatusBar();

    /* Build frames for client area */
    BuildComponentCtrl();
    BuildSceneGraphCtrl();
    BuildAssetCtrl();
    BulidAssetDesc();
    BuildEditorView();
    BuildAssetBrowser();

    /* Setup main sizer */
    SetSizer(mainSizer_);
}

void MainFrame::BuildAUI()
{
    /* Create main panel */
    mainPanel_ = MakeWxObject<wxPanel>(this, wxID_ANY, wxDefaultPosition, GetSize(), 0);
    mainSizer_->Add(mainPanel_, 1, wxEXPAND);

    /* Create AUI manager */
    auiMngr_ = MakeWxObject<UIViewManager>(mainPanel_);
    auiMngr_->Bind(wxEVT_AUI_PANE_CLOSE, &MainFrame::OnClosePane, this);
}

void MainFrame::BuildMenuBar()
{
    menuBar_ = MakeWxObject<MainFrameMenuBar>();
    SetMenuBar(menuBar_);
}

void MainFrame::BuildToolBar()
{
    toolBar_ = std::make_unique<MainFrameToolBarComposition>(mainPanel_);

    /* Integrate "main" toolbar */
    auiMngr_->AddPane(
        toolBar_->GetToolBars()[MainFrameToolBarComposition::ToolBars::Main],
        wxAuiPaneInfo()
            .Name(paneLabelToolbar + "0").Caption("Main Toolbar")
            .ToolbarPane().Top()
    );

    /* Integrate "transform" toolbar */
    auiMngr_->AddPane(
        toolBar_->GetToolBars()[MainFrameToolBarComposition::ToolBars::Transform],
        wxAuiPaneInfo()
            .Name(paneLabelToolbar + "1").Caption("Transform Toolbar")
            .ToolbarPane().Top()
    );

    /* Integrate "view" toolbar */
    auiMngr_->AddPane(
        toolBar_->GetToolBars()[MainFrameToolBarComposition::ToolBars::View],
        wxAuiPaneInfo()
            .Name(paneLabelToolbar + "2").Caption("View Toolbar")
            .ToolbarPane().Top()
    );
}

void MainFrame::BuildRibbonBar()
{
    ribbonBar_ = MakeWxObject<MainFrameRibbonBar>(this);
    mainSizer_->Add(ribbonBar_, 0, wxEXPAND);
}

void MainFrame::BuildStatusBar()
{
    statusBar_ = MakeWxObject<MainFrameStatusBar>(this);
    SetStatusBar(statusBar_);
}

void MainFrame::BuildComponentCtrl()
{
    componentCtrl_ = std::make_unique<MainFrameComponentController>();
    componentCtrl_->BuildComponentView(mainPanel_);

    auiMngr_->AddPane(
        componentCtrl_->GetComponentView(),
        wxAuiPaneInfo()
            .Name("PaneComponentCtrl").Caption(LANG_STRING(Components))
            .Right()
            .PinButton()
    );
}

void MainFrame::BuildSceneGraphCtrl()
{
    sceneGraphCtrl_ = std::make_unique<MainFrameSceneGraphController>();
    sceneGraphCtrl_->BuildTreeView(mainPanel_);

    auiMngr_->AddPane(
        sceneGraphCtrl_->GetTreeView(),
        wxAuiPaneInfo()
            .Name("PaneSceneGraphCtrl").Caption(LANG_STRING(SceneGraph))
            .Left()
            .PinButton()
    );
}

void MainFrame::BuildAssetCtrl()
{
    assetCtrl_ = std::make_unique<MainFrameAssetController>();
    assetCtrl_->BuildTreeView(mainPanel_);

    auiMngr_->AddPane(
        assetCtrl_->GetTreeView(),
        wxAuiPaneInfo()
            .Name("PaneAssetCtrl").Caption(LANG_STRING(Assets))
            .Left()
            .PinButton()
    );
}

void MainFrame::BulidAssetDesc()
{
    assetDesc_ = std::make_unique<MainFrameAssetDescriptor>();
    assetDesc_->BuildAttribView(mainPanel_);

    auiMngr_->AddPane(
        assetDesc_->GetAttribView(),
        wxAuiPaneInfo()
            .Name("PaneAssetDesc").Caption(LANG_STRING(AssetDescription))
            .Right()
            .PinButton()
    );
}

void MainFrame::BuildEditorView()
{
    CreateEditorViewAndAUIPane("Main View", true);
    for (int i = 0; i < 3; ++i)
        CreateEditorViewAndAUIPane("Sub View ( " + ToStr(i + 1) + " )");
}

void MainFrame::BuildAssetBrowser()
{
    try
    {
        IO::Log::Message("Initialize asset browser ...");
        IO::Log::ScopedIndent indent;

        /* Create editor view and store it in the list */
        assetBrowser_ = std::make_unique<AssetBrowser>(mainPanel_);

        auiMngr_->AddPane(
            assetBrowser_->GetScreenFrame(),
            wxAuiPaneInfo()
                .Name("PaneAssetBrowserScreenFrame").Caption("Asset Browser")
                .Center()
                .Bottom()
                .PinButton()
                .MaximizeButton()
        );
    }
    catch (const RenderContextException&)
    {
        FatalError("Creating Asset Browser Failed", this);
    }
}

void MainFrame::CreateSubFrames()
{
    configFrame_ = std::make_unique<ConfigFrame>(this);
    consoleFrame_ = std::make_unique<ConsoleFrame>(this);
    geometryHierarchyViewerFrame_ = std::make_unique<HierarchyViewerFrame>(LANG_STRING(GeometryHierarchyViewer), this);
}

EditorView* MainFrame::CreateEditorView()
{
    try
    {
        /* Create editor view and store it in the list */
        auto editorView = std::make_unique<EditorView>(mainPanel_);
        auto editorViewRef = editorView.get();
        editorViews_.push_back(std::move(editorView));
        return editorViewRef;
    }
    catch (const RenderContextException&)
    {
        /* Ignore this exception here */
    }
    return nullptr;
}

EditorView* MainFrame::CreateEditorViewAndAUIPane(const wxString& title, bool isMainView)
{
    static int screenPaneCounter;

    auto editorView = CreateEditorView();

    if (editorView)
    {
        wxAuiPaneInfo paneInfo;

        paneInfo.Name(paneLabelEditorView + ToStr(++screenPaneCounter));
        paneInfo.Caption(title).MaximizeButton();

        if (isMainView)
        {
            paneInfo.Center();
            paneInfo.CloseButton(false);
        }
        else
        {
            paneInfo.PinButton();
            paneInfo.Right();
        }

        auiMngr_->AddPane(editorView->GetScreenFrame(), paneInfo);
    }

    return editorView;
}

EditorView* MainFrame::EditorSubView(int subView) const
{
    auto index = static_cast<size_t>(subView);
    return (index >= 1 && index < editorViews_.size()) ? editorViews_[index].get() : nullptr;
}

/*
Checks or unchecks the menu items for the editor sub views,
whether the respective AUI panes are shown or not.
*/
void MainFrame::UpdateMenuStateForSubViews()
{
    auto toolBarView = GetToolBarComposition()->GetToolBars()[MainFrameToolBarComposition::ToolBars::View];
    auto menuEditorViews = toolBarView->FindDropDownMenu(MainMenuIDs::ViewEditorViews).menuList;
    if (menuEditorViews)
    {
        menuEditorViews->Check(MainMenuIDs::ViewEditorViewsSub1, IsEditorSubViewShown(1));
        menuEditorViews->Check(MainMenuIDs::ViewEditorViewsSub2, IsEditorSubViewShown(2));
        menuEditorViews->Check(MainMenuIDs::ViewEditorViewsSub3, IsEditorSubViewShown(3));
    }
}

void MainFrame::ShowToolBar(bool show)
{
    for (int i = 0; i < 3; ++i)
        auiMngr_->GetPane(paneLabelToolbar + ToStr(i)).Show(show);
}

void MainFrame::ShowRibbonBar(bool show)
{
    ribbonBar_->Show(show);
}


} // /namespace Editor

} // /namespace Fork



// ========================