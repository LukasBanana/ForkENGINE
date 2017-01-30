/*
 * Editor config panel graphics file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConfigPanelGraphics.h"
#include "../../Core/StaticLayoutConfig.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Core/UIExtensions/PropertyState.h"
#include "../../Core/WxHelper.h"
#include "../../Language/LanguageLoader.h"


namespace Fork
{

namespace Editor
{


struct PropertyIDs
{
    enum : int
    {
        Renderer,
        AntiAliasing,
        MultiSamples,
    };
};


using namespace StaticLayout::ConfigFrameLayout;

ConfigPanelGraphics::ConfigPanelGraphics(wxWindow* parent) :
    wxPanel{ parent }
{
    BuildPanelLayout();
}

void ConfigPanelGraphics::OnApplySettings(wxPropertyGridEvent& event)
{
    /*PropertyState state { event };

    switch (state.ExtractPropertyId())
    {
        case PropertyIDs::Renderer:
            
            break;
    }*/
}


/*
 * ======= Private: =======
 */

void ConfigPanelGraphics::BuildPanelLayout()
{
    auto topSizer = MakeWxObject<wxBoxSizer>(wxVERTICAL);

    /* Build property grid */
    propGrid_ = MakeWxObject<UIPropertyGrid>(this, wxDefaultPosition, wxSize(400, 400));

    propGrid_->AddPropertyCategory(LANG_STRING(RenderSystemSettings));
    {
        auto rendererProp     = propGrid_->AddEnumProperty(LANG_STRING( Renderer                 ), PROPERTY_ID( Renderer     ));
                                propGrid_->AddBoolProperty(LANG_STRING( AntiAliasing             ), PROPERTY_ID( AntiAliasing ));
        auto mutliSamplesProp = propGrid_->AddEnumProperty(LANG_STRING( AntiAliasingMultiSamples ), PROPERTY_ID( MultiSamples ));

        /* --- Setup items for renderer --- */
        #if 1//!!!
        rendererProp->AddChoice("OpenGL");
        rendererProp->AddChoice("Direct3D 11");
        rendererProp->SetChoiceSelection(0);
        #endif

        /* --- Setup items for multi-samples --- */
        mutliSamplesProp->AddChoice("2");
        mutliSamplesProp->AddChoice("4");
        mutliSamplesProp->AddChoice("8");
        mutliSamplesProp->AddChoice("16");
        mutliSamplesProp->SetChoiceSelection(2);
    }

    propGrid_->Bind(wxEVT_PG_CHANGED, &ConfigPanelGraphics::OnApplySettings, this);

    /* Integrate property grid into box sizer */
    topSizer->Add(propGrid_, 0, wxGROW | wxALL, buttonBorder);
    SetSizerAndFit(topSizer);
}


} // /namespace Editor

} // /namespace Fork



// ========================