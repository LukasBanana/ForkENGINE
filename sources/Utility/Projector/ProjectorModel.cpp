/*
 * Projector model file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Projector/ProjectorModel.h"
#include "Math/Collision/AABBCollisions.h"
#include "Math/Collision/CylinderCollisions.h"
//#include "Math/Collision/ConeCollisions.h"
#include "Math/Core/BaseMath.h"
#include "Math/Core/Rect.h"
#include "Math/Core/Range.h"

#include <limits>


namespace Fork
{

namespace Utility
{


ProjectorModel::ProjectorModel()
{
    BuildTriggerCones();
}
ProjectorModel::~ProjectorModel()
{
}

void ProjectorModel::ResetSelection()
{
    /* Clear selection states */
    mainTriggerState_.isSelected = false;
    for (size_t i = 0; i < 6; ++i)
        triggerStates_[i].isSelected = false;
}

bool ProjectorModel::IsAnySelected() const
{
    if (mainTriggerState_.isSelected)
        return true;
    
    for (size_t i = 0; i < 6; ++i)
    {
        if (triggerStates_[i].isSelected)
            return true;
    }

    return false;
}

bool ProjectorModel::ProcessSelection(const Math::Point2f& pointerPosition)
{
    bool isAnySelected = false;

    /* First, check if the pointer is inside the viewport */
    Math::Recti viewRect { GetViewport().position, GetViewport().size };

    if (viewRect.Overlap(pointerPosition.Cast<int>()))
    {
        Math::Point3f intersections[7];
        bool hasIntersection[7] = { 0 };

        /* Build picking ray */
        auto pickingRay = GetVisualProjection().ViewRay(pointerPosition, ViewMatrix().Inverse());

        /* Check main trigger selection */
        static const float boxSize = 0.3f;
        Math::AABB3f box { Math::Vector3f(-boxSize), Math::Vector3f(boxSize) };

        hasIntersection[0] = Math::CheckIntersectionWithAABB(box, pickingRay, intersections[0]);
        
        /* Check axis trigger selections */
        for (size_t i = 0; i < 6; ++i)
        {
            if (triggerStates_[i].IsVisible())
            {
                auto& cone = triggerCones_[i];

                hasIntersection[i + 1] = Math::CheckIntersectionWithCylinder(
                    Math::Cylinder<float>{ cone.radius, cone.height, cone.orientation },
                    pickingRay.ToLine(100),
                    intersections[i + 1]
                );
            }
        }

        /* Find nearest intersection */
        float maxDistanceSq = std::numeric_limits<float>::max();

        int selectedIndex = -1;

        for (int i = 0; i < 7; ++i)
        {
            if (hasIntersection[i])
            {
                float distanceSq = Math::Distance(pickingRay.origin, intersections[i]);
                if (distanceSq < maxDistanceSq)
                {
                    maxDistanceSq = distanceSq;
                    selectedIndex = i;
                }
            }
        }

        /* Set final selection state */
        ResetSelection();

        if (selectedIndex == 0)
        {
            mainTriggerState_.isSelected = true;
            isAnySelected = true;
        }
        else if (selectedIndex > 0)
        {
            triggerStates_[selectedIndex - 1].isSelected = true;
            isAnySelected = true;
        }
    }
    else
        ResetSelection();

    return isAnySelected;
}

void ProjectorModel::ProcessActivation(const Math::Point2f& pointerPosition)
{
    if (projectionTransition.isActive)
        return;
    
    /* Check for main trigger selection */
    if (mainTriggerState_.isSelected)
    {
        /* Change projection with transition */
        auto projection = GetProjection();
        projection.SetOrtho(!projection.GetOrtho());
        SetupProjection(projection, defaultTransitionDuration);
    }
    else
    {
        /* Check for axis selections */
        static const float angle = Math::pi*0.5f;
        static const Math::Quaternionf triggerRot[] =
        {
            Math::Quaternionf(0, angle, 0),
            Math::Quaternionf(angle, 0, 0),
            Math::Quaternionf(0, 0, 0),
            Math::Quaternionf(0, -angle, 0),
            Math::Quaternionf(-angle, 0, 0),
            Math::Quaternionf(0, angle*2.0f, 0),
        };

        for (size_t i = 0; i < 6; ++i)
        {
            if (triggerStates_[i].isSelected)
            {
                /* Change orientation with transition */
                SetupOrientation(triggerRot[i], defaultTransitionDuration);
                break;
            }
        }
    }
}

