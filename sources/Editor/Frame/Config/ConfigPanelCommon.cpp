/*
 * Editor config panel common file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ConfigPanelCommon.h"
#include "../../Core/StaticLayoutConfig.h"
#include "../../Core/UIExtensions/UIPropertyGrid.h"
#include "../../Core/UIExtensions/PropertyState.h"
#include "../../Core/WxHelper.h"
#include "../../Language/LanguageLoader.h"
#include "../../Core/Devices.h"
#include "../../Entity/EntitySelector.h"
#include "../../EditorView.h"
#include "../../Tools/AssetBrowser/AssetBrowser.h"
#include "../../Core/DefaultColors.h"
#include "../Main/MainFrame.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace Editor
{


struct PropertyIDs
{
    enum : int
    {
        /* --- Selector Settings --- */
        MoveFreeInRaster,
        DrawSelectedAxesOnly,
        DrawPreviousTransform,
        DrawAxesLabels,
        DrawThickAxes,
        PickingOnlyTranslation,
        AlignCursor,
        XNormalColor,
        XSelectColor,
        YNormalColor,
        YSelectColor,
        ZNormalColor,
        ZSelectColor,

        /* --- Editor View Settings --- */
        ProjectorSize,
        ProjectorTransition,

        /* --- Asset Browser Settings --- */
        AssetPreviewSize,
        ShowPreviewMask,
    };
};


using namespace StaticLayout::ConfigFrameLayout;
using Devices::entitySelector;


/*
 * Internal functions
 */

static inline Math::Vector3<Utility::SelectorView::Configuration::Axis>& SelectorAxes()
{
    return entitySelector->GetView()->config.axes;
};


/*
 * ConfigPanelCommon class
 */

ConfigPanelCommon::ConfigPanelCommon(wxWindow* parent) :
    wxPanel(parent)
{
    BuildPanelLayout();
}

void ConfigPanelCommon::OnApplySettings(wxPropertyGridEvent& event)
{
    auto UpdateSelectorPlaneColor = []()
    {
        auto& config = entitySelector->GetView()->config;
        config.selectPlaneColor = (config.axes.x.selectColor + config.axes.y.selectColor + config.axes.z.selectColor) / 3.0f;
        config.selectPlaneColor.a = 0.5f;
    };

    /* Select property state */
    PropertyState state { event };

    switch (state.ExtractPropertyID())
    {
        /* --- Selector Settings --- */

        case PropertyIDs::MoveFreeInRaster:
            entitySelector->GetModel()->raster.moveFreeInRaster = state.GetBool();
            break;
        case PropertyIDs::DrawSelectedAxesOnly:
            entitySelector->config.drawSelectedAxesOnly = state.GetBool();
            break;
        case PropertyIDs::DrawPreviousTransform:
            entitySelector->config.drawPrevTransform = state.GetBool();
            break;
        case PropertyIDs::DrawAxesLabels:
            entitySelector->config.drawAxesLabels = state.GetBool();
            break;
        case PropertyIDs::DrawThickAxes:
            if (state.GetBool())
            {
                entitySelector->GetView()->config.normalAxisWidth = 2.0f;
                entitySelector->GetView()->config.selectAxisWidth = 3.0f;
            }
            else
            {
                entitySelector->GetView()->config.normalAxisWidth = 1.0f;
                entitySelector->GetView()->config.selectAxisWidth = 1.0f;
            }
            break;
        case PropertyIDs::AlignCursor:
            entitySelector->GetModel()->alignCursor = state.GetBool();
            break;

        case PropertyIDs::PickingOnlyTranslation:
            entitySelector->GetModel()->pickingOnlyTranslation = state.GetBool();
            break;

        case PropertyIDs::XNormalColor:
            SelectorAxes().x.normalColor = state.GetColorRGB().Cast<float>();
            break;
        case PropertyIDs::XSelectColor:
            SelectorAxes().x.selectColor = state.GetColorRGB().Cast<float>();
            UpdateSelectorPlaneColor();
            break;

        case PropertyIDs::YNormalColor:
            SelectorAxes().y.normalColor = state.GetColorRGB().Cast<float>();
            break;
        case PropertyIDs::YSelectColor:
            SelectorAxes().y.selectColor = state.GetColorRGB().Cast<float>();
            UpdateSelectorPlaneColor();
            break;

        case PropertyIDs::ZNormalColor:
            SelectorAxes().z.normalColor = state.GetColorRGB().Cast<float>();
            break;
        case PropertyIDs::ZSelectColor:
            SelectorAxes().z.selectColor = state.GetColorRGB().Cast<float>();
            UpdateSelectorPlaneColor();
            break;

        /* --- Editor View Settings --- */

        case PropertyIDs::ProjectorSize:
            EditorView::globalConfig.projectorVisualSize = state.GetInt();
            break;
        case PropertyIDs::ProjectorTransition:
            ForEach(
                MainFrame::Instance()->GetEditorViews(),
                [&](const std::unique_ptr<EditorView>& editorView)
                {
                    editorView->ChangeProjectorTransition(static_cast<unsigned long long>(state.GetInt()));
                }
            );
            break;

        /* --- Asset Browser Settings --- */

        case PropertyIDs::AssetPreviewSize:
            AssetBrowser::Instance()->GetModel()->previewSize = Math::Size2f(static_cast<float>(state.GetInt()));
            AssetBrowser::Instance()->UpdateAssetPreviewLocations();
            break;

        case PropertyIDs::ShowPreviewMask:
            AssetBrowser::Instance()->GetModel()->showPreviewMask = state.GetBool();
            break;
    }
}


