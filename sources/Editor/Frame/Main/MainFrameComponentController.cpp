/*
 * Main frame component controller file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "MainFrameComponentController.h"
#include "MainFrameMenuBar.h"
#include "MainFrame.h"
#include "Core/Exception/DefaultException.h"
#include "IO/Core/Log.h"
#include "../../Core/UIExtensions/UIMenuList.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Core/WxHelper.h"
#include "../../Core/Component/UIComponentFactory.h"

#include <wx/arrstr.h>


namespace Fork
{

namespace Editor
{


/*
 * Internal class
 */

class ComponentDropTarget : public wxTextDropTarget
{
    
    public:
        
        ComponentDropTarget(UIPropertyGrid* componentView) :
            componentView_{ componentView }
        {
        }

        bool OnDropText(wxCoord x, wxCoord y, const wxString& text) override
        {
            /* Get item where the mouse points to */
            auto hitTest = componentView_->HitTest(wxPoint(x, y));
            auto prop = hitTest.GetProperty();

            if (prop && prop->GetValue().IsType("string"))
            {
                //prop->GetClientData();

                auto propID = prop->GetName();
                componentView_->SetPropertyString(propID, text);
                componentView_->NotifyPGChanged(prop);
            }

            return true;
        }

    private:
        
        UIPropertyGrid* componentView_ = nullptr;

};


/*
 * MainFrameComponentController class
 */

void MainFrameComponentController::HideAllComponentCategories()
{
    componentView_->HideAllCategories();
}


/*
 * ======= Protected: =======
 */

void MainFrameComponentController::BuildComponentView(wxWindow* parent)
{
    /* Create main property grid for all components */
    componentView_ = MakeWxObject<UIPropertyGrid>(parent, wxDefaultPosition, wxSize(400, 400));
    componentView_->Bind(wxEVT_PG_RIGHT_CLICK, &MainFrameComponentController::OnPGRightClick, this);

    /* Build all property grid categories */
    UIComponentFactory::BuildPropertyGrid(*componentView_);
    componentView_->HideAllCategories();

    /* Set this class as drop target for the component view */
    componentView_->SetDropTarget(MakeWxObject<ComponentDropTarget>(componentView_));
}


/*
 * ======= Private: =======
 */

void MainFrameComponentController::OnPGRightClick(wxPropertyGridEvent& event)
{
    auto prop = event.GetProperty();
    if (prop && prop->IsCategory())
    {
        try
        {
            /* Store reference to currently selected component type */
            const auto name = prop->GetName().ToStdString();
            selectedComponentType_ = UIComponentFactory::QueryTypeByNamePrefix(name);

            /* Show popup menu */
            UIMenuList menu;
            MainFrameMenuBar::BuildMenuComponent(menu);
            MainFrame::Instance()->PopupMenu(&menu);
        }
        catch (const DefaultException& err)
        {
            /* Print error message */
            IO::Log::Error(err);
        }
    }
}


} // /namespace Editor

} // /namespace Fork



// ========================