/*
 * Editor UI property vector3 header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_VECTOR3_H__
#define __FORK_EDITOR_UI_PROPERTY_VECTOR3_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


class PGVector3Property;

//! Abstract component property class for 3D vectors.
class UIPropertyVector3 : public Engine::Component::Vector3Property, public UIProperty
{
    
    public:
        
        UIPropertyVector3(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        PGVector3Property* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================