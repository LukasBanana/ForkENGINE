/*
 * Editor UI property grid observer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_PROPERTY_GRID_OBSERVER_H__
#define __FORK_EDITOR_UI_PROPERTY_GRID_OBSERVER_H__


#include "UIComponent.h"

#include <vector>


namespace Fork
{

namespace Editor
{


/**
Base class for all compontent entites and assets.
\remarks This observer class implements two function for notification:
"NotifyComponentChanged" (when a component has changed) and
"NotifyPGChanged" (when a property grid has changed).
*/
class UIPropertyGridObserver
{
    
    public:
        
        virtual ~UIPropertyGridObserver();

        //! Callback when a property grid has changed.
        virtual void OnPGChanged(const Engine::Component::Types componentType, const PropertyState& propertyState) = 0;

    protected:

        //! Attaches this observer to all associated property grids.
        void AttachObserver();
        //! Detaches this observer from all associated property grids.
        void DetachObserver();

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================