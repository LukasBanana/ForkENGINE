/*
 * Editor UI asset header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_ASSET_H__
#define __FORK_EDITOR_UI_ASSET_H__


#include "Core/DeclPtr.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "../../../Core/Component/ComponentContainer.h"
#include "../../../Core/UIExtensions/UITreeView.h"
#include "Utility/MultiLineFontString.h"
#include "Engine/Asset/Asset.h"

#include <vector>


namespace Fork
{

namespace Editor
{


class AssetPreviewManager;
class AssetBrowserView;
class UIPropertyGrid;

DECL_SHR_PTR(UIAsset);

//! Editor asset base class.
class UIAsset
{
    
    public:
        
        typedef Engine::Asset::Types Types;

        //! Default preview size: { 256, 256 }.
        static const Math::Size2i defaultPreviewSize;

        /* === Structures === */

        //! Asset flags enumeration.
        struct Flags
        {
            typedef unsigned char DataType;
            enum : DataType
            {
                HasRealtimePreview = (1 << 0), //!< Preview can be updated in real-time by moving the mouse while holding the right mouse button.
            };
        };

        //! Asset preview partition structure.
        struct PreviewPartition
        {
            PreviewPartition() = default;
            PreviewPartition(
                AssetPreviewManager* previewMngr,
                const Video::Texture2DPtr& texture,
                unsigned int layer
            );

            AssetPreviewManager*    previewMngr = nullptr;  //!< Reference to the asset preview manager.
            Video::Texture2DPtr     texture     = nullptr;  //!< 2D array texture.
            unsigned int            layer       = 0;        //!< 2D array texture layer.
        };

        /* === Functions === */

        virtual ~UIAsset();

        //! Returns the asset type.
        virtual Types Type() const = 0;
        //! Returns a bitwise combination of asset flags.
        virtual Flags::DataType Flags() const = 0;

        //! Draws the asset overlay at the specified location.
        virtual void DrawOverlay(const AssetBrowserView* view, const Math::Point2f& position, const Math::Size2f& size);

        //! Instantiates this asset.
        virtual void Instantiate();

        //! Selects this asset.
        void Select();
        //! Deselects this asset.
        void Deselect();

        /* --- Preview and labels --- */

        //! Draws the asset preview into the specified preivew parition.
        void DrawPreview(const Math::Size2i& previewSize = UIAsset::defaultPreviewSize);
        //! Generates the MIP-maps for the layer in the array texture of this asset.
        void GenerateMIPMaps();

        void ChangeLabelConfig(const Video::Font* font, int maxWidth);
        int LabelHeight() const;

        /**
        Returns the asset preview partition data.
        \see DrawPreview
        */
        inline const PreviewPartition& GetPreviewPartition() const
        {
            return previewPartition_;
        }

        //! Returns the label of this asset.
        inline const std::string& GetLabel() const
        {
            return label_;
        }
        //! Returns the list of all lines for the label.
        inline const std::vector<std::string> GetLabelMultiLines() const
        {
            return multiLineLabel_.GetLines();
        }

        /* --- Property grid --- */

        //! Shows the asset attributes in the associated property grid.
        void ShowAttributes();

        //! Builds all categories of the assets for the specified property grid.
        static void BuildPropertyGrid(UIPropertyGrid& propGrid);

        /* === Members === */

        //! Preview position on screen.
        Math::Point2f previewPosition;

    protected:

        UIAsset(const PreviewPartition& previewPartition, const std::string& assetLabel);

        //! Draws the asset preview content.
        virtual void DrawPreviewContent(const Math::Size2i& previewSize) = 0;

        //! Returns the ID string for the attribute category of the specified asset type.
        static wxString AttribCategoryID(const Types type);
        //! Returns the ID string for the attribute category of this asset.
        wxString AttribCategoryID() const;

        //! Changes the asset label.
        void ChangeLabel(const std::string& assetLabel);

        //! Callback when this asset is selected.
        virtual void OnSelect();
        //! Callback when this asset is deselected.
        virtual void OnDeselect();

        /**
        Callback when the attributes will be shown.
        This can be used to update the property grid with all asset attributes.
        */
        virtual void OnShowAttributes(UIPropertyGrid& propGrid, const wxString& namePrefix);

        inline Video::RenderTarget* GetRenderTarget() const
        {
            return renderTarget_.get();
        }

    private:
        
        void BeginPreview(const Math::Size2i& previewSize);
        void EndPreview();

        /* === Members === */

        std::unique_ptr<UITreeView::Entry>  treeViewEntry_;

        PreviewPartition                    previewPartition_;

        //! \todo Use only one single render target for all asset previews!
        Video::RenderTargetPtr              renderTarget_;

        std::string                         label_;
        Utility::MultiLineFontString        multiLineLabel_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================