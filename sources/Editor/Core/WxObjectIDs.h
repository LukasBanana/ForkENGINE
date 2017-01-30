/*
 * Editor wxWidgets object ID list header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_WXWIDGETS_OBJECT_ID_LIST_H__
#define __FORK_EDITOR_WXWIDGETS_OBJECT_ID_LIST_H__


#include <wx/defs.h>


namespace Fork
{

namespace Editor
{


struct WxObjectIDs
{
    enum : int
    {
        RibbonEditorToolbar = wxID_HIGHEST + 1,
    };
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================