void ProjectorModel::Update()
{
    UpdateProjectionTransition();
    UpdateOrientationTransition();
    UpdateTriggerVisibility();
}

void ProjectorModel::CancelTransition()
{
    if (projectionTransition.isActive)
        projectionTransition.Stop(projection_, visualProjection_);
}

void ProjectorModel::SetupProjection(
    const Video::Projectionf& projection, unsigned long long transitionDuration)
{
    if (transitionDuration > 0)
    {
        auto endVisualProjection = projection;
        AdjustVisualProjection(endVisualProjection);

        projectionTransition.Start(
            transitionDuration,
            projection_, projection,
            visualProjection_, endVisualProjection
        );
    }
    else
    {
        projection_ = projection;
        visualProjection_ = projection;
        AdjustVisualProjection(visualProjection_);
    }
}

void ProjectorModel::SetupViewport(
    const Video::Viewport& viewport, const Video::Viewport& visualViewport)
{
    viewport_ = visualViewport;
    visualProjection_.SetViewport(viewport_);
    projection_.SetViewport(viewport);
}

void ProjectorModel::SetupViewportDefaultVisual(
    Video::Viewport viewport, int visualSize,
    float nearPlaneVisual, float farPlaneVisual,
    float nearPlaneMain, float farPlaneMain)
{
    viewport.minDepth = nearPlaneMain;
    viewport.maxDepth = farPlaneMain;

    SetupViewport(
        viewport,
        { { viewport.size.width - visualSize, 0 },
          { visualSize, visualSize },
          nearPlaneVisual, farPlaneVisual }
    );
}

void ProjectorModel::SetupOrientation(
    const Math::Quaternionf& orientation, unsigned long long transitionDuration)
{
    if (orientationSource)
    {
        if (transitionDuration > 0)
        {
            orientationTransition.Start(
                transitionDuration,
                orientationSource->GetRotation(), orientation
            );
        }
        else
            orientationSource->SetRotation(orientation);
    }
}

Math::Matrix4f ProjectorModel::ViewMatrix() const
{
    Math::Matrix4f viewMatrix;
    
    viewMatrix.Translate({ 0, 0, viewDistance });
    viewMatrix *= Orientation().Mat4();
    
    return viewMatrix;
}

Math::Quaternionf ProjectorModel::Orientation() const
{
    return orientationSource != nullptr ? orientationSource->GetRotation().Inverse() : Math::Quaternionf();
}


/*
 * ======= Protected: =======
 */

void ProjectorModel::UpdateProjectionTransition()
{
    if (projectionTransition.isActive)
        projectionTransition.Interpolate(projection_, visualProjection_);
}

void ProjectorModel::UpdateOrientationTransition()
{
    if (orientationTransition.isActive && orientationSource)
    {
        Math::Quaternionf orientation;
        orientationTransition.Interpolate(orientation);
        orientationSource->SetRotation(orientation);
    }
}

void ProjectorModel::UpdateTriggerVisibility()
{
    static const Math::Vector3f viewDir { 0, 0, -1 };
    static const Math::Rangef interpRange { 0.96f, 0.98f };

    static const Math::Vector3f triggerDir[]
    {
        { -1, 0, 0 }, { 0,  1, 0 }, { 0, 0, -1 },
        {  1, 0, 0 }, { 0, -1, 0 }, { 0, 0,  1 },
    };

    for (size_t i = 0; i < 6; ++i)
    {
        /* Update visibility factor for current trigger state */
        auto interp = Math::Dot(Orientation() * triggerDir[i], viewDir);
        triggerStates_[i].visibility = 1.0f - Math::Saturate(interp, interpRange.min, interpRange.max);
    }
}


