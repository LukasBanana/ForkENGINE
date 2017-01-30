/*
 * Editor asset browser file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AssetBrowser.h"
#include "Engine/FileHandler/AssetFileEvaluator.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Shader/StandardShader/ImageArrayDrawingConstantBuffers.h"
#include "../../Core/UIExtensions/UIScreenFrame.h"
#include "../../Core/UIExtensions/UIProgressBar.h"
#include "../../Core/UIExtensions/ScopedProgressBarState.h"
#include "../../Core/Devices.h"
#include "../../Core/WxHelper.h"
#include "../../Core/Paths.h"
#include "IO/Core/Log.h"
#include "../../Frame/Main/MainFrame.h"
#include "../../Frame/Main/MainFrameStatusBar.h"

#include "Asset/TextureUIAsset.h"
#include "Asset/ModelUIAsset.h"
#include "Asset/SoundUIAsset.h"

#include <wx/file.h>
#include <wx/dir.h>


namespace Fork
{

namespace Editor
{


using namespace Devices;

static const int scrollSize = 30;

AssetBrowser* AssetBrowser::instance_ = nullptr;

AssetBrowser::AssetBrowser(wxWindow* parent) :
    view_{ GetModel() }
{
    AssetBrowser::instance_ = this;

    CreateScreenFrame(parent);

    BindScreenFrameEvent();

    /* Setup initial background color */
    ChangeBackgroundColor(defaultClearColor);

    /* Create resources */
    CreateEventHandlers();
    CreateModelPreviewRenderer();

    /* Add defautl model assets (cube, cone etc.) */
    AddDefaultAssets();

    #if 0//!!!
    GetModel()->previewSize = { 128, 128 };
    #endif
}

void AssetBrowser::OnDropFiles(wxDropFilesEvent& event)
{
    /* Store all droped filenames */
    wxString* dropedFiles = event.GetFiles();

    wxArrayString filenames;

    for (int i = 0; i < event.GetNumberOfFiles(); ++i)
    {
        auto name = dropedFiles[i];

        if (wxFileExists(name))
            filenames.push_back(name);
        else if (wxDirExists(name))
            wxDir::GetAllFiles(name, &filenames);
    }

    if (!filenames.empty())
    {
        /* Process all stored filenames */
        auto progress = MainFrame::Instance()->GetMainStatusBar()->ScopedProgress(filenames.size());

        for (auto file : filenames)
        {
            AddAssetFromFile(file.ToStdString());
            progress.NextStep();
        }
    }
}

void AssetBrowser::OnResizeScreen(wxSizeEvent& event)
{
    screenFrame_->OnResize(event);
    UpdateAssetPreviewLocations();
}

void AssetBrowser::OnScrollLineUp(wxScrollWinEvent& event)
{
    Scroll(-scrollSize);
}

void AssetBrowser::OnScrollLineDown(wxScrollWinEvent& event)
{
    Scroll(scrollSize);
}

void AssetBrowser::OnScrollPageUp(wxScrollWinEvent& event)
{
    Scroll(-screenFrame_->GetClientSize().GetHeight());
}

void AssetBrowser::OnScrollPageDown(wxScrollWinEvent& event)
{
    Scroll(screenFrame_->GetClientSize().GetHeight());
}

void AssetBrowser::OnScrollThumbTrack(wxScrollWinEvent& event)
{
    ScrollLocate(event.GetPosition());
    Draw();
}

void AssetBrowser::ScrollTop()
{
    ScrollLocate(0);
}

void AssetBrowser::ScrollBottom()
{
    ScrollLocate(screenFrame_->GetScrollRange(wxVERTICAL));
}

void AssetBrowser::ScrollToSelectedAsset()
{
    static const int border = 10;

    auto selectedAsset = GetModel()->LastSelectedAsset();
    if (selectedAsset)
    {
        /* Get asset and frame locations */
        auto posY           = static_cast<int>(selectedAsset->previewPosition.y);
        auto scrollPos      = ScrollPosition();
        auto previewSize    = static_cast<int>(GetModel()->previewSize.height);
        auto frameHeight    = GetScreenFrame()->GetVirtualSize().GetHeight();

        /* Check if asset position exceeded browser frame's view */
        if (scrollPos > posY - border)
            ScrollLocate(posY - border);
        else if (scrollPos < posY + border + previewSize - frameHeight)
            ScrollLocate(posY - frameHeight + previewSize + border);
    }
}

