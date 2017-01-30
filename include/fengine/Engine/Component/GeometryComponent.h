/*
 * Geometry component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_COMPONENT_H__
#define __FORK_GEOMETRY_COMPONENT_H__


#include "Engine/Component/Component.h"
#include "Scene/Node/GeometryNode.h"


namespace Fork
{

namespace Engine
{


//! Geometry component.
class FORK_EXPORT GeometryComponent : public Component
{
    
    public:
        
        //! Properties of this component.
        struct PropertyIDs
        {
            enum : PropertyKey
            {
                Asset = 0,
            };
        };

        GeometryComponent(PropertyFactory* propertyFactory = nullptr);

        Types Type() const override;
        std::string IDName() const override;

        static std::string GetIDName();

        //! Sets the new geometry and notifies all observers about its change.
        void SetupGeometry(const Scene::GeometryPtr& geometry);

        inline const Scene::GeometryNodePtr& GetSceneNode() const
        {
            return sceneNode_;
        }

    private:

        bool IsObservableOfInterest(const Types type) const override;
        void OnObservableChanged(const Component* observable) override;

        Scene::GeometryNodePtr sceneNode_;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================