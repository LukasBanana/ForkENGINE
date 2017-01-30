/*
 * Editor asset browser view file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AssetBrowserView.h"
#include "AssetBrowserModel.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Shader/StandardShader/ImageArrayDrawingConstantBuffers.h"
#include "../../Core/UIExtensions/UIScreenFrame.h"
#include "../../Core/Devices.h"
#include "../../Core/WxHelper.h"
#include "../../Core/Paths.h"

#include <wx/file.h>
#include <wx/dir.h>


namespace Fork
{

namespace Editor
{


using namespace Devices;

/* 
 * Internal functions
 */

static Video::Texture2DPtr LoadTexture(const std::string& filename)
{
    return renderSystem->GetTextureManager()->LoadTexture2D(Paths::dirImages + filename);
}

static inline Video::ColorRGBAf SelectionColor(bool hasFocus)
{
    return hasFocus ? Video::ColorRGBAf{ 1.0f, 1.0f, 0.0f } : Video::ColorRGBAf{};
}


/*
 * AssetBrowserView class
 */

AssetBrowserView::AssetBrowserView(const AssetBrowserModel* model) :
    model_{ model }
{
    /* Create resources */
    CreateRenderStates();
    LoadImages();
    CreateFont();
}

void AssetBrowserView::Draw(
    Video::RenderContext* renderContext, const Video::Viewport& viewport,
    const Math::Point2f& offsetPosition, bool hasFocus)
{
    ASSERT_POINTER(renderContext);

    /*
    Clear buffers
    -> this will also make the render context to the currnet one
    */
    renderContext->ClearBuffers();
    renderContext->SetupViewport(viewport);

    primitiveRenderer->BeginDrawing2D();
    {
        renderContext->ChangeState(renderSystem->GetDefaultBlendState());
        renderContext->ChangeState(rasterizerState_.get());

        DrawAssetSelections(offsetPosition, hasFocus);
        DrawAssetPreviews(renderContext, offsetPosition);

        if (model_->showPreviewMask)
            renderContext->ChangeState(renderSystem->GetDefaultBlendState());

        DrawAssetOverlays(offsetPosition, viewport.size.height, {}, SelectionColor(hasFocus));

        renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());
    }
    primitiveRenderer->EndDrawing2D();

    /* Show final result on the screen */
    renderContext->Present();
}

void AssetBrowserView::DrawOverlayButton(
    const Video::Texture2D* texture, const Math::Point2f& centerPoint, bool highlight, float size) const
{
    ASSERT_POINTER(texture);

    static const Video::ColorRGBAf highlightColor(0.6f, 0.7f, 1.0f);
    auto imageSize = texture->GetSize().Cast<float>()*size;

    primitiveRenderer->DrawImage(
        texture,
        centerPoint - Math::Point2f{ imageSize.width/2, imageSize.height/2 },
        imageSize,
        (highlight ? highlightColor : Video::ColorRGBAf())
    );
}


/*
 * ======= Private: =======
 */

void AssetBrowserView::CreateRenderStates()
{
    /* Create blend states to draw the preview mask */
    Video::BlendState::Description blendStateDesc;
    {
        blendStateDesc.srcBlend         = Video::BlendState::BlendOps::Zero;
        blendStateDesc.destBlend        = Video::BlendState::BlendOps::One;
        blendStateDesc.srcBlendAlpha    = Video::BlendState::BlendOps::SrcAlpha;
        blendStateDesc.destBlendAlpha   = Video::BlendState::BlendOps::Zero;
    }
    blendStateSetMask_ = renderSystem->CreateBlendState(blendStateDesc);
    {
        blendStateDesc.srcBlend         = Video::BlendState::BlendOps::DestAlpha;
        blendStateDesc.destBlend        = Video::BlendState::BlendOps::InvDestAlpha;
        blendStateDesc.srcBlendAlpha    = Video::BlendState::BlendOps::One;
        blendStateDesc.destBlendAlpha   = Video::BlendState::BlendOps::Zero;
    }
    blendStateApplyMask_ = renderSystem->CreateBlendState(blendStateDesc);

    /* Create rasterizer state */
    Video::RasterizerState::Description rasterizerStateDesc;
    {
        rasterizerStateDesc.isAntiAliasedLineEnable = false;
        rasterizerStateDesc.isMultiSampleEnable     = false;
    }
    rasterizerState_ = renderSystem->CreateRasterizerState(rasterizerStateDesc);
}