/*
 * ======= Private: =======
 */

void ProjectorModel::BuildTriggerCones()
{
    const float r = 0.2f, h = 0.5f, pos = 0.8f;

    triggerCones_[0] = { r, h, { { -pos, 0, 0 }, {  1,  0,  0 } } }; // <-- Left
    triggerCones_[1] = { r, h, { { 0,  pos, 0 }, {  0, -1,  0 } } }; // <-- Top
    triggerCones_[2] = { r, h, { { 0, 0, -pos }, {  0,  0,  1 } } }; // <-- Front
    triggerCones_[3] = { r, h, { {  pos, 0, 0 }, { -1,  0,  0 } } }; // <-- Right
    triggerCones_[4] = { r, h, { { 0, -pos, 0 }, {  0,  1,  0 } } }; // <-- Bottom
    triggerCones_[5] = { r, h, { { 0, 0,  pos }, {  0,  0, -1 } } }; // <-- Back
}

void ProjectorModel::AdjustVisualProjection(Video::Projectionf& visualProjection)
{
    visualProjection.SetOrthoSize({ 3, 3 });
    visualProjection.SetViewport(GetViewport());
}


/*
 * ProjectionTransition structure
 */

static float TransitionInterpolator(const std::chrono::system_clock::time_point& startTime, unsigned long long duration)
{
    if (duration > 0)
    {
        auto endTime = std::chrono::system_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        return static_cast<float>(elapsedTime) / static_cast<float>(duration);
    }
    return 0.0f;
}

void ProjectorModel::ProjectionTransition::Start(
    unsigned long long transitionDuration,
    const Video::Projectionf& from, const Video::Projectionf& to,
    const Video::Projectionf& fromVisual, const Video::Projectionf& toVisual)
{
    if (transitionDuration > 0)
    {
        startProjection         = from;
        endProjection           = to;

        startVisualProjection   = fromVisual;
        endVisualProjection     = toVisual;

        isActive                = true;
        duration                = transitionDuration;

        startTime               = std::chrono::system_clock::now();
    }
}

void ProjectorModel::ProjectionTransition::Interpolate(
    Video::Projectionf& projection, Video::Projectionf& visualProjection)
{
    auto t = TransitionInterpolator(startTime, duration);

    if (t < 1.0f)
    {
        t = Math::SmootherStep(Math::Saturate(t));
        projection      .Interpolate(startProjection      , endProjection      , t);
        visualProjection.Interpolate(startVisualProjection, endVisualProjection, t);
    }
    else
        Stop(projection, visualProjection);
}

void ProjectorModel::ProjectionTransition::Stop(
    Video::Projectionf& projection, Video::Projectionf& visualProjection)
{
    projection          = endProjection;
    visualProjection    = endVisualProjection;
    isActive            = false;
}


/*
 * OrientationTransition structure
 */

void ProjectorModel::OrientationTransition::Start(
    unsigned long long transitionDuration, const Math::Quaternionf& from, const Math::Quaternionf& to)
{
    if (transitionDuration > 0)
    {
        startOrientation    = from;
        endOrientation      = to;

        isActive            = true;
        duration            = transitionDuration;

        startTime           = std::chrono::system_clock::now();
    }
}

void ProjectorModel::OrientationTransition::Interpolate(Math::Quaternionf& orientation)
{
    auto t = TransitionInterpolator(startTime, duration);

    if (t < 1.0f)
    {
        t = Math::SmootherStep(Math::Saturate(t));
        orientation.SLerp(startOrientation, endOrientation, t);
    }
    else
        Stop(orientation);
}

void ProjectorModel::OrientationTransition::Stop(Math::Quaternionf& orientation)
{
    orientation = endOrientation;
    isActive    = false;
}


} // /namespace Utility

} // /namespace Fork



// ========================