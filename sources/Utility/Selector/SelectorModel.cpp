/*
 * Selector model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Selector/SelectorModel.h"
#include "Math/Collision/LineCollisions.h"
#include "Math/Collision/RayCollisions.h"
#include "Math/Collision/PlaneCollisions.h"
#include "Math/Common/Transform.h"
#include "Core/STLHelper.h"
#include "Platform/Core/Cursor.h"

#include <algorithm>


namespace Fork
{

namespace Utility
{


/*
 * Raster class
 */

Math::Vector3f SelectorModel::Raster::RealVector() const
{
    return { static_cast<float>(vector.x),
             static_cast<float>(vector.y),
             static_cast<float>(vector.z) };
}

Math::Vector3f SelectorModel::Raster::RasterVector() const
{
    return { vector.x.Raster(size),
             vector.y.Raster(size),
             vector.z.Raster(size) };
}

Math::Vector3f SelectorModel::Raster::RasterFlushVector()
{
    return { vector.x.RasterFlush(size),
             vector.y.RasterFlush(size),
             vector.z.RasterFlush(size) };
}


/*
 * SelectorModel class
 */

SelectorModel::~SelectorModel()
{
}

void SelectorModel::SetupView(const Scene::CameraNode& view)
{
    /* Store new view settings */
    projection_ = view.projection;
    viewMatrix_ = view.GetViewMatrix();
    cameraMatrix_ = view.transform.GetMatrix();

    /* Update selector model transformation */
    UpdateTransform();

    /* Scale transformation for the visual selector */
    auto projMatrix = projection_.GetMatrixLH();
    float projHeight = projMatrix.GetColumn(1).y;
    float visualSize = 0.5f / projHeight;

    /*
    The 4th element (w) of the 4th column (index 3) of the 4x4 projection matrix
    equals 1.0 for an orthogonal matrix and 0.0 for a perspective matrix.
    Use this value to interpolate the 'scale by distance' factor.
    */
    visualSize *= Math::Lerp(
        Math::Distance(transform_.GetPosition(), view.transform.GetPosition()),
        1.0f,
        projMatrix.GetColumn(3).w
    );

    transform_.SetScale(Math::Vector3f(visualSize));
}

void SelectorModel::RetrieveWorldViewMatrix(Math::Matrix4f& matrix) const
{
    matrix = GetViewMatrix();
    matrix *= GetTransform().GetMatrix();
}

void SelectorModel::UpdateTransform()
{
    /* Setup position */
    auto pivotCenter = ComputePivotCenter(pivotAlignment);

    if (raster.IsEnabled() && raster.moveFreeInRaster)
    {
        /* Add offset to suppress raster position for selector */
        transform_.SetPosition(pivotCenter + raster.RealVector());
    }
    else
        transform_.SetPosition(pivotCenter);

    /* Setup rotation */
    transform_.SetRotation(ComputeOrientation(orientationAlignment));
}

void SelectorModel::UpdateAxisVisibility()
{
    if (useVisibility)
    {
        /* Setup point projections */
        Math::Point4f points[4];
        Math::Matrix4f matrix;

        RetrieveWorldViewMatrix(matrix);

        /*
        Compute visibility by projection distances.
        This makes the visibility dependent to the current view projection.
        */
        points[0] = matrix * Math::Point4f(1, 0, 0);
        points[1] = matrix * Math::Point4f(0, 1, 0);
        points[2] = matrix * Math::Point4f(0, 0, 1);
        points[3] = Math::Point4f(matrix.GetPosition());

        projection_.ProjectPoint(points[3]);
        for (size_t i = 0; i < 3; ++i)
            projection_.ProjectPoint(points[i]);

        auto ComputeVisibility = [](const Math::Point4f& origin, const Math::Point4f& point)
        {
            auto d = Math::Distance(origin.Vec2(), point.Vec2());
            return Math::Saturate(d, 10.0f, 15.0f);
        };

        /* Compute visibility for each axis */
        axesState_.x.visibility = ComputeVisibility(points[3], points[0]);
        axesState_.y.visibility = ComputeVisibility(points[3], points[1]);
        axesState_.z.visibility = ComputeVisibility(points[3], points[2]);
    }
    else
    {
        /* Always set visibility factor to 1.0 */
        axesState_.x.visibility = 1.0f;
        axesState_.y.visibility = 1.0f;
        axesState_.z.visibility = 1.0f;
    }
}

