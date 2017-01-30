/*
 * Editor language EN library file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Interface.h"


FORK_EXPORT const wchar_t* LanguageName()
{
    return L"English";
}

FORK_EXPORT const wchar_t* LanguageString(unsigned int index)
{
    static const wchar_t* stringList[] =
    {
        /* --- Common --- */
        L"Unnamed",
        L"Default",
        L"Common",
        L"Graphics",
        L"Fatal Error",
        L"Terminal",
        L"Texture",
        L"Model",
        L"Format",
        L"Width",
        L"Height",
        L"Depth",
        L"Errors",
        L"Axis",

        /* --- Main Frame --- */
        L"Components",
        L"Scene Graph",
        L"Assets",
        L"Asset Description",

        /* --- Main Frame Ribbon Bar --- */
        L"Edit",
        L"Toolbar",
        L"Transformation",
        L"View",

        L"Terrain",

        L"Game",
        
        /* --- Main Menu: Editor --- */
        L"Edit&or",

        L"&Quit",
        L"Quits the entire application.",

        /* --- Main Menu: File --- */
        L"&Project",

        L"&New Project\tCtrl+N",
        L"Generates a new project.",

        L"&Open Project ...\tCtrl+O",
        L"Opens an existing project from file.",

        L"&Save Project\tCtrl+S",
        L"Saves the current project.",

        L"Save Project &As ...",
        L"Saves the current project under a new filename.",

        L"&Project Settings ...",
        L"Opens the window for project settings.",

        /* --- Main Menu: Edit --- */
        L"&Edit",

        L"&Undo\tCtrl+Z",
        L"Undo previous task.",
        L"&Redo\tCtrl+Y",
        L"Redo previous task.",

        L"&Cut\tCtrl+X",
        L"Cuts all selected objects into clipboard.",
        L"C&opy\tCtrl+C",
        L"Copys all selected objects into clipboard.",
        L"&Paste\tCtrl+V",
        L"Pastes all objects from clipboard.",

        L"&Delete\tDel",
        L"Deletes all selected objects.",

        L"Preference&s ...",
        L"Opens the window for editor preferences.",

        /* --- Main Menu: Entity --- */
        L"&Entity",

        L"&Add Component",
        L"Adds a new component to all selected game objects.",
        L"Remove Component",
        L"Removes the selected component",

        /* --- Main Menu: Transform */
        L"&Translate\t1",
        L"Translates all selected entities",
        L"&Rotate\t2",
        L"Rotates all selected entities",
        L"&Scale\3",
        L"Scales all selected entities",

        L"Selector &Orientation Alignment",
        L"Changes the selector orientation alignment",
        L"&Global\tShift+1",
        L"&Local\tShift+2",
        L"&View\tShift+3",

        L"Selector &Pivot Alignment",
        L"&Median Point\tCtrl+1",
        L"&Individual Origins\tCtrl+2",
        L"&Active Element\tCtrl+3",
        L"&Bounding Box\tCtrl+4",
        L"Mi&nimum\tCtrl+5",
        L"Ma&ximum\tCtrl+6",
        L"&Global Cursor\tCtrl+7",
        L"Changes the selector pivot alignment",

        L"Transformation &Raster\tCtrl+R",
        L"&No Raster",
        L"&Relative Raster",
        L"&Absolute Raster",

        L"Enable &X Axis",
        L"Enable &Y Axis",
        L"Enable &Z Axis",
        L"Enables the axes X, Y, Z",

        L"&Pin Camera\tCtrl+P",
        L"Pins the camera when translating entites",
        L"Configures the transformation raster",
        L"Show/Hide Selector",
        L"Shows or hides the selector",
        L"Enable/Disable Selection",
        L"Enables or disables entity selection",

        /* --- Main Menu: View --- */
        L"&Render Mode",
        L"Changes the render mode for the current view",
        L"&Shaded",
        L"S&olid",
        L"&Wireframe",
        L"&Bounding Box",
        L"Editor &Views",
        L"Shows or hides specific editor sub views",
        L"Sub View",
        L"Camera move speed",

        /* --- Main Menu: Help --- */
        L"&Help",

        L"&Manual",
        L"Shows the man page.",

        L"Online &Help",
        L"Opens the online help in the web browser.",

        L"Show &Tip",
        L"Shows the tip dialog.",

        L"&About",
        L"Shows the about dialog.",

        /* --- Config Frame (Common) --- */
        L"Selector Settings",
        L"Move Free In Raster",
        L"Draw Selected Axes Only",
        L"Draw Previous Transformation",
        L"Draw Axes Labels",
        L"Draw Thick Axes",
        L"Picking Only Translation",
        L"Align Cursor",
        L"Axes Colors",
        L"Normal Color",
        L"Selection Color",
        
        L"Editor View Settings",
        L"Projector Size",
        L"Projector Transition (in ms.)",

        L"Asset Browser Settings",
        L"Asset Preview Size",
        L"Show Preview Mask",

        /* --- Config Frame (UI) --- */
        L"Editor Preferences",
        L"User Interface",
        L"General Settings",
        L"Color Scheme",
        L"Language",

        /* --- Config Frame (Graphics) --- */
        L"Render System Settings",
        L"Renderer",
        L"Anti Aliasing",
        L"Anti Aliasing Multi Samples",

        /* --- Hierarchy Viewer Frames --- */
        L"Geometry Hierarchy Viewer",

        /* --- About Dialog --- */
        L"ForkEd is the ForkENGINE (WYSIWYG) world editor",

        /* --- Component: Meta Data --- */
        L"Meta Data",
        L"Name",
        L"Enabled",

        /* --- Component: Transform --- */
        L"Transform",
        L"Position",
        L"Rotation",
        L"Scaling",

        /* --- Component: Geometry --- */
        L"Geometry",
        L"Asset",

        /* --- Component: Auto UV Map--- */
        L"Auto UV Map",
        L"Update",
        L"Global",
        L"Offset U",
        L"Offset V",
        L"Scale U",
        L"Scale V",

        /* --- Asset: Texture --- */
        L"Texture Asset",

        /* --- Asset: Model --- */
        L"Model Asset",
        L"Initial Size",
    };

    if (index >= sizeof(stringList)/sizeof(stringList[0]))
        return NULL;

    return stringList[index];
}



// ========================