/*
 * Skeleton header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SKELETON_H__
#define __FORK_SKELETON_H__


#include "Scene/Node/SceneNode.h"

#include <vector>


namespace Fork
{

namespace Scene
{


DECL_SHR_PTR(Skeleton);

/**
Class for an abstract mesh skeleton.
This can be used in combination with skeletal animation.
\todo Incomplete
*/
class FORK_EXPORT Skeleton
{
    
    public:
        
        //! Vertex weiht for skeleton joints.
        struct VertexWeight
        {
            unsigned int    index;  //!< Vertex index.
            float           weight; //!< Vertex weight factor. Must be in the range [0.0 .. 1.0].
        };

        // Forward declaration
        class Joint;

        //! Joint array list type.
        typedef std::vector<std::unique_ptr<Joint>> JointList;

        //! Skeleton joint (also called "bone") class.
        class FORK_EXPORT Joint
        {
            
            public:
                
                Joint() = default;

                Joint(const Joint&) = delete;
                Joint& operator = (const Joint&) = delete;

                //! Creates a new child node for this joint.
                Joint* CreateChild();
                //! Deletes the specified child node of this joint.
                void DeleteChild(Joint* joint);

                //! Stores the global transformation of this joint inside the specified matrix.
                void GlobalTransform(Math::Matrix4f& matrix) const;
                //! Returns the global transformation of this joint.
                Math::Matrix4f GlobalTransform() const;

                /**
                Fills the specified 4x4 matrix buffer with the matrices of this and all children joint nodes.
                The hierarchy traversal is done in 'pre-order depth-first-search' (DFS).
                \param[out] buffer Pointer to the matrix buffer.
                \param[in] numMatrices Specifies the number of matrices. This may also be zero.
                Use "HierarchySize" to determine the required size for the matrices of the entire joint hierarchy.
                \param[in] isGlobal Specifies whether the transformations are global or
                local relative to the joint parent's transformation. By default true.
                \param[in] isRelative Specifies whether the transformations are relative or absolute.
                Relative means only the deviation from the origin transformation is used.
                Otherwise the absolute transformation of the current joint hierarchy is used.
                For animations this should be relative. By default true.
                \throws NullPointerException If 'buffer' is null.
                \remarks This function can be used to fill the joint transformations in a shader constant buffer.
                \code
                // Usage example:
                struct VertexParam
                {
                    Math::Matrix4f matrices[50];
                }
                vertexParam;

                // Fill matrix buffer
                skeleton.rootJoint.FillMatrixBuffer(vertexParam.matrices, 50);

                // Upload constant buffer onto the GPU
                renderSystem->UpdateBuffer(vertexConstBuffer.get(), vertexParam);
                \endcode
                \note Make sure that the specified buffer is large enough!
                \see HierarchySize
                */
                void FillMatrixBuffer(Math::Matrix4f* buffer, size_t numMatrices, bool isGlobal = true, bool isRelative = false) const;

                //! Returns the size of the joint hierarchy. This includes this joint and all sub joint nodes.
                size_t HierarchySize() const;

                /**
                Normalizes the vertex weights. After this call, the sum of all wegiths are 1.
                \param[in] recursive Specifies whether or not to perform this function
                recursively in the joint hierarchy. By default false.
                \param[in] maxNumWeights Specifies the maximal number of weights to use for normalization.
                This can be used to limit the number of weights in a shader, but the sum of these weights is still 1.
                Set this to 0 for no limit. By default 0.
                \see weights
                \see VertexWeight::weight
                */
                void NormalizeWeights(bool recursive = false, size_t maxNumWeights = 0);

                /*
                Updates the (global) origin matrix. This is called recursively
                for each child and should be started from the root joint.
                \see GetOriginMatrix
                */
                void UpdateOriginMatrix();

                //! Returns a raw pointer to the joint parent node. This may also be null.
                inline Joint* GetParent() const
                {
                    return parent_;
                }

                //! Returns the joint children list.
                inline const JointList& GetChildren() const
                {
                    return children_;
                }

                /**
                Returns the (global) origin matrix for this joint.
                This will be updated when "UpdateOriginMatrix" is called.
                \remarks This is used to transform the vertices:
                \code
                // Pseudocode
                vertexPosition = jointTransform * originMatrix * vertexCoord;
                \endcode
                \see UpdateOriginMatrix
                */
                inline const Math::Matrix4f& GetOriginMatrix() const
                {
                    return originMatrix_;
                }

                //! Skeleton joint meta data.
                NodeMetaData metaData;

                /**
                Local dynamic skeleton joint 3D transformation. To get a global transformation, use "GlobalTransform".
                \see Math::Transform3D
                */
                Transform transform;

                //! Vertex weight list.
                std::vector<VertexWeight> weights;

            private:
                
                Joint(Joint* parent);

                void FillMatrixBufferGlobal(Math::Matrix4f parentMatrix, Math::Matrix4f*& buffer, size_t& numMatrices, bool isRelative) const;
                void FillMatrixBufferLocal(Math::Matrix4f*& buffer, size_t& numMatrices, bool isRelative) const;

                void UpdateOriginMatrix(Math::Matrix4f parentMatrix);

                Joint*          parent_     = nullptr;
                JointList       children_;

                Math::Matrix4f  originMatrix_;  //!< Origin matrix is the inverse parent matrix.

        };

        //! Root skeleton joint. This joint has no parent joint.
        Joint rootJoint;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================