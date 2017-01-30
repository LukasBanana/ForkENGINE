/*
 * Projector header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_PROJECTOR_H__
#define __FORK_UTILITY_PROJECTOR_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Utility/Projector/ProjectorModel.h"
#include "Utility/Projector/ProjectorView.h"
#include "Video/Font/TextureFont.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "IO/InputDevice/Mouse.h"


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(Projector);

/**
Projector controller class. The projector can be used to visualize the current camera projection.
It provides a cube with 6 cones (one for each cube side).
It is designed with the model-view-controller pattern, where "ProjectorModel"
contains the model data, and "ProjectorView" provides functions to draw the projector.
Both ProjectorModel and ProjectorView can be extended.
\remarks Here is an example picture with the two kinds of projection:
\image html images/Projection.Example.png
\see ProjectorModel
\see ProjectorView
*/
class FORK_EXPORT Projector
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
        Projector(
            const Video::PrimitiveRendererPtr& renderer,
            const Video::TextureFontPtr& labelFont = nullptr,
            const ProjectorModelPtr& model = nullptr,
            const ProjectorViewPtr& view = nullptr
        );
        virtual ~Projector();

        /**
        Draws the 3D projector.
        \see ProjectorView::Draw
        */
        virtual void Draw(const ProjectorView::DrawFlags::DataType flags = ProjectorView::DrawFlags::Default);

        /**
        Draws the 3D projector and updates the projection transition (if active).
        \see ProjectorModel::UpdateTransition
        \see ProjectorView::Draw
        */
        virtual void DrawAndUpdate(const ProjectorView::DrawFlags::DataType flags = ProjectorView::DrawFlags::Default);

        //! Returns a raw-pointer to the projector model.
        inline ProjectorModel* GetModel() const
        {
            return model_.get();
        }
        //! Returns a raw-pointer to the projector view.
        inline ProjectorView* GetView() const
        {
            return view_.get();
        }

        //! Returns the primitive renderer which is used to draw the projector.
        inline Video::PrimitiveRenderer* GetRenderer() const
        {
            return renderer_.get();
        }

    protected:

        /* === Classes === */

        class FORK_EXPORT MouseEventHandler : public IO::Mouse::EventHandler
        {
            
            public:
                
                //! \throws NullPointerException If 'projector' is null.
                MouseEventHandler(Projector* projector);

                bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override;
                bool OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame) override;

            protected:
                
                inline Projector* GetProjector() const
                {
                    return projector_;
                }

            private:
                
                Projector*      projector_ = nullptr;
                Math::Vector2f  mousePos_;

        };

        /* === Functions === */

        /**
        Exchanges the mouse event handler.
        \param[in] eventHandler Specifies the new mouse event handler.
        If this is null, the function has no effect.
        \see MouseEventHandler
        */
        void ExchangeMouseEventHandler(const std::shared_ptr<MouseEventHandler>& eventHandler);

    private:

        friend class MouseEventHandler;

        /* === Members === */

        Video::PrimitiveRendererPtr renderer_;

        ProjectorModelPtr model_;
        ProjectorViewPtr view_;

        Video::TextureFontPtr labelFont_;

        std::shared_ptr<MouseEventHandler> mouseEventHandler_;

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================