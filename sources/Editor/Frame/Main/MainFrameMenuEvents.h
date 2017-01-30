/*
 * Editor main frame menu events header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_MENU_EVENTS_H__
#define __FORK_EDITOR_MAIN_FRAME_MENU_EVENTS_H__


#include <wx/window.h>


namespace Fork
{

namespace Editor
{

class UIToolBar;

namespace MainFrameMenuEvents
{


void ProcessMenuEvent(wxWindow* parent, wxCommandEvent& event);


} // /namespace MainFrameMenuEvents

} // /namespace Editor

} // /namespace Fork


#endif



// ========================