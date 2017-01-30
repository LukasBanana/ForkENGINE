/*
 * Editor default message boxes header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_DEFAULT_MESSAGE_BOXES_H__
#define __FORK_EDITOR_DEFAULT_MESSAGE_BOXES_H__


#include <wx/string.h>
#include <wx/window.h>


namespace Fork
{

namespace Editor
{


void FatalError(const wxString& message, wxWindow* parent = nullptr);


} // /namespace Editor

} // /namespace Fork


#endif



// ========================