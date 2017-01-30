/*
 * Editor UI property vector4 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_VECTOR4_H__
#define __FORK_EDITOR_UI_PROPERTY_VECTOR4_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


class PGVector4Property;

//! Abstract component property class for 4D vectors.
class UIPropertyVector4 : public Engine::Component::Vector4Property, public UIProperty
{
    
    public:
        
        UIPropertyVector4(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        PGVector4Property* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================