/* --- Processing --- */

void SelectorModel::ResetSelection()
{
    isActive_ = false;

    axesState_.x.isSelected = false;
    axesState_.y.isSelected = false;
    axesState_.z.isSelected = false;

    transformState_.selectionState = SelectionStates::None;
}

bool SelectorModel::ProcessSelection(const Math::Point2f& pointerPosition)
{
    if (IsActive())
        return false;

    ResetSelection();

    if (!isEnabled)
        return false;

    UpdateAxisVisibility();

    /* Setup collision lines */
    Math::Matrix4f matrix;
    RetrieveWorldViewMatrix(matrix);

    /* Setup points */
    Math::Point4f points[3], halfPoints[3], planePoints[3];

    auto origin = Math::Point4f(matrix.GetPosition());

    points[0] = matrix * Math::Point4f(1, 0, 0);
    points[1] = matrix * Math::Point4f(0, 1, 0);
    points[2] = matrix * Math::Point4f(0, 0, 1);

    halfPoints[0] = matrix * Math::Point4f(GetAxesPlaneSize(), 0, 0);
    halfPoints[1] = matrix * Math::Point4f(0, GetAxesPlaneSize(), 0);
    halfPoints[2] = matrix * Math::Point4f(0, 0, GetAxesPlaneSize());

    planePoints[0] = matrix * Math::Point4f(GetAxesPlaneSize(), GetAxesPlaneSize(), 0); // XY plane
    planePoints[1] = matrix * Math::Point4f(0, GetAxesPlaneSize(), GetAxesPlaneSize()); // YZ plane
    planePoints[2] = matrix * Math::Point4f(GetAxesPlaneSize(), 0, GetAxesPlaneSize()); // ZX plane

    /* Setup point projections */
    projection_.ProjectPoint(origin);

    for (size_t i = 0; i < 3; ++i)
    {
        projection_.ProjectPoint(points     [i]);
        projection_.ProjectPoint(halfPoints [i]);
        projection_.ProjectPoint(planePoints[i]);
    }

    /* Process axes plane selection */
    bool isAnySelected = false;

    if (AreAxesPlanesEnabled())
    {
        for (size_t i = 0; i < 3; ++i)
        {
            size_t indexA = i;
            size_t indexB = (i + 1) % 3;

            if ( axesState_[indexA].IsSelectable() && axesState_[indexB].IsSelectable() &&
                 ( IsLineSelected(halfPoints[indexA].Vec2(), planePoints[i].Vec2(), pointerPosition) ||
                   IsLineSelected(halfPoints[indexB].Vec2(), planePoints[i].Vec2(), pointerPosition) ) )
            {
                axesState_[indexA].isSelected = true;
                axesState_[indexB].isSelected = true;
                isAnySelected = true;
                break;
            }
        }
    }

    /* Process axis selection */
    if (!isAnySelected)
    {
        for (size_t i = 0; i < 3; ++i)
        {
            if (axesState_[i].IsSelectable() && IsLineSelected(origin.Vec2(), points[i].Vec2(), pointerPosition))
            {
                /*
                Mark axis as selected and pre-compute arrow-
                direction (for later transformation meta data).
                */
                axesState_[i].isSelected = true;
                isAnySelected = true;

                transformState_.arrowDirection = points[i].Vec2() - origin.Vec2();
                transformState_.arrowDirection.Normalize();

                break;
            }
        }
    }

    UpdateSelectionState();

    return isAnySelected;
}

void SelectorModel::ProcessActivation(const Math::Point2f& pointerPosition, bool activate, const Math::Point2i* frameOrigin)
{
    if (isEnabled && IsAnySelected())
    {
        /* Setup axes activation states */
        if (activate)
        {
            axesState_.x.isActive = axesState_.x.isSelected;
            axesState_.y.isActive = axesState_.y.isSelected;
            axesState_.z.isActive = axesState_.z.isSelected;
        }
        else
        {
            axesState_.x.isActive = false;
            axesState_.y.isActive = false;
            axesState_.z.isActive = false;
        }

        /* Store special states only once when the selection was previously not active */
        if (!isActive_ && activate)
        {
            /* Store previous transformation */
            StoreAllNodeTransforms();
            prevTransform_ = GetTransform();

            if (alignCursor && frameOrigin)
            {
                /* Align cursor to selector origin and reset picking displacement */
                AlignCursorToOrigin(*frameOrigin);
                transformState_.pickDisplacement = {};
            }
            else
            {
                /* Update picking displacement */
                Math::Point3f position;
                if (PickPosition(pointerPosition, position))
                    transformState_.pickDisplacement = position - GetTransform().GetPosition();
            }
        }

        /* Store new activation state */
        isActive_ = activate;
    }
}

