/*
 * Editor default message boxes file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "DefaultMessageBoxes.h"
#include "../Language/LanguageLoader.h"

#include <wx/msgdlg.h> 


namespace Fork
{

namespace Editor
{


void FatalError(const wxString& message, wxWindow* parent)
{
    wxMessageBox("Error: " + message + "!", LANG_STRING(FatalError), wxOK | wxICON_ERROR, parent);
}


} // /namespace Editor

} // /namespace Fork



// ========================