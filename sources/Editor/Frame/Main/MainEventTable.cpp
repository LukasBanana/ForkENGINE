/*
 * Editor main frame event table file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainEventTable.h"
#include "MainFrame.h"


namespace Fork
{

namespace Editor
{


#define MENU_ITEM(n)            EVT_MENU(MainMenuIDs::n, MainFrame::OnMenuStates)
#define MENU_ITEM_DROP_DOWN(n)  EVT_AUITOOLBAR_TOOL_DROPDOWN(MainMenuIDs::n, MainFrame::OnToolbarDropDown )

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    /* --- File --- */
    MENU_ITEM( NewProject      )
    MENU_ITEM( OpenProject     )
    MENU_ITEM( SaveProject     )
    MENU_ITEM( SaveProjectAs   )
    MENU_ITEM( ProjectSettings )
    MENU_ITEM( Quit            )

    /* --- Edit --- */
    MENU_ITEM( Undo        )
    MENU_ITEM( Redo        )
    MENU_ITEM( Cut         )
    MENU_ITEM( Copy        )
    MENU_ITEM( Paste       )
    MENU_ITEM( Delete      )
    MENU_ITEM( Preferences )

    /* --- GameObject --- */
    MENU_ITEM( AddComponent )

    /* --- Transform --- */
    MENU_ITEM( TransformTranslate )
    MENU_ITEM( TransformRotate    )
    MENU_ITEM( TransformScale     )

    MENU_ITEM( SelectorOrientationGlobal )
    MENU_ITEM( SelectorOrientationLocal  )
    MENU_ITEM( SelectorOrientationView   )

    MENU_ITEM( SelectorPivotMedianPoint       )
    MENU_ITEM( SelectorPivotIndividualOrigins )
    MENU_ITEM( SelectorPivotActiveElement     )
    MENU_ITEM( SelectorPivotBoundingBox       )
    MENU_ITEM( SelectorPivotMinimum           )
    MENU_ITEM( SelectorPivotMaximum           )
    MENU_ITEM( SelectorPivotGlobalCursor      )

    MENU_ITEM( TransformRasterNone     )
    MENU_ITEM( TransformRasterRelative )
    MENU_ITEM( TransformRasterAbsolute )
    MENU_ITEM( TransformRaster_0_1     )
    MENU_ITEM( TransformRaster_0_25    )
    MENU_ITEM( TransformRaster_0_5     )
    MENU_ITEM( TransformRaster_1       )
    MENU_ITEM( TransformRaster_2_5     )
    MENU_ITEM( TransformRaster_5       )

    MENU_ITEM( EnableAxisX )
    MENU_ITEM( EnableAxisY )
    MENU_ITEM( EnableAxisZ )

    /* --- View --- */
    MENU_ITEM( ViewRenderModeShaded      )
    MENU_ITEM( ViewRenderModeSolid       )
    MENU_ITEM( ViewRenderModeWireframe   )
    MENU_ITEM( ViewRenderModeBoundingBox )
    MENU_ITEM( ViewEditorViewsSub1       )
    MENU_ITEM( ViewEditorViewsSub2       )
    MENU_ITEM( ViewEditorViewsSub3       )

    /* --- Help --- */
    MENU_ITEM( Manual     )
    MENU_ITEM( OnlineHelp )
    MENU_ITEM( ShowTip    )
    MENU_ITEM( About      )

    /* --- Popup: Component --- */
    MENU_ITEM( RemoveComponent )

    /* --- Toolbars --- */
    MENU_ITEM_DROP_DOWN( OpenProject         )
    MENU_ITEM_DROP_DOWN( SelectorOrientation )
    MENU_ITEM_DROP_DOWN( SelectorPivot       )
    MENU_ITEM_DROP_DOWN( TransformRaster     )
    MENU_ITEM_DROP_DOWN( ViewRenderMode      )
    MENU_ITEM_DROP_DOWN( ViewEditorViews     )
END_EVENT_TABLE()

#undef MENU_ITEM
#undef MENU_ITEM_DROP_DOWN


} // /namespace Editor

} // /namespace Fork



// ========================