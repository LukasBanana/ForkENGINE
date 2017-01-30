/*
 * Main frame component controller header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MAIN_FRAME_COMPONENT_CTRL_H__
#define __FORK_EDITOR_MAIN_FRAME_COMPONENT_CTRL_H__


#include "Engine/Component/Component.h"

#include <wx/window.h>
#include <wx/propgrid/propgrid.h>
#include <wx/dnd.h>


namespace Fork
{

namespace Editor
{


class UIPropertyGrid;

class MainFrameComponentController
{
    
    public:
        
        //! Hides all component categories in the property grid.
        void HideAllComponentCategories();

        //! Returns the component property grid view.
        inline UIPropertyGrid* GetComponentView() const
        {
            return componentView_;
        }

        //! Returns the previously selected component type.
        inline Engine::Component::Types GetSelectedComponentType() const
        {
            return selectedComponentType_;
        }

    protected:
        
        friend class MainFrame;

        void BuildComponentView(wxWindow* parent);

    private:
        
        void OnPGRightClick(wxPropertyGridEvent& event);

        UIPropertyGrid*             componentView_          = nullptr;
        Engine::Component::Types    selectedComponentType_  = Engine::Component::Types::Notification;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================