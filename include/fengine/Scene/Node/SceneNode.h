/*
 * Scene node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_NODE_H__
#define __FORK_SCENE_NODE_H__


#include "Math/Core/Transform3D.h"
#include "Math/Geometry/Frustum.h"
#include "Scene/Node/SceneVisitor.h"
#include "Scene/Manager/SceneNodeSorter.h"
#include "Core/TreeHierarchy/NodeMetaData.h"
#include "Core/Export.h"

#include <memory>
#include <vector>


namespace Fork
{

namespace Scene
{


/**
Standard scene type for node transformations.
\see Math::Transform3D
*/
typedef Math::Transform3Df Transform;

/**
Standard scene type for view frustums.
\see Math::Frustum
*/
typedef Math::Frustum<> ViewFrustum;


//! Scene node base class.
class FORK_EXPORT SceneNode
{
    
    public:
        
        SceneNode() = default;
        SceneNode(const SceneNode&) = delete;
        SceneNode& operator = (const SceneNode&) = delete;
        virtual ~SceneNode();

        /* === Functions === */

        //! Visit function for the scene visitor (Visitor Pattern).
        virtual void Visit(SceneVisitor* visitor);

        /**
        Returns the local transformation of this scene node.
        By default the identity matrix is returned.
        */
        virtual const Math::Matrix4f& LocalTransform() const;

        //! Evalutates the global transformation of this scene node (depending on its parent).
        virtual void GlobalTransform(Math::Matrix4f& matrix) const;
        //! Returns the global transformation of this scene node (depending on its parent).
        virtual Math::Matrix4f GlobalTransform() const;

        /**
        Returns true if this scene node has a transformation,
        e.g. DynamicSceneNode has a transformation but StaticSceneNode does not.
        \note You can still use "LocalTransform" and "GlobalTransform" for
        those scene node which don't have a transformation, but you will get an identity matrix.
        \return True if this scene node has a transformation. By default false.
        */
        virtual bool HasTransform() const;

        //! Adds the specified scene node to the child list.
        void AddChild(const SceneNodePtr& sceneNode);
        //! Removes the specified scene node from the child list.
        void RemoveChild(const SceneNodePtr& sceneNode);
        //! Clears the child list.
        void ReleaseChildren();

        /**
        Sorts the children scene nodes.
        \see SceneNodeSorter
        */
        void SortSceneNodes(
            const Transform& compareTransform, const SceneNodeSorter::SortMethods method, bool isGlobal = false
        );

        /**
        Sets up the new parent for the specified scene node.
        \param[in] sceneNode Specifies the scene node whose parent is to be changed. If this is null, the function has no effect.
        \param[in] parent Raw-pointer to the new parent scene node. To remove the parent, set this to null.
        */
        static void SetupParent(const SceneNodePtr& sceneNode, SceneNode* parent);

        //! Returns a raw-pointer to the scene node's parent.
        inline SceneNode* GetParent() const
        {
            return parent_;
        }

        //! Returns the child list.
        inline const std::vector<SceneNodePtr>& GetChildren() const
        {
            return children_;
        }

        /* === Members === */

        //! Specifies whether this scene node is visible, audible or commonly enabled. By default true.
        bool            isEnabled = true;

        //! Scene node meta data.
        NodeMetaData    metaData;

    private:

        /* === Members === */

        std::vector<SceneNodePtr>   children_;              //!< Child scene nodes.
        SceneNode*                  parent_     = nullptr;  //!< Parent scene node.

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================