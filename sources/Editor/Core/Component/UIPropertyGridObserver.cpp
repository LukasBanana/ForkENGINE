/*
 * Editor UI property grid observer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIPropertyGridObserver.h"
#include "../UIExtensions/UIPropertyGrid.h"
#include "../../Frame/Main/MainFrameComponentController.h"
#include "../../Frame/Main/MainFrame.h"


namespace Fork
{

namespace Editor
{


UIPropertyGridObserver::~UIPropertyGridObserver()
{
    DetachObserver();
}


/*
 * ======= Protected: =======
 */

// Returns the main property-grid of the component controller
static UIPropertyGrid* ComponentPG()
{
    return MainFrame::Instance()->GetComponentCtrl()->GetComponentView();
}

void UIPropertyGridObserver::AttachObserver()
{
    ComponentPG()->AttachObserver(*this);
}

void UIPropertyGridObserver::DetachObserver()
{
    ComponentPG()->DetachObserver(*this);
}


} // /namespace Editor

} // /namespace Fork



// ========================