/*
 * Editor asset browser header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ASSET_BROWSER_H__
#define __FORK_EDITOR_ASSET_BROWSER_H__


#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"
#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/RenderSystem/RenderState/BlendState.h"
#include "Video/RenderSystem/Texture/TextureManager.h"
#include "Scene/FileHandler/ModelReader.h"
#include "Asset/UIAsset.h"
#include "Asset/ModelPreviewRenderer.h"
#include "AssetPreviewManager.h"
#include "AssetPreviewMask.h"
#include "AssetBrowserModel.h"
#include "AssetBrowserView.h"
#include "../../Core/FreeLookMouseEventHandler.h"

#include <wx/window.h>


namespace Fork
{

namespace Editor
{


DECL_SHR_PTR(AssetBrowser);

class ModelPreviewRenderer;
class UIScreenFrame;

/**
Editor asset browser.
\todo Split this class into a model-view-controller (AssetBrowser, AssetBrowserView, AssetBrowserModel).
*/
class AssetBrowser
{
    
    public:
        
        AssetBrowser(wxWindow* parent);

        /* === Callbacks === */

        void OnDropFiles        (wxDropFilesEvent&  event);
        void OnResizeScreen     (wxSizeEvent&       event);

        void OnScrollLineUp     (wxScrollWinEvent&  event);
        void OnScrollLineDown   (wxScrollWinEvent&  event);
        void OnScrollPageUp     (wxScrollWinEvent&  event);
        void OnScrollPageDown   (wxScrollWinEvent&  event);
        void OnScrollThumbTrack (wxScrollWinEvent&  event);

        /* === Functions === */
        
        void Draw();

        void ChangeBackgroundColor(const Video::ColorRGBAf& color);

        //! Scrolls the asset browser.
        void Scroll(int scrollDelta);
        //! Sets the scroll position.
        void ScrollLocate(int scrollPos);
        //! Returns the current scroll position.
        int ScrollPosition() const;

        //! Scrolls to the top.
        void ScrollTop();
        //! Scrolls to the bottom.
        void ScrollBottom();

        //! Scrolls to the currently selected asset (if it is not visible).
        void ScrollToSelectedAsset();

        /**
        Adds a new asset from the specified file
        \param[in] filename Specifies the filename for the new asset.
        \return Shared pointer to the new asset. This can be TextureAsset,
        ModelAsset, SoundAsset etc. respectively to the file type.
        \see AddTextureAsset
        */
        UIAssetPtr AddAssetFromFile(const std::string& filename);

        //! \see AssetBrowserModel::AddTextureAsset
        UIAssetPtr AddTextureAsset(const Video::TexturePtr& textureResource, const std::string& assetLabel = "");
        //! \see AssetBrowserModel::AddModelAsset
        UIAssetPtr AddModelAsset(const Scene::GeometryPtr& geometryResource, const std::string& assetLabel = "");
        //! \see AssetBrowserModel::AddSoundAsset
        UIAssetPtr AddSoundAsset(const Audio::SoundSourcePtr& soundResource, const std::string& assetLabel = "");

        void UpdateAssetPreviewLocations();

        //! Posts the 'turn selected model asset' event (pitch and yaw are in radians).
        void PostTurnSelectedModelAsset(float pitch, float yaw);

        //! Returns true if the asset browser frame has the focus.
        bool HasFocus() const;

        static AssetBrowser* Instance();

        /* === Inline functions === */

        inline AssetBrowserModel* GetModel()
        {
            return &model_;
        }
        inline const AssetBrowserModel* GetModel() const
        {
            return &model_;
        }

        inline UIScreenFrame* GetScreenFrame() const
        {
            return screenFrame_;
        }

        inline ModelPreviewRenderer* GetModelPreviewRenderer() const
        {
            return modelPreviewRenderer_.get();
        }

    private:
        
        /* === Classes === */

        class KeyboardEventHandler : public IO::Keyboard::EventHandler
        {
            
            public:
                
                bool OnKeyDown(const IO::KeyCodes keyCode, const Platform::Frame* frame) override;

        };

        class MouseEventHandler : public FreeLookMouseEventHandler
        {
            
            public:
                
                bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnWheelMotion(int motion, const Platform::Frame* frame) override;
                bool OnGlobalMotion(const Math::Vector2i& motion) override;

        };

        class TextureEventHandler : public Video::TextureManager::EventHandler
        {
            
            public:
                
                void OnTexture2DCreated(const Video::Texture2DPtr& texture) override;

        };

        struct TurnModelAssetEvent
        {
            bool    isActive    = false;
            float   pitch       = 0.0f;
            float   yaw         = 0.0f;
        };

        /* === Functions === */

        void CreateScreenFrame(wxWindow* parent);
        void CreateEventHandlers();
        void CreateModelPreviewRenderer();

        void AddDefaultAssets();

        void BindScreenFrameEvent();

        Math::Point2f ImageEntryOffsetPosition() const;

        //! Enables or disables event handler listening.
        void ListenToTextureManager(bool isEnabled);

        //! Returns the flags for geometry reading.
        Scene::ModelReader::Flags::DataType GeometryReadingFlags() const;

        /* === Members === */

        AssetBrowserModel                       model_;
        AssetBrowserView                        view_;

        UIScreenFrame*                          screenFrame_ = nullptr;

        std::shared_ptr<KeyboardEventHandler>   keyboardEventHandler_;
        std::shared_ptr<MouseEventHandler>      mouseEventHandler_;
        std::shared_ptr<TextureEventHandler>    textureEventHandler_;

        std::unique_ptr<ModelPreviewRenderer>   modelPreviewRenderer_;

        TurnModelAssetEvent                     eventTurnModelAsset_;

        static AssetBrowser*                    instance_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================