/*
 * Editor wxWidgets input event mapping header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_COLOR_SCHEMES_H__
#define __FORK_EDITOR_COLOR_SCHEMES_H__


#include "Platform/Core/Frame.h"

#include <wx/event.h>
#include <functional>


namespace Fork
{

namespace Editor
{

namespace WxInputEventMapping
{


void OnKeyDown      (wxKeyEvent&    event, const Platform::Frame* frame);
void OnKeyUp        (wxKeyEvent&    event, const Platform::Frame* frame);
void OnChar         (wxKeyEvent&    event, const Platform::Frame* frame);
void OnMouseDblClick(wxMouseEvent&  event, const Platform::Frame* frame);
void OnMouseDown    (wxMouseEvent&  event, const Platform::Frame* frame);
void OnMouseUp      (wxMouseEvent&  event, const Platform::Frame* frame);
void OnMouseWheel   (wxMouseEvent&  event, const Platform::Frame* frame);

void BindEventHandler(
    wxEvtHandler* eventHandler,
    bool bindKeyDown        = true,
    bool bindKeyUp          = true,
    bool bindChar           = true,
    bool bindMouseDlbClick  = true,
    bool bindMouseDown      = true,
    bool bindMouseUp        = true,
    bool bindMouseWheel     = true
);


} // /namespace WxInputEventMapping

} // /namespace Editor

} // /namespace Fork


#endif



// ========================