/*
 * Editor UI property color header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_COLOR_H__
#define __FORK_EDITOR_UI_PROPERTY_COLOR_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


//! Abstract component property class for colors.
class UIPropertyColor : public Engine::Component::ColorRGBAProperty, public UIProperty
{
    
    public:
        
        UIPropertyColor(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        wxColourProperty* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================