void AssetBrowser::Draw()
{
    screenFrame_->GetRenderContext()->Activate();

    /* Check if asset preivew must be updated */
    if (eventTurnModelAsset_.isActive)
    {
        GetModel()->TurnSelectedModelAsset(eventTurnModelAsset_.pitch, eventTurnModelAsset_.yaw);
        eventTurnModelAsset_ = TurnModelAssetEvent();
    }

    /* Draw asset browser view */
    view_.Draw(
        screenFrame_->GetRenderContext(),
        screenFrame_->FullViewport(),
        ImageEntryOffsetPosition(),
        screenFrame_->HasFocus()
    );
}

void AssetBrowser::ChangeBackgroundColor(const Video::ColorRGBAf& color)
{
    screenFrame_->GetRenderContext()->SetupClearColor(color);
}

void AssetBrowser::Scroll(int scrollDelta)
{
    ScrollLocate(ScrollPosition() + scrollDelta);
}

void AssetBrowser::ScrollLocate(int scrollPos)
{
    screenFrame_->SetScrollPos(wxVERTICAL, scrollPos);
}

int AssetBrowser::ScrollPosition() const
{
    return screenFrame_->GetScrollPos(wxVERTICAL);
}

UIAssetPtr AssetBrowser::AddAssetFromFile(const std::string& filename)
{
    /* Determine asset type from file */
    const auto assetType = Engine::AssetFileEvaluator::EvaluateFile(filename);

    switch (assetType)
    {
        case Engine::Asset::Types::Texture:
        {
            auto textureResource = renderSystem->GetTextureManager()->LoadTexture2D(filename);
            return AddTextureAsset(textureResource, ExtractFileName(filename));
        }
        break;
        
        case Engine::Asset::Types::Geometry:
        {
            ListenToTextureManager(true);
            auto geometryResource = sceneManager->LoadGeometry(filename, GeometryReadingFlags());
            ListenToTextureManager(false);
            return AddModelAsset(geometryResource, ExtractFileName(filename));
        }
        break;

        case Engine::Asset::Types::Sound:
        {
            auto soundResource = soundSystem->LoadBufferSound(filename);
            return AddSoundAsset(soundResource, ExtractFileName(filename));
        }
        break;

        default:
        {
            IO::Log::Error("Unknown asset type of file \"" + filename + "\"");
        }
        break;
    }

    return nullptr;
}

UIAssetPtr AssetBrowser::AddTextureAsset(const Video::TexturePtr& textureResource, const std::string& assetLabel)
{
    auto asset = model_.AddTextureAsset(textureResource, assetLabel);
    UpdateAssetPreviewLocations();
    return asset;
}

UIAssetPtr AssetBrowser::AddModelAsset(const Scene::GeometryPtr& geometryResource, const std::string& assetLabel)
{
    auto asset = model_.AddModelAsset(geometryResource, assetLabel);
    UpdateAssetPreviewLocations();
    return asset;
}

UIAssetPtr AssetBrowser::AddSoundAsset(const Audio::SoundSourcePtr& soundResource, const std::string& assetLabel)
{
    auto asset = model_.AddSoundAsset(soundResource, assetLabel);
    UpdateAssetPreviewLocations();
    return asset;
}

void AssetBrowser::UpdateAssetPreviewLocations()
{
    const auto clientSize = screenFrame_->GetClientSize();

    /* Update preview locations of the browser model */
    Math::Point2f lowestPosition;

    model_.UpdateAssetPreviewLocations(
        Math::Size2i(clientSize.GetWidth(), clientSize.GetHeight()).Cast<float>(),
        lowestPosition,
        view_.GetLabelFont()
    );

    /* Update screen frame scroll bar */
    auto thumbSize = screenFrame_->GetClientSize().GetHeight();
    auto scrollArea = static_cast<int>(lowestPosition.y);

    screenFrame_->SetScrollbar(
        wxVERTICAL, screenFrame_->GetScrollPos(wxVERTICAL),
        thumbSize, scrollArea
    );
}

void AssetBrowser::PostTurnSelectedModelAsset(float pitch, float yaw)
{
    eventTurnModelAsset_.isActive = true;
    eventTurnModelAsset_.pitch += pitch;
    eventTurnModelAsset_.yaw += yaw;
}

