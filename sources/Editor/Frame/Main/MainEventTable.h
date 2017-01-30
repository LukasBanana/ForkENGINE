/*
 * Editor main frame event table header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_EVENT_TABLE_H__
#define __FORK_EDITOR_MAIN_FRAME_EVENT_TABLE_H__


namespace Fork
{

namespace Editor
{


struct MainMenuIDs
{
    enum
    {
        /* --- File --- */
        NewProject = 0,
        OpenProject,
        SaveProject,
        SaveProjectAs,
        ProjectSettings,
        Quit,

        /* --- Edit --- */
        Undo,
        Redo,

        Cut,
        Copy,
        Paste,

        Delete,

        Preferences,

        /* --- GameObject --- */
        AddComponent,

        /* --- Transform --- */
        TransformTranslate,
        TransformRotate,
        TransformScale,

        SelectorOrientation,
        SelectorOrientationGlobal,
        SelectorOrientationLocal,
        SelectorOrientationView,

        SelectorPivot,
        SelectorPivotMedianPoint,
        SelectorPivotIndividualOrigins,
        SelectorPivotActiveElement,
        SelectorPivotBoundingBox,
        SelectorPivotMinimum,
        SelectorPivotMaximum,
        SelectorPivotGlobalCursor,

        TransformRaster,
        TransformRasterNone,
        TransformRasterRelative,
        TransformRasterAbsolute,
        TransformRaster_0_1,
        TransformRaster_0_25,
        TransformRaster_0_5,
        TransformRaster_1,
        TransformRaster_2_5,
        TransformRaster_5,

        EnableAxisX,
        EnableAxisY,
        EnableAxisZ,

        TransformPinCamera,
        SelectorShow,
        SelectionEnable,

        /* --- View --- */
        ViewRenderMode,
        ViewRenderModeShaded,
        ViewRenderModeSolid,
        ViewRenderModeWireframe,
        ViewRenderModeBoundingBox,

        ViewEditorViews,
        ViewEditorViewsSub1,
        ViewEditorViewsSub2,
        ViewEditorViewsSub3,

        /* --- Tools --- */
        //...

        /* --- Help --- */
        Manual,
        OnlineHelp,
        ShowTip,
        About,

        /* --- Popup: Component --- */
        RemoveComponent,
    };
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================