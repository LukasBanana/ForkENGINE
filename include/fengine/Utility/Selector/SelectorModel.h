/*
 * Selector model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_UTILITY_SELECTOR_MODEL_H__
#define __FORK_UTILITY_SELECTOR_MODEL_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/Core/Projection.h"
#include "Math/Core/Transform3D.h"
#include "Math/Common/RasterNumber.h"
#include "Scene/Node/DynamicSceneNode.h"
#include "Scene/Node/CameraNode.h"
#include "Utility/Selector/Selectable.h"


namespace Fork
{

namespace Utility
{


DECL_SHR_PTR(SelectorModel);

//! Selector model class.
class FORK_EXPORT SelectorModel
{

    public:
        
        /* === Types === */

        //! List type for selectable nodes.
        typedef std::vector<Selectable*> NodeListType;
        
        /**
        3D raster vector.
        \see Math::RasterNumber
        */
        typedef Math::Vector3<Math::RasterNumber<float>> RasterVector;

        /* === Enumerations === */

        //! Transformation modes.
        enum class TransformModes
        {
            Translate,  //!< Translate selected objects.
            Rotate,     //!< Rotate selected objects.
            Scale,      //!< Scale selected objects.
        };

        //! Pivot center alignments.
        enum class PivotAlignments
        {
            MedianPoint,        //!< Pivot center is aligned to the median point (actually the arithmetic average) of all elements. This is the default.
            IndividualOrigins,  //!< Pivot center is aligned to the individual origins of each element.
            ActiveElement,      //!< Pivot center is aligned to the last selected (active) element.
            BoundingBox,        //!< Pivot center is aligned to the bounding box of all element origins.
            Minimum,            //!< Pivot center is aligned to the minimum of the active element's bounding box.
            Maximum,            //!< Pivot center is aligned to the maximum of the active element's bounding box. This will also flip the axes directions.
            GlobalCursor,       //!< Pivot center is aligned to the global cursor position. \see global3DCursorPosition.
        };

        //! Orienation alignments.
        enum class OrientationAlignments
        {
            Global, //!< Orientation is aligned global. Independent of the selected object's orientation. This is the default.
            Local,  //!< Orientation is aligned local. This is only available when one single object is selected. Otherwise "Global" will be used.
            View    //!< Orientation is aligned to the view.
        };

        //! Selection states.
        enum class SelectionStates
        {
            None,       //!< Nothing is selected.
            XAxis,      //!< X axis is selected.
            YAxis,      //!< Y axis is selected.
            ZAxis,      //!< Z axis is selected.
            XYPlane,    //!< XY plane is selected.
            YZPlane,    //!< YZ plane is selected.
            ZXPlane,    //!< ZX plane is selected.
            Uniform,    //!< Uniform selection is activated (all axes).
        };

        /* === Classes === */

        //! Selector transformation raster structure.
        struct FORK_EXPORT Raster
        {
            //! Raster transformation modes.
            enum class Modes
            {
                None,       //!< No raster transformation is performed.
                Relative,   //!< A relative raster is used. This depends on coordinate changes.
                Absolute,   //!< An absolute raster is used. This depends on global coordinates.
            };

            /**
            Returns true if the raster is enabled.
            This is the case when 'mode' is not Modes::None.
            \see mode
            */
            inline bool IsEnabled() const
            {
                return mode != Modes::None;
            }

            /**
            Returns the real raster vector (see the cast operator of 'Math::RasterNumber').
            This is equivalent to:
            \code
            { static_cast<float>(vector.x), static_cast<float>(vector.y), static_cast<float>(vector.z) }
            \endcode
            */
            Math::Vector3f RealVector() const;
            /**
            Returns the raster vector with the current size.
            \see Math::RasterNumber::Raster
            \see size
            */
            Math::Vector3f RasterVector() const;
            /**
            Returns the raster vector with the current size and flushes each vector component.
            \see Math::RasterNumber::RasterFlush
            \see size
            */
            Math::Vector3f RasterFlushVector();

            /**
            Raster transformation mode. By default Modes::None.
            \see size
            \see vector
            */
            Modes mode = Modes::None;

            /**
            Specifies the raster size (when raster mode is enabled).
            This should always be greater than 0.0. By default 1.0.
            \see mode
            */
            float size = 1.0f;

            /**
            Raster vector used for transformation when the raster mode is enabled.
            \see mode
            */
            SelectorModel::RasterVector vector;

            /**
            Specifies whether the selector itself will move free in the raster,
            while all selected objects are transformed inside the raster.
            If false the selector itself will move inside the raster, too.
            By default true.
            */
            bool moveFreeInRaster = true;
        };

        // State class for each coordinate axis.
        struct FORK_EXPORT AxisState
        {
            AxisState() = default;
            AxisState(int)
            {
                /* Dummy */
            }

            //! Returns true if the visibility is greater than 0.5.
            inline bool IsVisible() const
            {
                return visibility > 0.5f;
            }

            /**
            Returns true if this axis is selectable. This is the case when it is visible and enabled.
            \see IsVisible
            \see isEnabled
            */
            inline bool IsSelectable() const
            {
                return IsVisible() && isEnabled;
            }

            //! Specifies whether this axis is enabled (and visible) or not. By default true.
            bool    isEnabled   = true;

            //! Specifies whether this axis is selected (and highlighted) or not. By default false.
            bool    isSelected  = false;

            //! Specifies whether this axis is active for transformation (and highlighted) or not. By default false.
            bool    isActive    = false;

            /**
            Specifies the visibility factor of this axis.
            This is in the range [0.0 .. 1.0]. By default 1.0.
            \see SelectorModel::UpdateAxisVisibility
            */
            float   visibility  = 1.0f;
        };

        /* === Functions === */

        virtual ~SelectorModel();

        /**
        Sets the view.
        \param[in] view Specifies the camera view node from which the selector will be aligned.
        \note This must be called before any "Process..." is called!
        The best is to call this at the beginning of every frame.
        */
        void SetupView(const Scene::CameraNode& view);

        //! Retrieves the world-view matrix of this selector.
        void RetrieveWorldViewMatrix(Math::Matrix4f& matrix) const;

        /**
        Updates the selector transformation depending on the current configuration and all selected nodes.
        This will be called automatically when "SetupView" is called.
        \see SetupView
        */
        void UpdateTransform();

        /**
        Updates the axis visibility. This depends on the angle between the respective axis and the view direction.
        If the axis points nearly toward or backward the view camera, the axis is hidden.
        */
        void UpdateAxisVisibility();

        /* --- Processing --- */

        /**
        Restes the selection states. Will also be called by "ProcessSelection".
        \see ProcessSelection
        */
        void ResetSelection();

        /**
        Processes the selection.
        \param[in] pointerPosition Specifies the pointer (or cursor or mouse) position.
        This can be used as the following example shows:
        \code
        // This works only in fullscreen mode, because "Platform::Cursor::GetPosition"
        // returns the global cursor position (and not relative to the graphics frame).
        selectorModel->ProcessSelection(Platform::Cursor::GetPosition().Cast<float>());
        \endcode
        \return True if any axis is selected.
        */
        virtual bool ProcessSelection(const Math::Point2f& pointerPosition);
        /**
        Processes the activation of the transformation. This should be called, when the mouse button is pressed.
        \param[in] pointerPosition Specifies the pointer (or cursor or mouse) position.
        \param[in] activate Specifies whether the activation is to be enabled or disabled.
        Should be true when the mouse button is pressed down, and false when the mouse button is released.
        \param[in] frameOrigin Optional raw pointer to the frame origin (the left-top point).
        This is only used when the cursor alignment is enabled (see "alignCursor"). By default null.
        \see alignCursor
        */
        virtual void ProcessActivation(const Math::Point2f& pointerPosition, bool activate, const Math::Point2i* frameOrigin = nullptr);

        /**
        Cancels the current activation. This will restore the previous transformation of all selected nodes.
        \see ProcessActivation
        */
        virtual void CancelActivation();

        /**
        Computes the picking position for the current selection state.
        If a single axis is active, the closest line segment between the picking ray
        and the axis direction will be computed. The point which lies onto the axis vector will be returned.
        If a plane is active (i.e. two axes are active), the intersection of the picking ray and that plane will be returned.
        \remarks This function depends on the following internal states: projection (see 'GetProjection'),
        camera matrix (see 'SetupView' and 'GetCameraMatrix') and currently selected axes (see 'GetAxesState').
        \see SetupView
        \see GetProjection
        \see GetCameraMatrix
        \see Projection::PickingRay
        \see GetAxesState
        */
        bool PickPosition(const Math::Point2f& pointerPosition, Math::Point3f& position) const;
        /**
        Computes offset between the current transformation's position and picking position (see 'PickPosition').
        \see PickPosition
        */
        Math::Vector3f PickOffset(const Math::Point2f& pointerPosition) const;

        /* --- Dynamic states --- */

        //! Resizes the selector model.
        void Resize(float scale);
        //! Returns the size of the selector model.
        float Size() const;

        /**
        Sets the axes plane size.
        \param[in] size Specifies the new size factor.
        This will be clamped between the range [0.0 .. 1.0]. By default 0.25.
        */
        void SetupAxesPlaneSize(float size);

        /**
        Returns the active orientation alignment. This is commonly the same as "orientationAlignment",
        but if the number of selected objects is not 1, and the orientation alignment was set as "OrientationAlignments::Local",
        the return value is "OrientationAlignments::Global".
        \see OrientationAlignments::Local
        */
        OrientationAlignments ActiveOrientationAlignment(const OrientationAlignments orientationAlignment) const;
        //! \see ActiveOrientationAlignment(const OrientationAlignments)
        OrientationAlignments ActiveOrientationAlignment() const;

        /**
        Enables or disables the specified selector axis. By default all axes are enabled.
        \param[in] axisIndex Specifies the axis index. This must be 0, 1 or 2. Otherwise the function call has no effect.
        \param[in] enable Specifies whether the axis is to be enabled or disabled. By default true.
        \remarks To retrieve the current state use "GetAxesState" (e.g. 'GetAxesState().x.isEnabled').
        \see GetAxesState
        \see AxisState::isEnabled
        */
        void EnableAxis(size_t axisIndex, bool enable = true);

        /* --- Node selection --- */

        /**
        Adds a new node to the selection list.
        \param[in] node Raw pointer to the selectable node, which is to be selected.
        If this node already exists in the list, it will be pushed at the top of the list,
        which makes it to the active (or rather last selected) node.
        If this is null, the function call has no effect.
        */
        void AddNode(Selectable* node);
        //! Removes a node from the selection list.
        void RemoveNode(Selectable* node);

        //! Clears the node selection list.
        void RemoveAllNodes();

        /**
        Returns true if the specified scene node is already in the list.
        \see AddNode
        */
        bool HasNode(Selectable* node) const;

        /**
        Computes the pivot center point for the scene node list with the specified pivot alignment.
        \param[in] pivotAlignment Specifies the pivot alignment.
        \see PivotAlignments
        */
        Math::Point3f ComputePivotCenter(const PivotAlignments pivotAlignment) const;

        /**
        Computes the orientation alignment for the scene node list with the specified orientation alignment.
        \param[in] orientationAlignment Specifies the orientation alignemnt.
        \see OrientationAlignments
        */
        Math::Quaternionf ComputeOrientation(const OrientationAlignments orientationAlignment) const;

        /**
        Returns true if the pivot center point is distributed.
        This is the case when more than one node is selected but the pivot alignment
        is not 'PivotAlignments::IndividualOrigins', or the pivot alignment it 'PivotAlignments::GlobalCursor'.
        \see ComputePivotCenter
        \see pivotAlignment
        */
        bool IsPivotDistributed() const;

        /* --- Selection states --- */

        /**
        Returns true if any axis or axes plane is selected.
        \see AxisState::IsSelected.
        */
        bool IsAnySelected() const;
        /**
        Returns true if currently an axis is selected.
        \see GetSelectionMode
        */
        bool IsAxisSelection() const;
        /**
        Returns true if currently an plane is selected.
        \see GetSelectionMode
        */
        bool IsPlaneSelection() const;

        /* --- Enabled states --- */

        /**
        Returns true if all axes are enabled.
        \see AxisState::IsEnabled
        */
        bool AreAllAxesEnabled() const;
        /**
        Returns true if with the current state, the axes planes are enabled.
        This is the case for translation mode (TransformModes::Translate).
        */
        bool AreAxesPlanesEnabled() const;
        /**
        Returns true if with the current state, uniform transformation is enabled.
        This is the case for rotation- and scaling mode (TransformModes::Rotate and TransformModes::Scale)
        and if all axes are enabled.
        */
        bool IsUniformTransformEnabled() const;
        /**
        Returns true if picking translation is currently active.
        This is the case when 'pickingOnlyTranslation' is true and 'transformMode' is 'TransformModes::Translate'.
        \see pickingOnlyTranslation
        \see transformMode
        */
        bool IsPickingActive() const;

        /* === Inline functions === */

        //! Returns the transformation for this selector model.
        inline const Math::Transform3Df& GetTransform() const
        {
            return transform_;
        }
        /**
        Returns the previous transformation for this selector model.
        This is only updated when the selector begins to move.
        */
        inline const Math::Transform3Df& GetPrevTransform() const
        {
            return prevTransform_;
        }

        //! Returns the projection matrix. This is used for 2D selection detection.
        inline const Video::Projectionf& GetProjection() const
        {
            return projection_;
        }
        //! Returns the view matrix. This is used for 2D selection detection.
        inline const Math::Matrix4f& GetViewMatrix() const
        {
            return viewMatrix_;
        }
        //! Returns the camera matrix.
        inline const Math::Matrix4f& GetCameraMatrix() const
        {
            return cameraMatrix_;
        }

        /**
        Returns true if the transformation is active,
        i.e. the scene nodes will be transformed (when the user moves the mouse for instance).
        */
        inline bool IsActive() const
        {
            return isActive_;
        }

        /**
        Returns the current axes state.
        \see AxisState
        */
        inline const Math::Vector3<AxisState>& GetAxesState() const
        {
            return axesState_;
        }

        /**
        Returns the list of all selected dynamic scene nodes.
        \see AddNode
        \see RemoveNode
        \see ClearAllNodes
        */
        inline const NodeListType& GetSelectedNodes() const
        {
            return selectedNodes_;
        }
        //! Returns the active (last selected) scene node or null if currently no scene node is selected.
        inline Selectable* GetActiveNode() const
        {
            return activeNode_;
        }

        //! Returns true if any nodes are selected. This is equivalent to '!GetSelectedNodes().empty()'.
        inline bool AreAnyNodesSelected() const
        {
            return !selectedNodes_.empty();
        }

        //! Returns the axes plane size factor.
        inline float GetAxesPlaneSize() const
        {
            return axesPlaneSize_;
        }

        /**
        Returns the current normalized 2D arrow direction.
        \remarks This can be used for motion direction computions.
        Compute the dot product between this 2D vector and the mouse motion 2D vector,
        to get the motion speed factor.
        */
        inline const Math::Vector2f& GetArrowDirection() const
        {
            return transformState_.arrowDirection;
        }
        //! Returns the current selection mode.
        inline SelectionStates GetSelectionState() const
        {
            return transformState_.selectionState;
        }

        /* === Members === */

        //! Specifies whether the selector axes can be selected or not. By default true.
        bool isEnabled = true;

        /**
        Specifies whether axis visibility is to be used or not.
        This will hide an axis when it is nearly directly pointing to the view camera.
        By default true.
        \see UpdateAxisVisibility
        */
        bool useVisibility = true;

        /**
        Transformation mode. This specifies how the selected objects will
        be transformed (translated, rotated or scaled). By default TransformModes::Translate.
        */
        TransformModes transformMode = TransformModes::Translate;

        /**
        Pivot center alignment. This specifies how the selector pivot point
        is computed out of the selected object. By default PivotAlignments::MedianPoint.
        */
        PivotAlignments pivotAlignment = PivotAlignments::MedianPoint;

        /**
        Orientation alignment.
        \remarks The active orientation alignments depends on the transformation mode (see 'transformMode')
        and the number of selected objects. See 'ActiveOrientationAlignment' for more details.
        By default OrientationAlignments::Global.
        \see ActiveOrientationAlignment
        \see transformMode
        */
        OrientationAlignments orientationAlignment = OrientationAlignments::Global;

        //! Selector transformation raster.
        Raster raster;

        /**
        Global 3D cursor position.
        \see PivotAlignments::GlobalCursor
        */
        Math::Point3f global3DCursorPosition;

        /**
        Enables or disables picking only translation.
        This allows to translate objects to the exact position, the cursor points to.
        By default false.
        */
        bool pickingOnlyTranslation = false;

        /**
        Enables or disables cursor alignment.
        This allows to align (or rather move) the cursor to the selector origin, when an axis or plane is selected.
        Cursor alignment is only used for 'picking translations', e.g. when a plane is selected to translated the selected nodes.
        By default false.
        \see pickingOnlyTranslation
        */
        bool alignCursor = false;

    protected:

        /* === Functions === */

        Math::Point3f ComputePivotMedianPoint   () const;
        Math::Point3f ComputePivotActiveElement () const;
        Math::Point3f ComputePivotBoundingBox   () const;
        Math::Point3f ComputePivotMinimum       () const;
        Math::Point3f ComputePivotMaximum       () const;

        Math::Quaternionf ComputeOrientationLocal() const;

        //! Returns the bounding box of the specified selectable node.
        Math::AABB3f BoundingBoxFromNode(Selectable* node) const;

        bool IsLineSelected(
            const Math::Point2f& lineStart, const Math::Point2f& lineEnd,
            const Math::Point2f& pointerPosition, float selectionRadius = 5.0f
        ) const;

        void UpdateSelectionState();

        /**
        Aligns the cursor the selector origin and returns true if alignment was possible.
        \param[in] frameOrigin Specifies the frame origin (the left-top point).
        */
        bool AlignCursorToOrigin(const Math::Point2i& frameOrigin);

        /**
        Stores the transformation of all selected nodes.
        \see Selectable::StoreTransform
        */
        void StoreAllNodeTransforms();
        /**
        Restores the transformation of all selected nodes.
        \see Selectable::RestoreTransform
        */
        void RestoreAllNodeTransforms();

        //! Set the new active node and calls the callbacks for activation and deactivation.
        void SetupActiveNode(Selectable* node);

        /* --- Callbacks --- */

        /**
        Callback when a new node is added to the selection list. The pointer is never null!
        \see AddNode
        */
        virtual void OnAddNode(Selectable* node);
        /**
        Callback when a node is removed from the selection list. The pointer is never null!
        \see RemoveNode
        */
        virtual void OnRemoveNode(Selectable* node);
        /**
        Callback when all nodes are removed from the selection list.
        \see RemoveAllNodes
        */
        virtual void OnRemoveAllNodes();
        /**
        Callback when a node becomes the active one. The pointer is never null!
        \see GetActiveNode
        \see SetupActiveNode
        */
        virtual void OnActivateNode(Selectable* node);
        /**
        Callback when a node becomes inactive. The pointer is never null!
        \see GetActiveNode
        \see SetupActiveNode
        */
        virtual void OnDeactivateNode(Selectable* node);

    private:

        /* === Structures === */
        
        struct TransformState
        {
            //! Selected arrow direction. Will be updated when an arrow is selected and activated.
            Math::Vector2f arrowDirection;

            //! Displacement for the picking functionality (see 'PickOffset').
            Math::Vector3f pickDisplacement;
            
            //! Current selection state.
            SelectionStates selectionState = SelectionStates::None;
        };

        /* === Members === */

        Scene::Transform            transform_;                 //!< Current selector transformation.
        Scene::Transform            prevTransform_;             //!< Previous selector transformation (before user moves the selector).

        Video::Projectionf          projection_;
        Math::Matrix4f              viewMatrix_;
        Math::Matrix4f              cameraMatrix_;

        bool                        isActive_       = false;    //!< Specifies whether the selector is active, i.e. the user presses the mouse button and moves the mouse.

        Math::Vector3<AxisState>    axesState_;                 //!< State of all three axes.

        NodeListType                selectedNodes_;             //!< List of all selected dynamic scene nodes.
        Selectable*                 activeNode_     = nullptr;

        float                       axesPlaneSize_  = 0.25f;    //!< Size factor of the axes planes. By default 0.25.

        TransformState              transformState_;            //!< Internal transformation state.

};


} // /namespace Utility

} // /namespace Fork


#endif



// ========================