bool AssetBrowser::HasFocus() const
{
    return GetScreenFrame()->HasFocus();
}

AssetBrowser* AssetBrowser::Instance()
{
    return instance_;
}


/*
 * ======= Private: =======
 */

void AssetBrowser::CreateScreenFrame(wxWindow* parent)
{
    /* Create screen frame and render context */
    screenFrame_ = MakeWxObject<UIScreenFrame>(
        parent, wxDefaultPosition, wxSize(400, 100)
    );

    if (!UIScreenFrame::CreateRenderContext(screenFrame_))
    {
        screenFrame_->Destroy();
        throw RenderContextException("Render context creation for screen frame failed");
    }

    screenFrame_->SetResizeCallback([&](const Math::Size2i&){ Draw(); });
}

void AssetBrowser::CreateEventHandlers()
{
    keyboardEventHandler_ = std::make_shared<KeyboardEventHandler>();
    IO::Keyboard::Instance()->AddEventHandler(keyboardEventHandler_);

    mouseEventHandler_ = std::make_shared<MouseEventHandler>();
    IO::Mouse::Instance()->AddEventHandler(mouseEventHandler_);

    textureEventHandler_ = std::make_shared<TextureEventHandler>();
}

void AssetBrowser::CreateModelPreviewRenderer()
{
    try
    {
        modelPreviewRenderer_ = std::make_unique<ModelPreviewRenderer>();
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
        IO::Log::Warning("ModelPreviewRenderer is not available");
    }
}

void AssetBrowser::AddDefaultAssets()
{
    AddModelAsset( sceneManager->GenerateCube(),        "Cube"       );
    AddModelAsset( sceneManager->GenerateCylinder(),    "Cylinder"   );
    AddModelAsset( sceneManager->GenerateCone(),        "Cone"       );
    AddModelAsset( sceneManager->GenerateUVSphere(),    "UV-Sphere"  );
    AddModelAsset( sceneManager->GenerateIcoSphere(),   "Ico-Sphere" );
}

void AssetBrowser::BindScreenFrameEvent()
{
    /* Bind callbacks */
    screenFrame_->DragAcceptFiles(true);

    screenFrame_->Bind( wxEVT_DROP_FILES,           &AssetBrowser::OnDropFiles,        this );
    screenFrame_->Bind( wxEVT_SIZE,                 &AssetBrowser::OnResizeScreen,     this );

    screenFrame_->Bind( wxEVT_SCROLLWIN_LINEUP,     &AssetBrowser::OnScrollLineUp,     this );
    screenFrame_->Bind( wxEVT_SCROLLWIN_LINEDOWN,   &AssetBrowser::OnScrollLineDown,   this );
    screenFrame_->Bind( wxEVT_SCROLLWIN_PAGEUP,     &AssetBrowser::OnScrollPageUp,     this );
    screenFrame_->Bind( wxEVT_SCROLLWIN_PAGEDOWN,   &AssetBrowser::OnScrollPageDown,   this );
    screenFrame_->Bind( wxEVT_SCROLLWIN_THUMBTRACK, &AssetBrowser::OnScrollThumbTrack, this );
}

Math::Point2f AssetBrowser::ImageEntryOffsetPosition() const
{
    return { 0, -static_cast<float>(ScrollPosition()) };
}

void AssetBrowser::ListenToTextureManager(bool isEnabled)
{
    auto textureMngr = Video::RenderSystem::Active()->GetTextureManager();
    if (isEnabled)
        textureMngr->AddEventHandler(textureEventHandler_);
    else
        textureMngr->RemoveEventHandler(textureEventHandler_);
}

Scene::ModelReader::Flags::DataType AssetBrowser::GeometryReadingFlags() const
{
    typedef Scene::ModelReader::Flags Flags;
    return Flags::OptimizeGeometryGraph | Flags::GenerateTangentSpace;
    //return Flags::OptimizeGeometryGraph;
}


/*
 * KeyboardEventHandler class
 */

