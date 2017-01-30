/*
 * Editor UI property factory header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_FACTORY_H__
#define __FORK_EDITOR_UI_PROPERTY_FACTORY_H__


#include "Engine/Component/Component.h"

#include <wx/string.h>


namespace Fork
{

namespace Editor
{


class UIPropertyGrid;

/**
Property factory for the UIComponent class using in the editor.
\see UIComponent
*/
class UIPropertyFactory : public Engine::Component::PropertyFactory
{
    
    public:
        
        using PropertyKey = Engine::Component::PropertyKey;

        std::unique_ptr< Engine::Component::BoolProperty      > MakeBoolProperty        (const PropertyKey& key) override;
        std::unique_ptr< Engine::Component::IntProperty       > MakeIntProperty         (const PropertyKey& key) override;
        std::unique_ptr< Engine::Component::FloatProperty     > MakeFloatProperty       (const PropertyKey& key) override;
        std::unique_ptr< Engine::Component::StringProperty    > MakeStringProperty      (const PropertyKey& key) override;
        std::unique_ptr< Engine::Component::Vector3Property   > MakeVector3Property     (const PropertyKey& key) override;
        std::unique_ptr< Engine::Component::Vector4Property   > MakeVector4Property     (const PropertyKey& key) override;
        //std::unique_ptr< Engine::Component::ColorRGBProperty  > MakeColorRGBProperty    (const PropertyKey& key) override;
        std::unique_ptr< Engine::Component::ColorRGBAProperty > MakeColorRGBAProperty   (const PropertyKey& key) override;
        std::unique_ptr< Engine::Component::AssetProperty     > MakeAssetProperty       (const PropertyKey& key) override;

        UIPropertyGrid* propertyGridRef = nullptr;
        std::string     namePrefix;

    private:
        
        template <class Comp> std::unique_ptr<Comp> MakeUIProperty(const PropertyKey& key);

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================