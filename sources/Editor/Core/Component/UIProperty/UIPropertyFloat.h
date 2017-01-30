/*
 * Editor UI property float header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_FLOAT_H__
#define __FORK_EDITOR_UI_PROPERTY_FLOAT_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


//! Abstract component property class for floats.
class UIPropertyFloat : public Engine::Component::FloatProperty, public UIProperty
{
    
    public:
        
        UIPropertyFloat(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        wxFloatProperty* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================