/*
 * ======= Private: =======
 */

void ConfigPanelCommon::BuildPanelLayout()
{
    auto topSizer = MakeWxObject<wxBoxSizer>(wxVERTICAL);

    /* Build property grid */
    propGrid_ = MakeWxObject<UIPropertyGrid>(this, wxDefaultPosition, wxSize(400, 600));

    auto parentProp = propGrid_->AddPropertyCategory(LANG_STRING(SelectorSettings));
    {
        propGrid_->AddBoolProperty(LANG_STRING( MoveFreeInRaster       ), PROPERTY_ID( MoveFreeInRaster       ));
        propGrid_->AddBoolProperty(LANG_STRING( DrawSelectedAxesOnly   ), PROPERTY_ID( DrawSelectedAxesOnly   ));
        propGrid_->AddBoolProperty(LANG_STRING( DrawPreviousTransform  ), PROPERTY_ID( DrawPreviousTransform  ));
        propGrid_->AddBoolProperty(LANG_STRING( DrawAxesLabels         ), PROPERTY_ID( DrawAxesLabels         ));
        propGrid_->AddBoolProperty(LANG_STRING( DrawThickAxes          ), PROPERTY_ID( DrawThickAxes          ));
        propGrid_->AddBoolProperty(LANG_STRING( PickingOnlyTranslation ), PROPERTY_ID( PickingOnlyTranslation ));
        propGrid_->AddBoolProperty(LANG_STRING( AlignCursor            ), PROPERTY_ID( AlignCursor            ));

        propGrid_->AddPropertyCategory(LANG_STRING(AxesColors), wxPG_LABEL, parentProp);
        {
            propGrid_->AddColorRGBProperty("X " + LANG_STRING(Axis) + " " + LANG_STRING( NormalColor    ), PROPERTY_ID(XNormalColor), RGBToWxColor(SelectorAxes().x.normalColor.ToRGB()));
            propGrid_->AddColorRGBProperty("X " + LANG_STRING(Axis) + " " + LANG_STRING( SelectionColor ), PROPERTY_ID(XSelectColor), RGBToWxColor(SelectorAxes().x.selectColor.ToRGB()));
            propGrid_->AddColorRGBProperty("Y " + LANG_STRING(Axis) + " " + LANG_STRING( NormalColor    ), PROPERTY_ID(YNormalColor), RGBToWxColor(SelectorAxes().y.normalColor.ToRGB()));
            propGrid_->AddColorRGBProperty("Y " + LANG_STRING(Axis) + " " + LANG_STRING( SelectionColor ), PROPERTY_ID(YSelectColor), RGBToWxColor(SelectorAxes().y.selectColor.ToRGB()));
            propGrid_->AddColorRGBProperty("Z " + LANG_STRING(Axis) + " " + LANG_STRING( NormalColor    ), PROPERTY_ID(ZNormalColor), RGBToWxColor(SelectorAxes().z.normalColor.ToRGB()));
            propGrid_->AddColorRGBProperty("Z " + LANG_STRING(Axis) + " " + LANG_STRING( SelectionColor ), PROPERTY_ID(ZSelectColor), RGBToWxColor(SelectorAxes().z.selectColor.ToRGB()));
        }
    }

    propGrid_->AddPropertyCategory(LANG_STRING(EditorViewSettings));
    {
        propGrid_->AddIntProperty(LANG_STRING( ProjectorSize       ), PROPERTY_ID( ProjectorSize       ), 100, 75, 200, true);
        propGrid_->AddIntProperty(LANG_STRING( ProjectorTransition ), PROPERTY_ID( ProjectorTransition ), 250, 0, 1000, true);
    }

    propGrid_->AddPropertyCategory(LANG_STRING(AssetBrowserSettings));
    {
        propGrid_->AddIntProperty   (LANG_STRING( AssetPreviewSize ), PROPERTY_ID( AssetPreviewSize ), 64, 32, 256, true);
        propGrid_->AddBoolProperty  (LANG_STRING( ShowPreviewMask  ), PROPERTY_ID( ShowPreviewMask  ), true             );
    }

    propGrid_->Bind(wxEVT_PG_CHANGED, &ConfigPanelCommon::OnApplySettings, this);

    /* Integrate property grid into box sizer */
    topSizer->Add(propGrid_, 0, wxGROW | wxALL, buttonBorder);
    SetSizerAndFit(topSizer);
}


} // /namespace Editor

} // /namespace Fork



// ========================