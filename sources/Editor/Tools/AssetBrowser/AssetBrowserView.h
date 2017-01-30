/*
 * Editor asset browser view header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ASSET_BROWSER_VIEW_H__
#define __FORK_EDITOR_ASSET_BROWSER_VIEW_H__


#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"
#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/RenderSystem/RenderState/BlendState.h"
#include "Video/RenderSystem/RenderContext.h"
#include "AssetPreviewManager.h"
#include "AssetPreviewMask.h"


namespace Fork
{

namespace Editor
{


class AssetBrowserModel;

//! Editor asset browser view.
class AssetBrowserView
{
    
    public:
        
        AssetBrowserView(const AssetBrowserModel* model);

        /* === Functions === */
        
        /**
        Draws the entire asset browser view.
        \param[in] renderContext Raw-pointer to the render context where the view is to be rendered in.
        \param[in] viewport Specifies the viewport where the view is to be rendered in.
        \param[in] offsetPosition Specifies the asset preview offset position.
        This depends on the current view scroll position.
        \throws NullPointerException If 'renderContext' is null.
        */
        void Draw(
            Video::RenderContext* renderContext,
            const Video::Viewport& viewport,
            const Math::Point2f& offsetPosition,
            bool hasFocus = true
        );

        /**
        \throws NullPointerException If 'texture' is empty.
        */
        void DrawOverlayButton(
            const Video::Texture2D* texture, const Math::Point2f& centerPoint,
            bool highlight = false, float size = 1.0f
        ) const;

        //! Returns the font used to draw the asset labels.
        inline const Video::TextureFont* GetLabelFont() const
        {
            return font_.get();
        }

        inline const Video::Texture2D* GetPlayButtonTexture() const
        {
            return playButtonTex_.get();
        }
        inline const Video::Texture2D* GetStopButtonTexture() const
        {
            return stopButtonTex_.get();
        }

    private:
        
        /* === Functions === */

        void CreateRenderStates();
        void LoadImages();
        void CreateFont();

        void DrawAssetSelections(const Math::Point2f& offsetPosition, bool hasFocus);
        void DrawAssetPreviews(Video::RenderContext* renderContext, const Math::Point2f& offsetPosition);
        void DrawAssetOverlays(
            const Math::Point2f& offsetPosition,
            float viewportHeight,
            const Video::ColorRGBAf& labelColor,
            const Video::ColorRGBAf& labelSelectionColor
        );

        /* === Members === */

        Video::Texture2DPtr         assetPreviewSelectionTex_;

        Video::Texture2DPtr         playButtonTex_;
        Video::Texture2DPtr         stopButtonTex_;

        Video::BlendStatePtr        blendStateSetMask_;
        Video::BlendStatePtr        blendStateApplyMask_;
        Video::RasterizerStatePtr   rasterizerState_;

        Video::TextureFontPtr       font_;

        const AssetBrowserModel*    model_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================