void AssetBrowserView::LoadImages()
{
    assetPreviewSelectionTex_   = LoadTexture("AssetPreviewMask.png");
    playButtonTex_              = LoadTexture("Button.Play.png");
    stopButtonTex_              = LoadTexture("Button.Stop.png");
}

void AssetBrowserView::CreateFont()
{
    /* Create font for asset previews */
    font_ = primitiveRenderer->CreateTextureFont({ "Calibri", 15 });
}

void AssetBrowserView::DrawAssetSelections(const Math::Point2f& offsetPosition, bool hasFocus)
{
    static const float border = 2.0f;

    for (auto selectedAsset : model_->GetSelectedAssets())
    {
        const auto position = selectedAsset->previewPosition + offsetPosition - Math::Point2f(border);
        const auto size = model_->previewSize + Math::Size2f(border*2);

        auto selectionColor = model_->selectionColor;

        if (!hasFocus)
            selectionColor = Video::ColorRGBf(0.8f);

        /* Draw selection highlighting */
        if (model_->showPreviewMask)
        {
            primitiveRenderer->DrawImage(
                assetPreviewSelectionTex_.get(),
                position, size, selectionColor
            );
        }
        else
            primitiveRenderer->DrawRect({ position, size }, selectionColor);
    }
}

void AssetBrowserView::DrawAssetPreviews(Video::RenderContext* renderContext, const Math::Point2f& offsetPosition)
{
    if (model_->showPreviewMask)
    {
        /* Draw preview mask for all assets */
        renderContext->ChangeState(blendStateSetMask_.get());

        model_->GetAssetPreviewMask()->SetupAndDrawImageArray(
            *primitiveRenderer, offsetPosition, model_->previewSize
        );

        renderContext->ChangeState(blendStateApplyMask_.get());
    }

    /* Draw all asset previews */
    for (auto& previewMngr : model_->GetAssetPreviewManagers())
    {
        previewMngr->SetupAndDrawImageArray(
            *primitiveRenderer, offsetPosition, model_->previewSize
        );
    }
}

//!TODO! -> optimize this for text drawing!!!
void AssetBrowserView::DrawAssetOverlays(
    const Math::Point2f& offsetPosition, float viewportHeight,
    const Video::ColorRGBAf& labelColor, const Video::ColorRGBAf& labelSelectionColor)
{
    const auto previewSize = model_->previewSize;
    const auto basePos = offsetPosition + Math::Point2f{ std::floor(previewSize.width/2), previewSize.height };
    auto fontHeight = static_cast<float>(font_->GetDesc().size.height);

    for (const auto& asset : model_->GetAssets())
    {
        /* Check if text is visible */
        auto pos = basePos + asset->previewPosition;
        auto numLines = asset->GetLabelMultiLines().size();

        if (pos.y <= -fontHeight*numLines)
        {
            /* Text is too high -> try next entry */
            continue;
        }
        if (pos.y >= viewportHeight)
        {
            /* Text is too low -> now further text drawing */
            return;
        }

        /* Draw label lines for the current asset */
        const bool isSelected = model_->IsAssetSelected(asset.get());
        const auto& color = (isSelected ? labelSelectionColor : labelColor);

        for (const auto& line : asset->GetLabelMultiLines())
        {
            primitiveRenderer->DrawText2D(
                font_.get(), pos, line, color,
                Video::PrimitiveRenderer::FontDrawFlags::CenterX
            );
            pos.y += fontHeight;
        }

        /* Draw asset specific overlay */
        asset->DrawOverlay(this, offsetPosition + asset->previewPosition, previewSize);
    }
}


} // /namespace Editor

} // /namespace Fork



// ========================