bool AssetBrowser::KeyboardEventHandler::OnKeyDown(const IO::KeyCodes keyCode, const Platform::Frame* frame)
{
    auto browser = AssetBrowser::Instance();

    if (!browser->HasFocus())
        return true;

    auto model = browser->GetModel();

    switch (keyCode)
    {
        case IO::KeyCodes::KeyHome:
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
            {
                browser->ScrollTop();
                model->SelectColumnBegin();
            }
            else
                model->SelectRowBegin();
            break;

        case IO::KeyCodes::KeyEnd:
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
            {
                browser->ScrollBottom();
                model->SelectColumnEnd();
            }
            else
                model->SelectRowEnd();
            break;

        case IO::KeyCodes::KeyA://KeyLeft:
            model->SelectLeft();
            browser->ScrollToSelectedAsset();
            break;
        case IO::KeyCodes::KeyD://Right:
            model->SelectRight();
            browser->ScrollToSelectedAsset();
            break;
        case IO::KeyCodes::KeyW://Up:
            model->SelectUp();
            browser->ScrollToSelectedAsset();
            break;
        case IO::KeyCodes::KeyS://Down:
            model->SelectDown();
            browser->ScrollToSelectedAsset();
            break;

        case IO::KeyCodes::KeyF1:
        {
            auto modelAsset = dynamic_cast<ModelUIAsset*>(model->LastSelectedAsset());
            if (modelAsset)
                modelAsset->ShowGeometryHierarchy();
        }
        break;
    }

    return true;
}


/*
 * MouseEventHandler class
 */

bool AssetBrowser::MouseEventHandler::OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{   
    auto browser = AssetBrowser::Instance();
    auto model = browser->GetModel();

    if (!browser->HasFocus())
        return true;

    switch (keyCode)
    {
        case IO::MouseKeyCodes::MouseLeft:
            model->ProcessSelection(
                IO::Mouse::Instance()->GetFrameMousePosition().position.Cast<float>(),
                browser->ImageEntryOffsetPosition()
            );
            browser->ScrollToSelectedAsset();
            break;

        case IO::MouseKeyCodes::MouseRight:
            for (auto selectedAsset : model->GetSelectedAssets())
            {
                if ((selectedAsset->Flags() & UIAsset::Flags::HasRealtimePreview) != 0)
                {
                    LockFreeLook();
                    break;
                }
            }
            #if 1//!!!
            {
                auto sndAsset = dynamic_cast<SoundUIAsset*>(model->LastSelectedAsset());
                if (sndAsset)
                {
                    auto res = sndAsset->GetSoundResource();
                    if (res->IsPlaying())
                        res->Stop();
                    else
                        res->Play();
                }
            }
            #endif
            break;
    }

    return true;
}

bool AssetBrowser::MouseEventHandler::OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    if (IsFreeLook() && keyCode == IO::MouseKeyCodes::MouseRight)
        UnlockFreeLook();
    return true;
}

bool AssetBrowser::MouseEventHandler::OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    auto browser = AssetBrowser::Instance();
    auto model = browser->GetModel();

    if (!browser->HasFocus())
        return true;

    switch (keyCode)
    {
        case IO::MouseKeyCodes::MouseLeft:
        {
            for (auto asset : model->GetSelectedAssets())
                asset->Instantiate();
        }
        break;

        case IO::MouseKeyCodes::MouseRight:
            return OnButtonDown(keyCode, frame);
    }

    return true;
}

bool AssetBrowser::MouseEventHandler::OnWheelMotion(int motion, const Platform::Frame* frame)
{
    auto browser = AssetBrowser::Instance();

    if (browser->HasFocus())
    {
        int scrollMotion = -scrollSize * motion;

        if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
            scrollMotion *= 5;

        browser->Scroll(scrollMotion);
    }

    return true;
}

bool AssetBrowser::MouseEventHandler::OnGlobalMotion(const Math::Vector2i& motion)
{
    static const float rotateFactor = 0.5f;

    auto browser = AssetBrowser::Instance();

    if (!browser->HasFocus())
        return true;

    if (IsFreeLook())
    {
        float pitch = static_cast<float>(motion.y) * -rotateFactor;
        float yaw   = static_cast<float>(motion.x) * -rotateFactor;

        browser->PostTurnSelectedModelAsset(pitch*Math::deg2rad, yaw*Math::deg2rad);
    }

    return true;
}


/*
 * TextureEventHandler class
 */

void AssetBrowser::TextureEventHandler::OnTexture2DCreated(const Video::Texture2DPtr& texture)
{
    AssetBrowser::Instance()->AddTextureAsset(texture, ExtractFileName(texture->metaData.name));
}


} // /namespace Editor

} // /namespace Fork



// ========================