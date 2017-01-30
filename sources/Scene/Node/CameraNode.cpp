/*
 * Camera node file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Node/CameraNode.h"


namespace Fork
{

namespace Scene
{


CameraNode::~CameraNode()
{
}

void CameraNode::Visit(SceneVisitor* visitor)
{
    UpdateView();
    visitor->VisitCameraNode(this);
}

void CameraNode::UpdateView()
{
    /* Get view matrix -> inverse camera's object matrix */
    viewMatrix_ = transform.GetMatrix();
    viewMatrix_.MakeInverse();

    /* Build frustum */
    viewFrustum_.Build(projection.GetMatrixLH() * viewMatrix_);
}

bool CameraNode::ProjectPoint(Math::Point4f& point) const
{
    point = GlobalTransform().Inverse() * point;
    return projection.ProjectPoint(point);
}

Math::Point4f CameraNode::ComputeProjectedPoint(Math::Point4f point) const
{
    ProjectPoint(point);
    return point;
}

Math::Ray3f CameraNode::ViewRay(const Math::Point2f& point) const
{
    return projection.ViewRay(point, transform.GetMatrix());
}


} // /namespace Scene

} // /namespace Fork



// ========================