void SelectorModel::CancelActivation()
{
    if (isEnabled && isActive_)
    {
        /* Reset states */
        isActive_ = false;

        axesState_.x.isActive = false;
        axesState_.y.isActive = false;
        axesState_.z.isActive = false;

        /* Restore node transformations */
        RestoreAllNodeTransforms();
    }
}

bool SelectorModel::PickPosition(const Math::Point2f& pointerPosition, Math::Point3f& position) const
{
    /* Compute picking ray with current selector view */
    auto pickingRay = GetProjection().ViewRay(pointerPosition, GetCameraMatrix());

    const auto& matrix = GetTransform().GetMatrix();

    /* Get active plane (is used) */
    Math::Plane<float> plane;

    if (axesState_.z.isActive && axesState_.y.isActive)
        plane.normal = { 1, 0, 0 };
    else if (axesState_.y.isActive && axesState_.x.isActive)
        plane.normal = { 0, 0, 1 };
    else if (axesState_.x.isActive && axesState_.z.isActive)
        plane.normal = { 0, 1, 0 };
    else
    {
        /* Setup axis direction */
        Math::Vector3f axisDir;

        if (axesState_.x.isActive)
            axisDir = matrix.RotateVector({ 1, 0, 0 });
        else if (axesState_.y.isActive)
            axisDir = matrix.RotateVector({ 0, 1, 0 });
        else if (axesState_.z.isActive)
            axisDir = matrix.RotateVector({ 0, 0, 1 });
    
        /* Compute closest line segment */
        auto closestSegment = Math::ComputeClosestSegmentBetweenRay(
            pickingRay, Math::Ray3f { GetTransform().GetPosition(), axisDir }
        );

        position = closestSegment.end;
        return true;
    }

    /* Compute intersection between plane and picking ray */
    plane = Math::Transform(matrix, plane);

    return Math::CheckIntersectionWithPlane(plane, pickingRay, position);
}

Math::Vector3f SelectorModel::PickOffset(const Math::Point2f& pointerPosition) const
{
    Math::Point3f position;
    if (PickPosition(pointerPosition, position))
        return position - GetTransform().GetPosition() - transformState_.pickDisplacement;
    return {};
}

/* --- Dynamic states --- */

void SelectorModel::Resize(float scale)
{
    transform_.SetScale(Math::Vector3f(scale));
}

float SelectorModel::Size() const
{
    return transform_.GetScale().x;
}

void SelectorModel::SetupAxesPlaneSize(float size)
{
    axesPlaneSize_ = Math::Saturate(size);
}

SelectorModel::OrientationAlignments SelectorModel::ActiveOrientationAlignment(
    const OrientationAlignments orientationAlignment) const
{
    return (orientationAlignment == OrientationAlignments::Local && selectedNodes_.size() != 1) ?
        OrientationAlignments::Global :
        orientationAlignment;
}

SelectorModel::OrientationAlignments SelectorModel::ActiveOrientationAlignment() const
{
    return ActiveOrientationAlignment(orientationAlignment);
}

void SelectorModel::EnableAxis(size_t axisIndex, bool enable)
{
    if (axisIndex < 3)
        axesState_[axisIndex].isEnabled = enable;
}

/* --- Transformation selection --- */

void SelectorModel::AddNode(Selectable* node)
{
    if (!node)
        return;

    /* Check if node is already in the list */
    auto it = std::find(selectedNodes_.begin(), selectedNodes_.end(), node);

    if (it != selectedNodes_.end())
    {
        /* Move selected node to the back of the list (to be the last selected one) */
        auto itNext = it;
        for (++itNext; itNext != selectedNodes_.end(); ++itNext)
            std::swap(*it, *itNext);
    }
    else
    {
        /* Add node to selection list */
        OnAddNode(node);
        selectedNodes_.push_back(node);
    }

    /* Make new node to the active one */
    SetupActiveNode(node);
}

