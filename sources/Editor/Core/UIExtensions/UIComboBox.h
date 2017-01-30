/*
 * Editor UI combobox header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_COMBOBOX_H__
#define __FORK_EDITOR_UI_COMBOBOX_H__


#include <wx/combobox.h>
#include <initializer_list>


namespace Fork
{

namespace Editor
{


class UIComboBox : public wxComboBox
{
    
    public:
        
        UIComboBox(
            wxWindow* parent, const wxPoint& position = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, const wxString& value = "",
            long style = wxCB_READONLY
        );
        virtual ~UIComboBox();

        wxString GetSelectedString() const;

        void AddItems(const std::initializer_list<wxString>& items);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================