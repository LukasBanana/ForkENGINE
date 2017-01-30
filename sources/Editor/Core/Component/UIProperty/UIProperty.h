/*
 * Editor UI property header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_H__
#define __FORK_EDITOR_UI_PROPERTY_H__


#include "../../UIExtensions/PropertyState.h"
#include "Engine/Component/Component.h"

#include <wx/propgrid/props.h>
#include <wx/propgrid/advprops.h>


namespace Fork
{

namespace Editor
{


class PropertyState;
class UIPropertyGrid;

//! UI component property interface.
class UIProperty
{
    
    public:
        
        virtual ~UIProperty()
        {
        }

        //! Writes the property value to the respective property in the UIPropertyGrid.
        virtual void WriteToProperty() = 0;
        //! Reads the property value from the specified property state.
        virtual void ReadFromProperty(const PropertyState& state) = 0;


};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================