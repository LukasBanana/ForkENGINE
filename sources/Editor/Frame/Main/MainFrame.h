/*
 * Editor main frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_H__
#define __FORK_EDITOR_MAIN_FRAME_H__


#include "../../EditorView.h"
#include "IO/FileSystem/File.h"

#include <wx/frame.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/auibar.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include <memory>
#include <vector>
#include <fstream>


namespace Fork
{

namespace Editor
{


class ConfigFrame;
class ConsoleFrame;
class HierarchyViewerFrame;
class MainFrameMenuBar;
class MainFrameStatusBar;
class MainFrameToolBarComposition;
class MainFrameComponentController;
class MainFrameSceneGraphController;
class MainFrameAssetController;
class MainFrameAssetDescriptor;
class MainFrameRibbonBar;
class UIViewManager;
class UIScreenFrame;
class AssetBrowser;

class MainFrame : public wxFrame
{
    
    public:
        
        MainFrame();
        ~MainFrame();

        /* === Callback functions === */

        void OnMenuStates       (wxCommandEvent&    event);
        void OnToolbarDropDown  (wxAuiToolBarEvent& event);

        void OnClose            (wxCloseEvent&      event);
        void OnResize           (wxSizeEvent&       event);
        void OnMove             (wxMoveEvent&       event);
        void OnClosePane        (wxAuiManagerEvent& event);

        void OnMoveSpeed        (wxCommandEvent&    event);

        /* === Functions === */

        //! Loads the frame layout from the specified file.
        void LoadLayout(IO::File& file);
        //! Saves the frame layout to the specified file.
        void SaveLayout(IO::File& file);

        /**
        Changes the label of the main frame with the specified project name.
        \param[in] projectName Specifies the new active project name which is shown in the frame title bar.
        If this is empty, a default label is set (e.g. "<Unnamed>").
        \remarks The final frame title also consists of the main label, i.e. the name of the editor, and the CPU architecture environemnt.
        \see Label
        \see LabelWithArchEnvironment
        */
        void ChangeLabelWithProjectName(const std::string& projectName);

        //! Updates the advanced user interface (AUI).
        void UpdateAUI();

        //! Switches between the ribbon bar or the default toolbar.
        void SwitchRibbonBar(bool useRibbonBar);
        //! Shows or hides the specified sub view. This index must be 1, 2 or 3.
        void ShowEditorSubView(int subView, bool show = true);
        //! Returns true if the specified sub view is shown.
        bool IsEditorSubViewShown(int subView) const;

        //! Draws all children screens.
        void DrawChildren();

        //! Returns the default label for this frame class.
        static std::string Label();
        //! Returns the default label with architecture environment (32- or 64 bit) for this frame class.
        static std::string LabelWithArchEnvironment();

        //! Returns the singleton instance of this main frame.
        static MainFrame* Instance();

        /* === Inline functions === */

        //! Returns the main menu bar.
        inline MainFrameMenuBar* GetMainMenuBar() const
        {
            return menuBar_;
        }
        //! Returns the main status bar.
        inline MainFrameStatusBar* GetMainStatusBar() const
        {
            return statusBar_;
        }
        //! Returns the main ribbon bar.
        inline MainFrameRibbonBar* GetMainRibbonBar() const
        {
            return ribbonBar_;
        }

        //! Returns the editor configuration frame.
        inline ConfigFrame* GetConfigFrame() const
        {
            return configFrame_.get();
        }
        //! Returns the console (also "terminal") frame.
        inline ConsoleFrame* GetConsoleFrame() const
        {
            return consoleFrame_.get();
        }
        //! Returns the hiearchy viewer frame.
        inline HierarchyViewerFrame* GetHierarchyViewerFrame() const
        {
            return geometryHierarchyViewerFrame_.get();
        }
        //! Returns the asset browser.
        inline AssetBrowser* GetAssetBrowser() const
        {
            return assetBrowser_.get();
        }

        //! Returns the tool-bar composition.
        inline MainFrameToolBarComposition* GetToolBarComposition() const
        {
            return toolBar_.get();
        }
        //! Returns the component view controller.
        inline MainFrameComponentController* GetComponentCtrl() const
        {
            return componentCtrl_.get();
        }
        //! Returns the scene graph view controller.
        inline MainFrameSceneGraphController* GetSceneGraphCtrl() const
        {
            return sceneGraphCtrl_.get();
        }
        //! Returns the asset view controller.
        inline MainFrameAssetController* GetAssetCtrl() const
        {
            return assetCtrl_.get();
        }
        //! Returns the asset descriptor.
        inline MainFrameAssetDescriptor* GetAssetDesc() const
        {
            return assetDesc_.get();
        }

        //! Returns the list of all editor views.
        inline const std::vector<std::unique_ptr<EditorView>>& GetEditorViews() const
        {
            return editorViews_;
        }

        DECLARE_EVENT_TABLE()

    private:
        
        /* === Functions === */

        void BindCallbacks();

        void BuildFrameLayout();
        void BuildAUI();
        void BuildMenuBar();
        void BuildToolBar();
        void BuildRibbonBar();
        void BuildStatusBar();
        void BuildComponentCtrl();
        void BuildSceneGraphCtrl();
        void BuildAssetCtrl();
        void BulidAssetDesc();
        void BuildEditorView();
        void BuildAssetBrowser();

        void CreateSubFrames();

        EditorView* CreateEditorView();
        EditorView* CreateEditorViewAndAUIPane(const wxString& title, bool isMainView = false);

        EditorView* EditorSubView(int subView) const;
        void UpdateMenuStateForSubViews();

        void ShowToolBar(bool show);
        void ShowRibbonBar(bool show);

        /* === Members === */

        wxPanel*                                        mainPanel_      = nullptr;
        wxSizer*                                        mainSizer_      = nullptr;

        MainFrameMenuBar*                               menuBar_        = nullptr;
        MainFrameStatusBar*                             statusBar_      = nullptr;
        MainFrameRibbonBar*                             ribbonBar_      = nullptr;
        
        std::unique_ptr<MainFrameToolBarComposition>    toolBar_;
        std::unique_ptr<MainFrameComponentController>   componentCtrl_;
        std::unique_ptr<MainFrameSceneGraphController>  sceneGraphCtrl_;
        std::unique_ptr<MainFrameAssetController>       assetCtrl_;
        std::unique_ptr<MainFrameAssetDescriptor>       assetDesc_;

        UIViewManager*                                  auiMngr_        = nullptr;

        std::unique_ptr<ConfigFrame>                    configFrame_;
        std::unique_ptr<ConsoleFrame>                   consoleFrame_;
        std::unique_ptr<AssetBrowser>                   assetBrowser_;
        std::unique_ptr<HierarchyViewerFrame>           geometryHierarchyViewerFrame_;

        std::vector<std::unique_ptr<EditorView>>        editorViews_;

        static MainFrame*                               instance_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================