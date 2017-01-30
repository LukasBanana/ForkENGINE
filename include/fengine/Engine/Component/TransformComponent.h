/*
 * Transform component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TRANSFORM_COMPONENT_H__
#define __FORK_TRANSFORM_COMPONENT_H__


#include "Engine/Component/Component.h"
#include "Scene/Node/SceneNode.h"


namespace Fork
{

namespace Engine
{


//! Transform component.
class FORK_EXPORT TransformComponent : public Component
{
    
    public:
        
        //! Properties of this component.
        struct PropertyIDs
        {
            enum : PropertyKey
            {
                Position = 0,
                Rotation,
                Scale,
            };
        };

        TransformComponent(PropertyFactory* propertyFactory = nullptr);

        Types Type() const override;
        std::string IDName() const override;

        static std::string GetIDName();

        //! Sets the new transformation and notifies all observers about its change.
        void SetupTransform(const Scene::Transform& transform);

        inline const Scene::Transform& GetTransform() const
        {
            return transform_;
        }
        inline Scene::Transform& GetTransform()
        {
            return transform_;
        }

    private:
        
        void OnThisComponentChanged() override;

        Scene::Transform    transform_;
        bool                isUpToDate_ = false;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================