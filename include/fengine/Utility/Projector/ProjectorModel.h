/*
 * Projector model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_PROJECTOR_MODEL_H__
#define __FORK_UTILITY_PROJECTOR_MODEL_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/Core/Projection.h"
#include "Math/Core/Transform3D.h"
#include "Math/Core/Cuboid.h"
#include "Math/Geometry/Cone.h"
#include "Math/Common/AxisDirections.h"
#include "Scene/Node/DynamicSceneNode.h"
#include "Scene/Node/CameraNode.h"

#include <chrono>
#include <array>


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(ProjectorModel);

//! Projector model class.
class FORK_EXPORT ProjectorModel
{

    public:
        
        /* === Enumerations === */

        //! Selection states.
        enum class SelectionStates
        {
            None,       //!< Nothing is selected.
            Projector,  //!< Projector's cube is selected. Clicking on the cube will switch between orthogonal and perspective projection.
            XPositive,  //!< X+ axis is selected.
            XNegative,  //!< X- axis is selected.
            YPositive,  //!< Y+ axis is selected.
            YNegative,  //!< Y- axis is selected.
            ZPositive,  //!< Z+ axis is selected.
            ZNegative,  //!< Z- axis is selected.
        };

        /* === Classes === */

        struct FORK_EXPORT TriggerState
        {
            TriggerState() = default;
            TriggerState(int)
            {
                /* Dummy */
            }

            //! Returns true if the visibility is greater than 0.5.
            inline bool IsVisible() const
            {
                return visibility > 0.5f;
            }

            //! Specifies whether this trigger is selected (and highlighted) or not. By default false.
            bool    isSelected  = false;

            /**
            Specifies the visibility factor of this trigger.
            This is in the range [0.0 .. 1.0]. By default 1.0.
            \see ProjectorModel::UpdateTriggerVisibility
            */
            float   visibility  = 1.0f;
        };

        /* === Functions === */

        ProjectorModel();
        virtual ~ProjectorModel();

        /**
        Restes the selection states. Will also be called by "ProcessSelection".
        \see ProcessSelection
        */
        void ResetSelection();

        /**
        Returns true if any item of the projector is selected,
        i.e. if either the box or any of the six cones are selected.
        \see ProcessSelection
        \see ResetSelection
        */
        bool IsAnySelected() const;

        /**
        Processes the selection.
        \param[in] pointerPosition Specifies the pointer (or cursor or mouse) position.
        This can be used as the following example shows:
        \code
        // This works only in fullscreen mode, because "Platform::Cursor::GetPosition"
        // returns the global cursor position (and not relative to the graphics frame).
        projectorModel->ProcessSelection(Platform::Cursor::GetPosition().Cast<float>(), activate);
        \endcode
        \param[in] activate Specifies whether the selected axis is to be activated or not.
        \return True if any item is of the projector is selected,
        i.e. if either the box or any of the six cones are selected.
        \see IsAnySelected
        \see ProcessActivation
        */
        virtual bool ProcessSelection(const Math::Point2f& pointerPosition);
        /**
        Processes the activation the currently selected item.
        \remarks This should be called when a mouse button is pressed.
        \see ProcessSelection
        */
        virtual void ProcessActivation(const Math::Point2f& pointerPosition);

        /**
        Updates the internal animation transitions and trigger visibilities.
        \see UpdateProjectionTransition
        \see UpdateTriggerVisibility
        */
        virtual void Update();

        /**
        Cancels the transition (if active) and sets the projection to the transition end state.
        \see SetupProjection
        */
        virtual void CancelTransition();

        /**
        Sets the new projection with the specified transition.
        \param[in] projection Specifies the new projection.
        \param[in] transitionDuration Specifies the transition duration (in milliseconds).
        By default 0 (No transition).
        */
        virtual void SetupProjection(
            const Video::Projectionf& projection, unsigned long long transitionDuration = 0
        );

        /**
        Sets the viewport and the viewport for the visual appearance of this projector model.
        \note This is actually part of the "Video::Projection" class. But in this case,
        it is separated to this projector model class. That's due to the possibility of
        projection transitions.
        */
        virtual void SetupViewport(const Video::Viewport& viewport, const Video::Viewport& visualViewport);

        /**
        Sets the viewport and the viewport for the visual appearance of this projector model.
        In this case, the visual viewport will be set into the right-top border of the screen
        with the specified size.
        \see SetupViewport
        */
        void SetupViewportDefaultVisual(
            Video::Viewport viewport, int visualSize = 100,
            float nearPlaneVisual = 0.0f, float farPlaneVisual = 0.1f,
            float nearPlaneMain = 0.1f, float farPlaneMain = 1.0f
        );

        /**
        Sets the new orientation with the specified transition.
        \param[in] orientation Specifies the new orientation.
        \param[in] transitionDuration Specifies the transition duration (in milliseconds).
        By default 0 (No transition).
        \see orientationSource
        */
        virtual void SetupOrientation(
            const Math::Quaternionf& orientation,
            unsigned long long transitionDuration = 0
        );

        /**
        Returns the view matrix for this model
        \see viewDistance
        */
        virtual Math::Matrix4f ViewMatrix() const;

        /**
        Returns the current view orientation of the projector.
        This is the inverse rotation of the orientation source.
        \see orientationSource
        */
        Math::Quaternionf Orientation() const;

        /* === Inline functions === */

        //! Returns the current projection.
        inline const Video::Projectionf& GetProjection() const
        {
            return projection_;
        }
        //! Returns the current visual projection (for the 3D projector model).
        inline const Video::Projectionf& GetVisualProjection() const
        {
            return visualProjection_;
        }

        //! Returns the viewport for the visual appearance of this projector model.
        inline const Video::Viewport& GetViewport() const
        {
            return viewport_;
        }

        //! Returns the current selection state.
        inline SelectionStates GetSelectionState() const
        {
            return selectionState_;
        }

        //! Returns the main trigger state.
        inline const TriggerState& GetMainTriggerState() const
        {
            return mainTriggerState_;
        }
        //! Returns the current trigger states.
        inline const Math::Cuboid<TriggerState>& GetTriggerStates() const
        {
            return triggerStates_;
        }

        //! Distance between the origin and the projector model (in Z+ direction). By default 2.0.
        float viewDistance = 2.0f;

        //! Raw pointer to the orientation transform source. Only the rotation will be read or written).
        Scene::Transform* orientationSource = nullptr;

        //! Default projection transition duration (in milliseconds). By default 250.
        unsigned long long defaultTransitionDuration = 250;

    protected:

        typedef std::chrono::system_clock::time_point TimeType;

        /* === Structures === */

        struct FORK_EXPORT ProjectionTransition
        {
            void Start(
                unsigned long long transitionDuration,
                const Video::Projectionf& from, const Video::Projectionf& to,
                const Video::Projectionf& fromVisual, const Video::Projectionf& toVisual
            );
            void Interpolate(
                Video::Projectionf& projection, Video::Projectionf& visualProjection
            );
            void Stop(
                Video::Projectionf& projection, Video::Projectionf& visualProjection
            );

            Video::Projectionf  startProjection;
            Video::Projectionf  endProjection;
            
            Video::Projectionf  startVisualProjection;
            Video::Projectionf  endVisualProjection;
            
            bool                isActive        = false;
            unsigned long long  duration        = 0;

            TimeType            startTime;
        };

        struct FORK_EXPORT OrientationTransition
        {
            void Start(
                unsigned long long transitionDuration,
                const Math::Quaternionf& from, const Math::Quaternionf& to
            );
            void Interpolate(Math::Quaternionf& orientation);
            void Stop(Math::Quaternionf& orientation);

            Math::Quaternionf   startOrientation;
            Math::Quaternionf   endOrientation;

            bool                isActive        = false;
            unsigned long long  duration        = 0;

            TimeType            startTime;
        };

        /* === Functions === */

        void UpdateProjectionTransition();
        void UpdateOrientationTransition();
        void UpdateTriggerVisibility();

        /* === Members === */

        ProjectionTransition    projectionTransition;
        OrientationTransition   orientationTransition;

    private:
        
        /* === Functions === */

        void BuildTriggerCones();

        void AdjustVisualProjection(Video::Projectionf& visualProjection);

        /* === Members === */

        Video::Projectionf          projection_;
        Video::Projectionf          visualProjection_;

        Video::Viewport             viewport_;                                  //!< This viewport is separated from the projection objects.

        SelectionStates             selectionState_ = SelectionStates::None;    //!< Current selection state.

        TriggerState                mainTriggerState_;                          //!< State of the main trigger.
        Math::Cuboid<TriggerState>  triggerStates_;                             //!< State of all six sub triggers.

        std::array<Math::Cone<>, 6> triggerCones_;                              //!< Trigger cone models.

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================