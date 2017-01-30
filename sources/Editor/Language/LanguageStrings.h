/*
 * Editor language strings header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_LANGUAGE_STRINGS_H__
#define __FORK_EDITOR_LANGUAGE_STRINGS_H__


namespace Fork
{

namespace Editor
{


//! Language strings
struct LangStrings
{
    typedef unsigned int DataType;
    enum : DataType
    {
        /* --- Common --- */
        Unnamed = 0,
        Default,
        Common,
        Graphics,
        FatalError,
        Terminal,
        Texture,
        Model,
        Format,
        Width,
        Height,
        Depth,
        Errors,
        Axis,

        /* --- Main Frame --- */
        Components,
        SceneGraph,
        Assets,
        AssetDescription,

        /* --- Main Frame Ribbon Bar --- */
        Ribbon_Edit,
        Ribbon_Edit_Toolbar,
        Ribbon_Edit_Transformation,
        Ribbon_Edit_View,
        
        Ribbon_Terrain,

        Ribbon_Game,

        /* --- Main Menu: Editor --- */
        Menu_Editor,

        Menu_Quit,
        Menu_QuitsApplication,

        /* --- Main Menu: Project --- */
        Menu_Project,

        Menu_New_Project,
        Menu_GeneratesNewProject,

        Menu_Open_Project,
        Menu_OpensProjectFromFile,

        Menu_Save_Project,
        Menu_SavesCurrentProject,

        Menu_SaveAs_Project,
        Menu_SavesCurrentProjectUnderNewFilename,

        Menu_ProjectSettings,
        Menu_OpenProjectSettingsWindow,

        /* --- Main Menu: Edit --- */
        Menu_Edit,

        Menu_Undo,
        Menu_UndoPrevTask,
        Menu_Redo,
        Menu_RedoPrevTask,

        Menu_Cut,
        Menu_CutsSelectedObjects,
        Menu_Copy,
        Menu_CopysSelectedObjects,
        Menu_Paste,
        Menu_PastesObjectsFromClipboard,

        Menu_Delete,
        Menu_DeletesSelectedObjects,

        Menu_Preferences,
        Menu_OpenEditorPreferencesWindow,

        /* --- Main Menu: Entity --- */
        Menu_Entity,
        
        Menu_AddComponent,
        Menu_AddsNewComponent,
        Menu_RemoveComponent,
        Menu_RemovesSelectedComponent,

        /* --- Main Menu: Transform */
        Menu_TransformTranslate,
        Menu_TranslatesAllSelectedEntities,
        Menu_TransformRotate,
        Menu_RotatesAllSelectedEntities,
        Menu_TransformScale,
        Menu_ScalesAllSelectedEntities,

        Menu_SelectorOrientation,
        Menu_ChangesSelectorOrientationAlignment,
        Menu_SelectorOrientation_Global,
        Menu_SelectorOrientation_Local,
        Menu_SelectorOrientation_View,

        Menu_SelectorPivot,
        Menu_SelectorPivot_MedianPoint,
        Menu_SelectorPivot_IndividualOrigins,
        Menu_SelectorPivot_ActiveElements,
        Menu_SelectorPivot_BoundingBox,
        Menu_SelectorPivot_Minimum,
        Menu_SelectorPivot_Maximum,
        Menu_SelectorPivot_GlobalCursor,
        Menu_ChangesSelectorPivotAlignment,

        Menu_TransformRaster,
        Menu_TransformRaster_None,
        Menu_TransformRaster_Relative,
        Menu_TransformRaster_Absolute,

        Menu_EnableAxis_X,
        Menu_EnableAxis_Y,
        Menu_EnableAxis_Z,
        Menu_EnablesTheAxesXYZ,

        Menu_TransformPinCamera,
        Menu_PinsTheCameraWhenTranslatingEntites,
        Menu_ConfiguresTheTransformationRaster,
        Menu_SelectorShow,
        Menu_ShowsOrHidesTheSelector,
        Menu_SelectionEnable,
        Menu_EnablesOrDisablesEntitySelection,

        /* --- Main Menu: View --- */
        Menu_ViewRenderMode,
        Menu_ChangesTheRenderMode,
        Menu_ViewRenderMode_Shaded,
        Menu_ViewRenderMode_Solid,
        Menu_ViewRenderMode_Wireframe,
        Menu_ViewRenderMode_BoundingBox,
        Menu_ViewEditorViews,
        Menu_ShowsHidesEditorViews,
        Menu_ViewEditorViews_SubView,
        Menu_CameraMoveSpeed,

        /* --- Main Menu: Help --- */
        Menu_Help,

        Menu_Manual,
        Menu_ShowsManual,

        Menu_OnlineHelp,
        Menu_OpensOnlineHelpInBrowser,

        Menu_ShowTip,
        Menu_ShowsTipDialog,

        Menu_About,
        Menu_ShowsAboutDialog,

        /* --- Config Frame (Common) --- */
        SelectorSettings,
        MoveFreeInRaster,
        DrawSelectedAxesOnly,
        DrawPreviousTransform,
        DrawAxesLabels,
        DrawThickAxes,
        PickingOnlyTranslation,
        AlignCursor,
        AxesColors,
        NormalColor,
        SelectionColor,
        
        EditorViewSettings,
        ProjectorSize,
        ProjectorTransition,

        AssetBrowserSettings,
        AssetPreviewSize,
        ShowPreviewMask,

        /* --- Config Frame (UI) --- */
        EditorPreferences,
        UserInterface,
        GeneralSettings,
        ColorScheme,
        LanguageModule,

        /* --- Config Frame (Graphics) --- */
        RenderSystemSettings,
        Renderer,
        AntiAliasing,
        AntiAliasingMultiSamples,

        /* --- Hierarchy Viewer Frames --- */
        GeometryHierarchyViewer,

        /* --- About Dialog --- */
        AboutDescription,

        /* --- Component: Meta Data --- */
        MetaData,
        Name,
        Enabled,

        /* --- Component: Transform --- */
        Transform,
        Position,
        Rotation,
        Scaling,

        /* --- Component: Geometry --- */
        Geometry,
        Asset,

        /* --- Component: Auto UV Map--- */
        AutoUVMap,
        Update,
        Global,
        OffsetU,
        OffsetV,
        ScaleU,
        ScaleV,

        /* --- Asset: Texture --- */
        TextureAsset,

        /* --- Asset: Model --- */
        ModelAsset,
        InitialSize,

        /* --- Special --- */
        __Num__, // <-- Number of indices (Don't use this as index!).
    };
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================