void SelectorModel::RemoveNode(Selectable* node)
{
    if (!node)
        return;
    
    /* Remove node from selection list */
    auto it = std::find(selectedNodes_.begin(), selectedNodes_.end(), node);
    if (it != selectedNodes_.end())
    {
        OnRemoveNode(node);
        selectedNodes_.erase(it);
    }

    /* Check if removed scene node was the active (last selected) scene node */
    if (activeNode_ == node)
    {
        /* Setup new active element */
        SetupActiveNode(selectedNodes_.empty() ? nullptr : selectedNodes_.back());
    }
}

void SelectorModel::RemoveAllNodes()
{
    OnRemoveAllNodes();
    selectedNodes_.clear();
    SetupActiveNode(nullptr);
}

bool SelectorModel::HasNode(Selectable* node) const
{
    /* Return true if the specified scene node has been found in the selected node list */
    return std::find(selectedNodes_.begin(), selectedNodes_.end(), node) != selectedNodes_.end();
}

Math::Point3f SelectorModel::ComputePivotCenter(const PivotAlignments pivotAlignment) const
{
    if (!selectedNodes_.empty())
    {
        switch (pivotAlignment)
        {
            case PivotAlignments::MedianPoint:
            case PivotAlignments::IndividualOrigins:
                return ComputePivotMedianPoint();
            case PivotAlignments::ActiveElement:
                return ComputePivotActiveElement();
            case PivotAlignments::BoundingBox:
                return ComputePivotBoundingBox();
            case PivotAlignments::Minimum:
                return ComputePivotMinimum();
            case PivotAlignments::Maximum:
                return ComputePivotMaximum();
            case PivotAlignments::GlobalCursor:
                return global3DCursorPosition;
        }
    }
    return {};
}

Math::Quaternionf SelectorModel::ComputeOrientation(const OrientationAlignments orientationAlignment) const
{
    switch (ActiveOrientationAlignment(orientationAlignment))
    {
        case OrientationAlignments::Global:
            return Math::Quaternionf();
        case OrientationAlignments::Local:
            return ComputeOrientationLocal();
        case OrientationAlignments::View:
            return cameraMatrix_.GetRotation();
    }
    return {};
}

bool SelectorModel::IsPivotDistributed() const
{
    return ( selectedNodes_.size() > 1 && pivotAlignment != PivotAlignments::IndividualOrigins ) || pivotAlignment == PivotAlignments::GlobalCursor;
}

/* --- Selection states --- */

bool SelectorModel::IsAnySelected() const
{
    return
        axesState_.x.isSelected ||
        axesState_.y.isSelected ||
        axesState_.z.isSelected;
}

bool SelectorModel::IsAxisSelection() const
{
    return
        transformState_.selectionState >= SelectionStates::XAxis &&
        transformState_.selectionState <= SelectionStates::ZAxis;
}

bool SelectorModel::IsPlaneSelection() const
{
    return
        transformState_.selectionState >= SelectionStates::XYPlane &&
        transformState_.selectionState <= SelectionStates::ZXPlane;
}

/* --- Enable states --- */

bool SelectorModel::AreAllAxesEnabled() const
{
    return
        axesState_.x.isEnabled &&
        axesState_.y.isEnabled &&
        axesState_.z.isEnabled;
}

bool SelectorModel::AreAxesPlanesEnabled() const
{
    return transformMode == TransformModes::Translate;
}

bool SelectorModel::IsUniformTransformEnabled() const
{
    return
        AreAllAxesEnabled() &&
        ( transformMode == TransformModes::Rotate ||
          transformMode == TransformModes::Scale );
}

bool SelectorModel::IsPickingActive() const
{
    return pickingOnlyTranslation && transformMode == TransformModes::Translate;
}


/*
 * ======= Protected: =======
 */

Math::Point3f SelectorModel::ComputePivotMedianPoint() const
{
    Math::Point3f pivotPoint;
    
    for (const auto& node : selectedNodes_)
        pivotPoint += node->GlobalTransform().GetPosition();

    return pivotPoint / static_cast<float>(selectedNodes_.size());
}

Math::Point3f SelectorModel::ComputePivotActiveElement() const
{
    return activeNode_ != nullptr ?
        activeNode_->GlobalTransform().GetPosition() :
        Math::Point3f();
}

