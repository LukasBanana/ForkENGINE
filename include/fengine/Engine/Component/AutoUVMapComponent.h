/*
 * Auto UV map component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_AUTO_UV_MAP_COMPONENT_H__
#define __FORK_AUTO_UV_MAP_COMPONENT_H__


#include "Engine/Component/Component.h"
#include "Scene/Geometry/Modifier/GeometryAutoUVMapModifier.h"


namespace Fork
{

namespace Engine
{


/**
Auto UV map component.
\see Scene::GeometryAutoUVMapModifier
*/
class FORK_EXPORT AutoUVMapComponent : public Component
{
    
    public:
        
        //! Properties of this component.
        struct PropertyIDs
        {
            enum : PropertyKey
            {
                Update = 0,
                Global,
                OffsetU,
                OffsetV,
                ScaleU,
                ScaleV,
            };
        };

        AutoUVMapComponent(PropertyFactory* propertyFactory = nullptr);

        Types Type() const override;
        std::string IDName() const override;

        static std::string GetIDName();

    private:
        
        bool IsObservableOfInterest(const Types type) const override;
        void OnObservableChanged(const Component* observable) override;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================