/*
 * Camera node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CAMERA_NODE_H__
#define __FORK_CAMERA_NODE_H__


#include "Scene/Node/DynamicSceneNode.h"
#include "Video/Core/Projection.h"
#include "Math/Geometry/Frustum.h"


namespace Fork
{

namespace Scene
{


/**
Standard camera node projection.
\see Video::Projection
*/
typedef Video::Projection<float> Projection;

/**
Standard camera view frustum.
\see Math::Frustum
*/
typedef Math::Frustum<float> ViewFrustum;


/**
View camera base class.
\note The camera projection must be initialized with a valid viewport
before you can render a scene from this camera's point of view!
\see CameraNode::projection
*/
class FORK_EXPORT CameraNode : public DynamicSceneNode
{
    
    public:
        
        CameraNode() = default;
        virtual ~CameraNode();

        CameraNode(const CameraNode&) = delete;
        CameraNode& operator = (const CameraNode&) = delete;

        /* === Functions === */

        /**
        Renders the camera and updates the view frustum.
        \see UpdateViewFrustum
        \see SceneVisitor::VisitCameraNode
        */
        virtual void Visit(SceneVisitor* visitor);

        /**
        Updates the view matrix and view frustum. The view frustum is in world space.
        \remarks The view matrix is the inverse matrix of the scene node transformation:
        \code
        cameraNode.transform.GetMatrix().Inverse()
        \endcode
        \see GetViewMatrix
        \see GetViewFrustum
        */
        void UpdateView();

        /**
        Projects the specified point with the view matrix of this camera scene node.
        \param[in,out] point Specifies the point (in world-space) which is to be projected.
        \see Video::Projection::ProjectPoint
        */
        bool ProjectPoint(Math::Point4f& point) const;

        /**
        Projects the specified point with the view matrix of this camera scene node.
        \param[in] point Specifies the point (in world-space) which is to be projected.
        \see Video::Projection::ComputeProjectedPoint
        */
        Math::Point4f ComputeProjectedPoint(Math::Point4f point) const;

        /**
        Builds a view ray with the world matrix of this camera scene node.
        \see Video::Projection::ViewRay
        */
        Math::Ray3f ViewRay(const Math::Point2f& point) const;

        //! Returns the view matrix. This is the inverse object matrix of the camera.
        inline const Math::Matrix4f& GetViewMatrix() const
        {
            return viewMatrix_;
        }

        //! Returns the camera's view frustum. This view frustum is in world space.
        inline const ViewFrustum& GetViewFrustum() const
        {
            return viewFrustum_;
        }

        /* === Members === */

        /**
        Camera projection settings.
        \see Video::Projection
        */
        Projection projection;

    private:

        /* === Members === */

        Math::Matrix4f viewMatrix_; //!< Camera view matrix. This is the inverse object matrix (transform.GetMatrix().Inverse()).

        ViewFrustum viewFrustum_;   //!< Camera view frustum.

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================