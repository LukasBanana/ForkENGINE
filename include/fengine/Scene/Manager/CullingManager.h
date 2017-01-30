/*
 * Culling manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CULLING_MANAGER_H__
#define __FORK_CULLING_MANAGER_H__


#include "Scene/Node/DynamicSceneNode.h"
#include "Scene/Geometry/BoundingVolume.h"
#include "Scene/Node/CameraNode.h"
#include "Math/Geometry/Frustum.h"
#include "Math/Geometry/AABB.h"


namespace Fork
{

namespace Scene
{


/**
View frustum culling manager class. This class handles frustum culling to optimize render passes.
\see ViewFrustum
\see BoundingVolume
*/
class FORK_EXPORT CullingManager
{

    public:
        
        /**
        Sets the current projection matrix.
        \remarks This must be called first.
        */
        void SetupProjectionMatrix(const Projection& projection);
        /**
        Sets the current view frustum.
        \remarks This must be called after "SetupProjectionMatrix".
        \see SetupProjectionMatrix
        */
        void SetupViewMatrix(const Math::Matrix4f& matrix);
        /**
        Sets the current world matrix.
        \remarks This must be called after "SetupViewFrustum".
        \see SetupViewFrustum
        */
        void SetupWorldMatrix(const Math::Matrix4f& matrix);

        /**
        Returns true if the bounding volume is inside the current view frustum with the current world transformation.
        \param[in] boundingVolume Specifies the bounding volume to test against the view frustum.
        \return True if the bounding volume is inside the current view frustum. Otherwise false.
        \remarks Before this function can be used, call "SetupProjectionMatrix", "SetupViewMatrix" and "SetupWorldMatrix".
        \see SetupProjectionMatrix
        \see SetupViewMatrix
        \see SetupWorldMatrix
        \see BoundingVolume
        */
        bool IsBoundingVolumeInsideFrustum(const BoundingVolume& boundingVolume) const;
        /**
        Tests the specified bounding box against the current view frustum.
        \see IsBoundingVolumeInsideFrustum
        */
        bool IsBoundingBoxInsideFrustum(const Math::AABB3f& boundingBox) const;
        /**
        Tests the specified bounding sphere against the current view frustum.
        \see IsBoundingVolumeInsideFrustum
        \todo Incomplete!
        */
        bool IsBoundingSphereInsideFrustum(const Math::Sphere<>& boundingSphere) const;

        /**
        Specifies whether culling is enabled or disabled. If this is false,
        all culling testing functions will alreadys return true. By default true.
        */
        bool cullingEnabled = true;

    private:
        
        Math::Point4f ToClipSpace(float x, float y, float z) const;

        ViewFrustum     viewFrustum_;           //!< Current view frustum.

        Math::Matrix4f  worldMatrix_;           //!< Current world matrix.
        Math::Matrix4f  projection_;            //!< Current projection matrix.
        Math::Matrix4f  viewProjection_;        //!< Current view-projection matrix.
        Math::Matrix4f  worldViewProjection_;   //!< Current world-view-projection matrix.

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================