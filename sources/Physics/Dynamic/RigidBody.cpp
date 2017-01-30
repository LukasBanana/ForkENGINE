/*
 * Physics rigid body file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Physics/Dynamic/RigidBody.h"


namespace Fork
{

namespace Physics
{


/*
 * StandardEventHandler class
 */

RigidBody::StandardEventHandler::~StandardEventHandler()
{
}

void RigidBody::StandardEventHandler::OnApplyForceAndTorque(RigidBody* rigidBody)
{
    rigidBody->SetupForce(gravity * rigidBody->Mass());
}

void RigidBody::StandardEventHandler::OnUpdateTransform(RigidBody* rigidBody, const Math::Matrix4f& globalMatrix)
{
    if (rigidBody->transform)
        rigidBody->transform->SetupNonScaleMatrix(globalMatrix);
}


/*
 * PickingEventHandler class
 */

RigidBody::PickingEventHandler::~PickingEventHandler()
{
}

void RigidBody::PickingEventHandler::OnApplyForceAndTorque(RigidBody* rigidBody)
{
    /* Get rigid body states */
    auto matrix     = rigidBody->Transform();
    auto massCenter = rigidBody->MassCenter();
    auto mass       = rigidBody->Mass();
    auto velocity   = rigidBody->Velocity();

    /* Compute force for picking */
    auto force = displacement_ * (mass * pickStiffness);
    auto forceDamp = velocity * (mass * pickDamp);
    force -= forceDamp;

    /* Compute torque for picking (with local point relative to center of mass) */
    auto relativePoint = matrix.RotateVector(localAttachmentPoint_ - massCenter);
    auto torque = Math::Cross(relativePoint, force);

    /* Apply force and torque to rigid body */
    rigidBody->AddForce(force);
    rigidBody->AddTorque(torque);
}

void RigidBody::PickingEventHandler::OnUpdateTransform(RigidBody* rigidBody, const Math::Matrix4f& globalMatrix)
{
    if (rigidBody->transform)
        rigidBody->transform->SetupNonScaleMatrix(globalMatrix);
}

void RigidBody::PickingEventHandler::Init(const Math::Point3f& globalAttatchmentPosition, RigidBody* rigidBody)
{
    displacement_ = {};
    rigidBody_ = rigidBody;

    if (rigidBody_)
    {
        /* Store local attachment point onto rigid body */
        localAttachmentPoint_ = rigidBody_->Transform().Inverse() * globalAttatchmentPosition;
    }
}

void RigidBody::PickingEventHandler::Locate(const Math::Point3f& globalPosition)
{
    static const float maxDisplacementMagnitude = 20.0f;

    if (!rigidBody_)
        return;

    /*
    Convert local attachment point back to (currently)
    global attachment point and compute displacement.
    */
    auto globalAttachmentPoint = rigidBody_->Transform() * localAttachmentPoint_;

    displacement_ = globalPosition - globalAttachmentPoint;

    /* Damp displacement */
    const float magnitude = displacement_.LengthSq();

    if (magnitude > Math::Sq(maxDisplacementMagnitude))
        displacement_ *= (maxDisplacementMagnitude / std::sqrt(magnitude));
}


/*
 * RigidBody class
 */

RigidBody::EventHandlerPtr RigidBody::defaultEventHandler_;

RigidBody::RigidBody() :
    eventHandler_(RigidBody::defaultEventHandler_)
{
}
RigidBody::~RigidBody()
{
}

Body::Types RigidBody::Type() const
{
    return Types::RigidBody;
}

void RigidBody::SetEventHandler(const EventHandlerPtr& eventHandler)
{
    eventHandler_ = eventHandler;
}

void RigidBody::SetDefaultEventHandler(const EventHandlerPtr& eventHandler)
{
    defaultEventHandler_ = eventHandler;
}

void RigidBody::PostApplyForceAndTorque()
{
    if (eventHandler_)
        eventHandler_->OnApplyForceAndTorque(this);
}

void RigidBody::PostUpdateTransform(const Math::Matrix4f& globalMatrix)
{
    if (eventHandler_)
        eventHandler_->OnUpdateTransform(this, globalMatrix);
}


} // /namespace Physics

} // /namespace Fork



// ========================