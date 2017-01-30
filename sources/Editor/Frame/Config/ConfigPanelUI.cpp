/*
 * Editor config panel UI file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConfigPanelUI.h"
#include "../../Core/StaticLayoutConfig.h"
#include "../../Core/UIExtensions/UIComboBox.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Core/UIExtensions/PropertyState.h"
#include "../../Core/WxHelper.h"
#include "../../Language/LanguageLoader.h"
#include "../../Core/ColorScheme.h"
#include "../../Core/Config.h"

#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>


namespace Fork
{

namespace Editor
{


struct PropertyIDs
{
    enum : int
    {
        languageModule,
        colorSchemes,
    };
};


using namespace StaticLayout::ConfigFrameLayout;

ConfigPanelUI::ConfigPanelUI(wxWindow* parent) :
    wxPanel(parent)
{
    BuildPanelLayout();
}

void ConfigPanelUI::OnApplySettings(wxPropertyGridEvent& event)
{
    PropertyState state { event };

    switch (state.ExtractPropertyID())
    {
        case PropertyIDs::languageModule:
        {
            //...
        }
        break;

        case PropertyIDs::colorSchemes:
        {
            /* Change to selected color scheme */
            const auto& colorSchemes = Config::Instance()->colorSchemes;

            auto index = state.GetInt();
            if (index < static_cast<int>(colorSchemes.size()))
                ChangeColorScheme(colorSchemes[index]);
        }
        break;
    }
}


/*
 * ======= Private: =======
 */

void ConfigPanelUI::BuildPanelLayout()
{
    auto topSizer = MakeWxObject<wxBoxSizer>(wxVERTICAL);

    /* Build property grid */
    propGrid_ = MakeWxObject<UIPropertyGrid>(this, wxDefaultPosition, wxSize(400, 400));

    propGrid_->AddPropertyCategory(LANG_STRING(GeneralSettings));
    {
        auto languageModuleProp = propGrid_->AddEnumProperty(LANG_STRING( LanguageModule ), PROPERTY_ID( languageModule ));
        auto colorSchemesProp   = propGrid_->AddEnumProperty(LANG_STRING( ColorScheme    ), PROPERTY_ID( colorSchemes   ));

        /* --- Setup items for language --- */
        #if 1//!!!
        languageModuleProp->AddChoice(L"English (EN)");
        languageModuleProp->AddChoice(L"Deutsch (DE)");
        languageModuleProp->AddChoice(L"Français (FR)");
        languageModuleProp->AddChoice(L"\u0420\u0443\u0441\u0441\u043A\u0438\u0439 (RU)");
        #endif
        languageModuleProp->SetChoiceSelection(0);

        /* --- Setup items for color schemes --- */
        const auto& colorSchemes = Config::Instance()->colorSchemes;

        if (!colorSchemes.empty())
        {
            for (const auto& scheme : colorSchemes)
                colorSchemesProp->AddChoice(scheme.name);
            colorSchemesProp->SetChoiceSelection(0);
        }
    }

    propGrid_->Bind(wxEVT_PG_CHANGED, &ConfigPanelUI::OnApplySettings, this);

    /* Integrate property grid into box sizer */
    topSizer->Add(propGrid_, 0, wxGROW | wxALL, buttonBorder);
    SetSizerAndFit(topSizer);
}


} // /namespace Editor

} // /namespace Fork



// ========================