/*
 * Physics rigid body header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_RIGID_BODY_H__
#define __FORK_PHYSICS_RIGID_BODY_H__


#include "Physics/Dynamic/Body.h"
#include "Physics/PhysicsConstants.h"
#include "Math/Core/Vector3.h"


namespace Fork
{

namespace Physics
{


DECL_SHR_PTR(RigidBody);

/**
Physics rigid body interface. Rigid bodies are dynamic physics objects,
whose position and rotation are physically simulated, but they will not be deformed (such as soft bodies).
*/
class FORK_EXPORT RigidBody : public virtual Body
{
    
    public:
        
        /* === Classes === */

        //! Dynamics simulation event handler interface.
        class FORK_EXPORT EventHandler
        {
            
            public:
                
                virtual ~EventHandler()
                {
                }

                //! This is called, when the forces and torques will be applied.
                virtual void OnApplyForceAndTorque(RigidBody* rigidBody)
                {
                    /* Dummy */
                }
                //! This is called, when the transformation of a rigid body will be updated.
                virtual void OnUpdateTransform(RigidBody* rigidBody, const Math::Matrix4f& globalMatrix)
                {
                    /* Dummy */
                }

        };

        typedef std::shared_ptr<EventHandler> EventHandlerPtr;

        //! Standard dynamics simulation event handler implementation.
        class FORK_EXPORT StandardEventHandler : public EventHandler
        {
            
            public:
                
                virtual ~StandardEventHandler();

                virtual void OnApplyForceAndTorque(RigidBody* rigidBody) override;
                virtual void OnUpdateTransform(RigidBody* rigidBody, const Math::Matrix4f& globalMatrix) override;

                //! Gravity vector. By default { 0, -Physics::earthGravity, 0 }.
                Math::Vector3f gravity { 0, -earthGravity, 0 };

        };

        typedef std::shared_ptr<StandardEventHandler> StandardEventHandlerPtr;

        //! Picking event handler implementation. This can be used when a rigid body is grabed with the mouse.
        class FORK_EXPORT PickingEventHandler : public EventHandler
        {
            
            public:
                
                virtual ~PickingEventHandler();

                void OnApplyForceAndTorque(RigidBody* rigidBody) override;
                void OnUpdateTransform(RigidBody* rigidBody, const Math::Matrix4f& globalMatrix) override;

                //! Initializes the picking process.
                void Init(const Math::Point3f& globalAttatchmentPosition, RigidBody* rigidBody);
                //! Updates the picking process.
                void Locate(const Math::Point3f& globalPosition);

                //! Picking stiffness. By default 100.0.
                float pickStiffness = 100.0f;

                //! Picking damp. By default 10.0.
                float pickDamp = 10.0f;

            private:
                
                Math::Vector3f displacement_;           //!< Relative picking displacement.
                Math::Point3f localAttachmentPoint_;    //!< Local body attachment point.

                RigidBody* rigidBody_ = nullptr;

        };

        typedef std::shared_ptr<PickingEventHandler> PickingEventHandlerPtr;

        /* === Interface === */

        virtual ~RigidBody();

        //! Returns Types::RigidBody.
        Types Type() const;

        /* --- Dynamics simulation --- */

        //! Sets the mass of this rigid body.
        virtual void SetupMass(float mass) = 0;
        //! Returns the mass of this rigid body.
        virtual float Mass() const = 0;

        /**
        Sets the center of mass of this rigid body.
        \param[in] relativeCenter Specifies the center of mass (in object space). By default { 0, 0, 0 }.
        */
        virtual void SetupMassCenter(const Math::Vector3f& relativeCenter) = 0;
        //! Returns the center of mass of this rigid body.
        virtual Math::Vector3f MassCenter() const = 0;

        /**
        Sets the velocity of this rigid body.
        \param[in] globalDirection Specifies the direction (in world space) in which the body is to be moved.
        */
        virtual void SetupVelocity(const Math::Vector3f& globalDirection) = 0;
        //! Returns the current velocity of this rigid body.
        virtual Math::Vector3f Velocity() const = 0;

        /**
        Sets the force vector of this rigid body.
        \param[in] forceVector Specifies the force vector. This could be "gravityVector * rigidBodyMass":
        \code
        void OnApplyForceAndTorque(RigidBody* rigidBody)
        {
            Math::Vector3f gravityVector { 0, -9.81f, 0 };
            rigidBody->SetupForce(gravityVector * rigidBody->Mass());
        }
        \endcode
        \note This can only be used inside a call of "EventHandler::OnApplyForceAndTorque"!
        \see SetEventHandler
        */
        virtual void SetupForce(const Math::Vector3f& force) = 0;
        /**
        Adds the force vector to this rigid body.
        \see SetupForce
        */
        virtual void AddForce(const Math::Vector3f& force) = 0;

        /**
        Sets the torque of this rigid body.
        \note This can only be used inside a call of "EventHandler::OnApplyForceAndTorque"!
        \see SetEventHandler
        */
        virtual void SetupTorque(const Math::Vector3f& torque) = 0;
        /**
        Adds the torque vector to this rigid body.
        \see SetupTorque
        */
        virtual void AddTorque(const Math::Vector3f& torque) = 0;

        /**
        Adds an impulse to the rigid body. This can be used when a bullet hits a rigid body for instance.
        \param[in] direction Specifies the direction (in world space) of the impulse.
        \param[in] pivotPoint Specifies the pivot point (in world space) where the impulse will be added to the rigid body.
        This point should be placed onto the geometry (but it'S not required).
        */
        virtual void AddImpulse(const Math::Vector3f& direction, const Math::Vector3f& pivotPoint) = 0;

        /* --- Event handler --- */

        /**
        Sets the dynamics simulation event handler for this rigid body.
        \see EventHandler
        \see SetDefaultEventHandler
        */
        void SetEventHandler(const EventHandlerPtr& eventHandler);

        /**
        Sets the default event handler.
        \see EventHandler
        \see SetEventHandler
        */
        static void SetDefaultEventHandler(const EventHandlerPtr& eventHandler);

        /**
        Posts an 'apply force and torque' event.
        \see EventHandler::OnApplyForceAndTorque
        */
        void PostApplyForceAndTorque();
        /**
        Posts an 'update transform' event.
        \see EventHandler::OnUpdateTransform
        */
        void PostUpdateTransform(const Math::Matrix4f& globalMatrix);

    protected:
        
        RigidBody();

        EventHandlerPtr eventHandler_;

        static EventHandlerPtr defaultEventHandler_;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================