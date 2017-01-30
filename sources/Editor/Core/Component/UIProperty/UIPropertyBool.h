/*
 * Editor UI property boolean header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_BOOL_H__
#define __FORK_EDITOR_UI_PROPERTY_BOOL_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


//! Abstract component property class for booleans.
class UIPropertyBool : public Engine::Component::BoolProperty, public UIProperty
{
    
    public:
        
        UIPropertyBool(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        wxBoolProperty* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================