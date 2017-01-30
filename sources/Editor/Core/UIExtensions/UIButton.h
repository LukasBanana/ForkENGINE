/*
 * Editor UI button header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_BUTTON_H__
#define __FORK_EDITOR_UI_BUTTON_H__


#include <wx/button.h>


namespace Fork
{

namespace Editor
{


class UIButton : public wxButton
{
    
    public:
        
        UIButton(
            wxWindow* parent, const wxPoint& position,
            const wxSize& size, const wxString& title = "", long style = 0
        );
        virtual ~UIButton();

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================