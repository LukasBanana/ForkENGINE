/*
 * Editor UI property string header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_STRING_H__
#define __FORK_EDITOR_UI_PROPERTY_STRING_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


//! Abstract component property class for strings.
class UIPropertyString : public Engine::Component::StringProperty, public UIProperty
{
    
    public:
        
        UIPropertyString(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        wxStringProperty* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================