/*
 * Component factory header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMPONENT_FACTORY_H__
#define __FORK_COMPONENT_FACTORY_H__


#include "Engine/Component/Component.h"


namespace Fork
{

namespace Engine
{


//! Component factory.
class FORK_EXPORT ComponentFactory
{
    
    public:
        
        virtual ~ComponentFactory();

        //! Instantiates the specified component type or null if 'type' is invalid.
        virtual std::unique_ptr<Component> InstantiateComponent(const Component::Types type) const;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================