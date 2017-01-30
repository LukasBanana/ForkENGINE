/*
 * Meta data component header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_META_DATA_COMPONENT_H__
#define __FORK_META_DATA_COMPONENT_H__


#include "Engine/Component/Component.h"


namespace Fork
{

namespace Engine
{


//! Meta Data component.
class FORK_EXPORT MetaDataComponent : public Component
{
    
    public:
        
        //! Properties of the this component.
        struct PropertyIDs
        {
            enum : PropertyKey
            {
                Name = 0,
                Enabled,
            };
        };

        MetaDataComponent(PropertyFactory* propertyFactory = nullptr);

        Types Type() const override;
        std::string IDName() const override;

        static std::string GetIDName();

        void SetupName(const std::string& name);

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================