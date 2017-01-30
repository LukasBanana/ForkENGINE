/*
 * Editor view header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_VIEW_H__
#define __FORK_EDITOR_VIEW_H__


#include "Scene/Node/CameraNode.h"
#include "Scene/Node/LightNode.h"
#include "Scene/Manager/FreeViewSceneNodeController.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/RenderSystem/RendererProfilerModel.h"
#include "IO/InputDevice/Keyboard.h"
#include "Utility/Projector/Projector.h"
#include "Core/FreeLookMouseEventHandler.h"
#include "EditorViewProjector.h"

#include <wx/window.h>


namespace Fork
{

namespace Editor
{


class UIScreenFrame;

class EditorView
{
    
    public:
        
        /* === Enumerations === */

        //! Scene render modes enumeration.
        enum class RenderModes
        {
            Shaded,
            Solid,
            Wireframe,
            BoundingBox,
        };

        /* === Structures === */

        //! This configuration is used for each individual editor view.
        struct Configuration
        {
            bool        showGrid            = true;
            bool        showOrigin          = true;
            bool        showProfiler        = false;
            bool        toggleCameraLight   = true;
            float       moveSpeed           = 0.01f;
            RenderModes renderMode          = RenderModes::Shaded;
        };

        //! Global configuration is used for all editor views.
        struct GlobalConfiguration
        {
            int projectorVisualSize = 100;
        };

        /* === Functions === */

        //! \throws RenderContextException If the render context creation for the screen frame failed.
        EditorView(wxWindow* parent);
        ~EditorView();

        void ProcessUserInteraction();
        void Draw();

        void ChangeBackgroundColor(const Video::ColorRGBAf& color);
        void ChangeProjectorTransition(unsigned long long duration);

        //! Moves the editor view.
        void MoveView(const Math::Vector3f& direction);
        //! Zooms the editor view (only for orthogonal projections).
        void ZoomView(float direction);

        //! Returns true if this editor view is the owner of the specified frame.
        bool IsFrameOwner(const Platform::Frame* frame) const;

        /* === Inline functions === */

        inline Scene::CameraNode* GetCamera()
        {
            return &camera_;
        }
        inline const Scene::CameraNode* GetCamera() const
        {
            return &camera_;
        }

        inline UIScreenFrame* GetScreenFrame() const
        {
            return screenFrame_;
        }

        inline const EditorViewProjector* GetProjector() const
        {
            return projector_.get();
        }

        /* === Members === */

        //! View configuration.
        Configuration config;

        //! Static view configuration (for all views).
        static GlobalConfiguration globalConfig;

    private:
        
        /* === Classes === */

        class KeyboardEventHandler : public IO::Keyboard::EventHandler
        {
            
            public:
                
                KeyboardEventHandler(EditorView* view);

                bool OnKeyDown(const IO::KeyCodes keyCode, const Platform::Frame* frame) override;

            private:
                
                EditorView* view_ = nullptr;

        };

        class MouseEventHandler : public FreeLookMouseEventHandler
        {
            
            public:
                
                MouseEventHandler(EditorView* view);

                bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnWheelMotion(int motion, const Platform::Frame* frame) override;
                bool OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame) override;
                bool OnGlobalMotion(const Math::Vector2i& motion) override;

            private:
                
                enum class FreeLookModes
                {
                    FreeLook,   //!< Turn camera free.
                    MoveXY,     //!< Move camera along the X/Y axes.
                };

                EditorView*     view_       = nullptr;
                Math::Point2i   mousePos_;
                FreeLookModes   lookMode_   = FreeLookModes::FreeLook;

        };

        friend class MouseEventHandler;

        /* === Functions === */

        void CreateScreenFrame(wxWindow* parent);

        void InitCameraSceneNode();

        void CreateVisualProjector();
        void CreateEventHandlers();

        void UpdateProjectionAndView();

        void DrawScene();
        void DrawSelectionHighlight();
        void DrawFrontMenu3D();
        void DrawFrontMenu2D();

        void DrawSelector();
        void DrawProjector();

        void DrawProfilerOutput(const Video::RendererProfilerModel& profilerModel);

        void PickEntity(const Math::Point2i& mousePos);

        //! Returns the current move speed, depending on the view projection type and zoom.
        float ViewMoveSpeed() const;

        /* === Members === */

        UIScreenFrame*                          screenFrame_ = nullptr;

        Scene::CameraNode                       camera_;
        Scene::FreeViewSceneNodeController      cameraCtrl_;
        Scene::LightNode                        cameraLight_;

        std::unique_ptr<EditorViewProjector>    projector_;

        std::shared_ptr<KeyboardEventHandler>   keyboardEventHandler_;
        std::shared_ptr<MouseEventHandler>      mouseEventHandler_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================