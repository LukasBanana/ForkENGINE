/*
 * Editor UI asset file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIAsset.h"
#include "../AssetBrowser.h"
#include "../../../Core/Devices.h"
#include "../../../Core/UIExtensions/UIPropertyGrid.h"

#include "../../../Frame/Main/MainFrame.h"
#include "../../../Frame/Main/MainFrameAssetController.h"
#include "../../../Frame/Main/MainFrameAssetDescriptor.h"
#include "../../../EditorView.h"
#include "Video/RenderSystem/RenderContext.h"
#include "../../../Core/UIExtensions/UIScreenFrame.h"

#include "ModelUIAsset.h"
#include "SoundUIAsset.h"
#include "TextureUIAsset.h"


namespace Fork
{

namespace Editor
{


static UITreeView* GetAssetTreeView()
{
    return MainFrame::Instance()->GetAssetCtrl()->GetTreeView();
}

/*
 * PreviewPartition structure
 */

UIAsset::PreviewPartition::PreviewPartition(
    AssetPreviewManager* previewMngr, const Video::Texture2DPtr& texture, unsigned int layer) :
        previewMngr { previewMngr },
        texture     { texture     },
        layer       { layer       }
{
}


/*
 * UIAsset class
 */

const Math::Size2i UIAsset::defaultPreviewSize { 256, 256 };

UIAsset::UIAsset(const PreviewPartition& previewPartition, const std::string& assetLabel) :
    previewPartition_{ previewPartition }
{
    ChangeLabel(assetLabel);

    /* Create asset preview render target */
    renderTarget_ = Devices::renderSystem->CreateRenderTarget();
    renderTarget_->Attach(previewPartition.texture, 0, previewPartition.layer);

    /* Create tree view entry */
    treeViewEntry_ = GetAssetTreeView()->CreateEntry(assetLabel);
}
UIAsset::~UIAsset()
{
    AssetBrowser::Instance()->GetModel()->UnregisterAsset(this);
}

void UIAsset::DrawPreview(const Math::Size2i& previewSize)
{
    BeginPreview(previewSize);
    DrawPreviewContent(previewSize);
    EndPreview();
}

void UIAsset::DrawOverlay(const AssetBrowserView* view, const Math::Point2f& position, const Math::Size2f& size)
{
    // dummy
}

void UIAsset::Instantiate()
{
    // dummy
}

void UIAsset::Select()
{
    //...
    OnSelect();
}

void UIAsset::Deselect()
{
    OnDeselect();
    //...
}

void UIAsset::GenerateMIPMaps()
{
    /* Generate MIP-maps only the asset's layer in the array texture */
    GetPreviewPartition().previewMngr->GenerateMIPMaps(GetPreviewPartition().layer);
}

void UIAsset::ChangeLabelConfig(const Video::Font* font, int maxWidth)
{
    multiLineLabel_.Change(font, label_, maxWidth);
}

int UIAsset::LabelHeight() const
{
    int fontHeight = multiLineLabel_.FontHeight();
    return fontHeight * static_cast<int>(multiLineLabel_.GetLines().size());
}

void UIAsset::ShowAttributes()
{
    auto propGrid = MainFrame::Instance()->GetAssetDesc()->GetAttribView();
    OnShowAttributes(*propGrid, AttribCategoryID());
    propGrid->ShowProperty(AttribCategoryID());
}

void UIAsset::BuildPropertyGrid(UIPropertyGrid& propGrid)
{
    ModelUIAsset    ::BuildPropertyGrid(propGrid, AttribCategoryID( Types::Geometry ));
    TextureUIAsset  ::BuildPropertyGrid(propGrid, AttribCategoryID( Types::Texture  ));
    //...
}


/*
 * ======= Protected: =======
 */

wxString UIAsset::AttribCategoryID(const Types type)
{
    return "attrib_asset_" + ToStr(static_cast<int>(type));
}

wxString UIAsset::AttribCategoryID() const
{
    return UIAsset::AttribCategoryID(Type());
}

void UIAsset::ChangeLabel(const std::string& assetLabel)
{
    label_ = assetLabel;
    multiLineLabel_.ChangeText(label_);
}

void UIAsset::OnSelect()
{
    // dummy
}

void UIAsset::OnDeselect()
{
    // dummy
}

void UIAsset::OnShowAttributes(UIPropertyGrid& propGrid, const wxString& namePrefix)
{
    // dummy
}


/*
 * ======= Private: =======
 */

void UIAsset::BeginPreview(const Math::Size2i& previewSize)
{
    /* Bind asset preview render-target */
    auto renderContext = Video::RenderContext::Active();

    renderContext->BindRenderTarget(GetRenderTarget());
    
    renderContext->ClearBuffers();
    renderContext->SetupViewport({ {}, previewSize });
}

void UIAsset::EndPreview()
{
    /* Unbind asset preview render-target */
    auto renderContext = Video::RenderContext::Active();
    renderContext->BindRenderTarget(renderContext->GetMainRenderTarget());
}


} // /namespace Editor

} // /namespace Fork



// ========================