Math::Point3f SelectorModel::ComputePivotBoundingBox() const
{
    Math::AABB3f boundingBox;

    for (const auto& node : selectedNodes_)
        boundingBox.InsertPoint(node->GlobalTransform().GetPosition());

    return boundingBox.Center();
}

Math::Point3f SelectorModel::ComputePivotMinimum() const
{
    return activeNode_ != nullptr ?
        activeNode_->GlobalTransform() * BoundingBoxFromNode(activeNode_).min :
        Math::Point3f();
}

Math::Point3f SelectorModel::ComputePivotMaximum() const
{
    return activeNode_ != nullptr ?
        activeNode_->GlobalTransform() * BoundingBoxFromNode(activeNode_).max :
        Math::Point3f();
}

Math::Quaternionf SelectorModel::ComputeOrientationLocal() const
{
    return activeNode_ != nullptr ?
        activeNode_->GlobalTransform().GetRotation() :
        Math::Quaternionf();
}

Math::AABB3f SelectorModel::BoundingBoxFromNode(Selectable* node) const
{
    //todo...
    return { Math::Vector3f(0.0f), Math::Vector3f(0.0f) };
}

bool SelectorModel::IsLineSelected(
    const Math::Point2f& lineStart, const Math::Point2f& lineEnd,
    const Math::Point2f& pointerPosition, float selectionRadius) const
{
    return Math::ComputeDistanceToLine(Math::Line2f{ lineStart, lineEnd }, pointerPosition) < selectionRadius;
}

void SelectorModel::UpdateSelectionState()
{
    /* Check for uniform selection */
    if (axesState_.x.isSelected && axesState_.y.isSelected && axesState_.z.isSelected)
        transformState_.selectionState = SelectionStates::Uniform;
    
    /* Check for plane selections */
    else if (axesState_.x.isSelected && axesState_.y.isSelected)
        transformState_.selectionState = SelectionStates::XYPlane;
    else if (axesState_.y.isSelected && axesState_.z.isSelected)
        transformState_.selectionState = SelectionStates::YZPlane;
    else if (axesState_.z.isSelected && axesState_.x.isSelected)
        transformState_.selectionState = SelectionStates::ZXPlane;

    /* Check for axis selections */
    else if (axesState_.x.isSelected)
        transformState_.selectionState = SelectionStates::XAxis;
    else if (axesState_.y.isSelected)
        transformState_.selectionState = SelectionStates::YAxis;
    else if (axesState_.z.isSelected)
        transformState_.selectionState = SelectionStates::ZAxis;

    /* Default state */
    else
        transformState_.selectionState = SelectionStates::None;
}

bool SelectorModel::AlignCursorToOrigin(const Math::Point2i& frameOrigin)
{
    /* Get projected point */
    auto point = Math::Point4f(GetViewMatrix() * GetTransform().GetPosition());
    if (GetProjection().ProjectPoint(point))
    {
        /* Set cursor position to projected point */
        Math::Point2f winCoord { point.x, point.y };
        Platform::Cursor::SetupPosition(frameOrigin + winCoord.Cast<int>());
        return true;
    }
    return false;
}

void SelectorModel::StoreAllNodeTransforms()
{
    for (auto node : selectedNodes_)
        node->StoreTransform();
}

void SelectorModel::RestoreAllNodeTransforms()
{
    for (auto node : selectedNodes_)
        node->RestoreTransform();
}

void SelectorModel::SetupActiveNode(Selectable* node)
{
    if (activeNode_ != node)
    {
        /* Callback for deactiviation */
        if (activeNode_)
            OnDeactivateNode(activeNode_);

        /* Set new active node */
        activeNode_ = node;

        /* Callback for activation */
        if (activeNode_)
            OnActivateNode(activeNode_);
    }
}

void SelectorModel::OnAddNode(Selectable* node)
{
    // dummy
}

void SelectorModel::OnRemoveNode(Selectable* node)
{
    // dummy
}

void SelectorModel::OnRemoveAllNodes()
{
    // dummy
}

void SelectorModel::OnActivateNode(Selectable* node)
{
    // dummy
}

void SelectorModel::OnDeactivateNode(Selectable* node)
{
    // dummy
}


} // /namespace Utility

} // /namespace Fork



// ========================