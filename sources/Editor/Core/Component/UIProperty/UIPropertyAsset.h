/*
 * Editor UI property asset header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_ASSET_H__
#define __FORK_EDITOR_UI_PROPERTY_ASSET_H__


#include "UIProperty.h"


namespace Fork
{

namespace Editor
{


//! Abstract component property class for assets.
class UIPropertyAsset : public Engine::Component::AssetProperty, public UIProperty
{
    
    public:
        
        UIPropertyAsset(UIPropertyGrid& propGrid, const wxString& propName);

        void WriteToProperty() override;
        void ReadFromProperty(const PropertyState& state) override;

    private:
        
        wxStringProperty* prop_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================