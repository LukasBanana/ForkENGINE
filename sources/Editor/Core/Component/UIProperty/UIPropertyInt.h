/*
 * Editor UI property integer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_INT_H__
#define __FORK_EDITOR_UI_PROPERTY_INT_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


//! Abstract component property class for integers.
class UIPropertyInt : public Engine::Component::IntProperty, public UIProperty
{
    
    public:
        
        UIPropertyInt(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        wxIntProperty* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================