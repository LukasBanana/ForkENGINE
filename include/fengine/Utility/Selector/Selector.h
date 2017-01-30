/*
 * Selector header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_SELECTOR_H__
#define __FORK_UTILITY_SELECTOR_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Utility/Selector/SelectorModel.h"
#include "Utility/Selector/SelectorView.h"
#include "Video/Font/TextureFont.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "IO/InputDevice/Mouse.h"


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(Selector);

/**
Selector controller class. The selector can be used to as scene node selection tool.
It provides a 3D coordinate cross with arrows to move, rotation or scale scene nodes,
or rather general 3D transformations.
It is designed with the model-view-controller pattern, where "SelectorModel"
contains the model data, and "SelectorView" provides functions to draw the selector.
Both SelectorModel and SelectorView can be extended.
\see SelectorModel
\see SelectorView
*/
class FORK_EXPORT Selector
{

    public:
        
        /* === Functions === */

        /**
        Selector constructor.
        \param[in] renderer Shared pointer to a primitive renderer.
        \param[in] model Shared pointer to a custom terminal model. If this is null, the default terminal model will be created.
        \param[in] view Shared pointer to a custom terminal view. If this is null, the default terminal view will be created.
        \throws NullPointerException If the renderer is a null pointer.
        \see Video::PrimitiveRenderer
        */
        Selector(
            const Video::PrimitiveRendererPtr& renderer,
            Scene::SceneManager* sceneManager,
            const Video::TextureFontPtr& labelFont = nullptr,
            const SelectorModelPtr& model = nullptr,
            const SelectorViewPtr& view = nullptr
        );
        virtual ~Selector();

        /**
        Sets the view alignment.
        \param[in] view Specifies the camera view node from which the selector will be aligned.
        */
        virtual void SetupView(const Scene::CameraNode& view);

        /**
        Draws the 3D selector.
        \see SelectorView::Draw
        */
        virtual void Draw(const SelectorView::DrawFlags::DataType flags = SelectorView::DrawFlags::Default);

        /**
        Processes the scene node transformation with absolute pointer position.
        \param[in] pointerPosition Specifies the pointer position.
        */
        virtual void ProcessAbsoluteTransform(const Math::Point2f& pointerPosition);
        /**
        Processes the scene node transformation with relative pointer position (or rather pointer motion).
        \param[in] pointerMotion Specifies the pointer motion.
        */
        virtual void ProcessRelativeTransform(const Math::Vector2f& pointerMotion);

        //! Returns a raw-pointer to the selector model.
        inline SelectorModel* GetModel()
        {
            return model_.get();
        }
        //! Returns a constant raw-pointer to the selector model.
        inline const SelectorModel* GetModel() const
        {
            return model_.get();
        }

        //! Returns a raw-pointer to the selector view.
        inline SelectorView* GetView()
        {
            return view_.get();
        }
        //! Returns a constant raw-pointer to the selector view.
        inline const SelectorView* GetView() const
        {
            return view_.get();
        }

        //! Returns the primitive renderer which is used to draw the selector.
        inline Video::PrimitiveRenderer* GetRenderer() const
        {
            return renderer_.get();
        }

    protected:
        
        /* === Classes === */

        //! Mouse event handler for scene node selection.
        class FORK_EXPORT MouseEventHandler : public IO::Mouse::EventHandler
        {
            
            public:
                
                //! \throws NullPointerException If 'selector' is null.
                MouseEventHandler(Selector* selector);

                bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;

                bool OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame) override;

                bool OnGlobalMotion(const Math::Vector2i& motion) override;

            protected:
                
                inline Selector* GetSelector() const
                {
                    return selector_;
                }

            private:
                
                void OnLostFocus();

                Selector* selector_ = nullptr;
                Math::Vector2f mousePos_;

        };

        /* === Functions === */

        /**
        Exchanges the mouse event handler.
        \param[in] eventHandler Specifies the new mouse event handler.
        If this is null, the function has no effect.
        \see MouseEventHandler
        */
        void ExchangeMouseEventHandler(const std::shared_ptr<MouseEventHandler>& eventHandler);

        /**
        Converts the specified selection mode into a 3D vector.
        \param[in] mode Specifies the selection mode.
        \return { 1, 0, 0 } if mode is "SelectorModel::SelectionModes::XAxis",
        { 0, 1, 0 } if mode is "SelectorModel::SelectionModes::XAxis",
        { 0, 0, 1 } if mode is "SelectorModel::SelectionModes::ZAxis" or
        { 0, 0, 0 } otherwise.
        */
        virtual Math::Vector3f ConvertAxisToVector(const SelectorModel::SelectionStates mode) const;
        //! Converst the specified selection mode and angle into a rotation quaternion.
        virtual Math::Quaternionf ConvertAxisToQuaternion(const SelectorModel::SelectionStates mode, float angle) const;

        /**
        Computes the arrow motion speed depending on the currently
        selected arrow direction and specified motion direction.
        */
        virtual float ArrowMotionSpeedByDirection(const Math::Vector2f& motion, float maxMotionSpeed = 0.05f) const;
        /**
        Computes the arrow motion speed depending on the currently
        selected arrow direction and specified motion direction.
        This is the inverse variant which is used for rotation.
        */
        virtual float ArrowMotionSpeedByInvDirection(const Math::Vector2f& motion, float maxMotionSpeed = 0.05f) const;
        /**
        Returns the arrow motion speed multiplied by the current distance factor.
        This is used to translate and scale objects faster, when they appear smaller on the screen.
        */
        virtual float ArrowMotionSpeedByDistance() const;

        virtual void ProcessTransformAxis(const Math::Vector2f& motion);
        virtual void ProcessTransformAxisPicked(const Math::Point2f& position);
        virtual void ProcessTransformUniform(const Math::Vector2f& motion);
        
        virtual void TranslateSelectedNodes(const Math::Vector3f& translation);
        virtual void TranslateSelectedNodesDirectly(const Math::Vector3f& translation);
        virtual void RotateSelectedNodes(const Math::Quaternionf& rotation);
        virtual void ScaleSelectedNodes(const Math::Vector3f& scale);

    private:

        friend class MouseEventHandler;

        /* === Functions === */

        void OnTransformBegin();
        void OnTransformEnd();

        /* === Members === */

        Video::PrimitiveRendererPtr renderer_;

        SelectorModelPtr model_;
        SelectorViewPtr view_;

        Video::TextureFontPtr labelFont_;

        std::shared_ptr<MouseEventHandler> mouseEventHandler_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================